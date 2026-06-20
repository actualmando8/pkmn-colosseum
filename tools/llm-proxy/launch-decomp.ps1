<#
  launch-decomp.ps1 - build the "decomp" tmux (psmux) cockpit, 9 panes:

     +-----------+-----------+-----------+-----------+
     | 0 PROXY   | 1 CODEX-1 | 2 CODEX-2 | 3 CODEX-3 |
     +-----------+-----------+-----------+-----------+
     | 4 CODEX-4 | 5 GLM     | 6 SONNET  | 7 OPUS wk |
     +-----------+-----------+-----------+-----------+
     | 8 OPUS ORCHESTRATOR (you type here)          |
     +----------------------------------------------+

  Fleet (codex-heavy - we have far more Codex headroom than Claude):
  - pane 0  proxy    - local router (tools/llm-proxy/proxy.js); only GLM routes through it.
  - panes 1-4 codex  - 4x Codex CLI (gpt-5.5). The workhorses.
  - pane 5  glm      - Claude Code via the proxy, pinned glm-5.2[1m] (isolated config + key).
  - pane 6  sonnet   - Claude Code Sonnet (DIRECT; Max bypasses the proxy - see note).
  - pane 7  worker   - a 2nd Opus the orchestrator dispatches hard structural work to.
  - pane 8  orch.    - the orchestrator Opus; the pane you're dropped into on attach.

  NOTE (auth): Claude Max-subscription traffic ignores ANTHROPIC_BASE_URL and goes
  straight to claude.ai, so the Opus/Sonnet panes CANNOT be proxied and run DIRECT.
  Only the GLM pane is proxied (it uses an API key, not Max OAuth). Live usage on the
  dashboard is therefore: GLM via the proxy; Claude/Codex via tools/decomp_work/agent_limits.json.

  - All Claude/Codex panes run --dangerously-skip-permissions for full autonomy.
  - After build, writes tools/decomp_work/tmux_control/panes.env so control.sh
    resolves codex/codex2..4/glm/sonnet/worker to the right panes (claude = orchestrator = self).
  - Panes are cmd shells (the reliable psmux default on this machine).

  GLM key resolution order: $env:GLM_API_KEY  ->  tools/llm-proxy/.glm_key file.

  Usage:
    powershell -ExecutionPolicy Bypass -File launch-decomp.ps1
    powershell -ExecutionPolicy Bypass -File launch-decomp.ps1 -NoAttach
  (or just double-click launch-decomp.bat)
#>
param(
  [switch]$NoAttach,
  [switch]$DryRun,          # build the layout with harmless echo placeholders
  [int]$Port = 8788,        # proxy listen port (8787 is taken by openbb-workspace-mcp)
  [string]$Session = "decomp"
)

$ErrorActionPreference = "Continue"

# --- paths ---
$repo  = (Resolve-Path (Join-Path $PSScriptRoot "..\..")).Path
$tm    = Join-Path $env:USERPROFILE "bin\tmux.exe"
if (-not (Test-Path $tm)) { $tm = "tmux" }

# --- GLM key ---
# Resolve a key from $env or an existing .glm_key, then persist it to .glm_key
# (gitignored, written BOM-free). The proxy reads the file itself, so the secret
# never appears in the pane command line, scrollback, or process args.
$keyFile = Join-Path $PSScriptRoot ".glm_key"
$glmKey = $env:GLM_API_KEY
if (-not $glmKey -and (Test-Path $keyFile)) { $glmKey = (Get-Content $keyFile -Raw).Trim() }
if ($glmKey) {
  [System.IO.File]::WriteAllText($keyFile, $glmKey.Trim())   # UTF-8, no BOM
} else {
  Write-Host "WARNING: no GLM key found (set `$env:GLM_API_KEY or create tools\llm-proxy\.glm_key)." -ForegroundColor Yellow
  Write-Host "         The proxy will still start, but GLM requests will be rejected until a key is set." -ForegroundColor Yellow
}

# --- GLM isolated CLAUDE config dir ---
# The GLM pane runs Claude Code in API-key mode pointed at the proxy. If it shares
# the normal ~/.claude config it ALSO sees the claude.ai OAuth login, and Claude
# warns "Both claude.ai and ANTHROPIC_API_KEY set - auth may not work" then throws
# 400 Authentication Failed (the dual-auth conflict, which also triggers a retry
# storm that trips z.ai's rate limit). Fix: give GLM its OWN config dir with NO
# .credentials.json, so it has a distinct identity and uses only the proxy key.
$glmConfigDir = Join-Path $env:USERPROFILE ".claude-glm"
if (-not (Test-Path $glmConfigDir)) { New-Item -ItemType Directory -Force -Path $glmConfigDir | Out-Null }
$glmJson = Join-Path $glmConfigDir ".claude.json"
if (-not (Test-Path $glmJson)) {
  # Seed onboarding-complete + pre-approved proxy key so the pane boots non-interactively.
  $seed = '{"hasCompletedOnboarding":true,"lastOnboardingVersion":"2.1.179","numStartups":200,"theme":"dark","autoUpdates":false,"customApiKeyResponses":{"approved":["zai-proxy ","zai-proxy"],"rejected":[]},"bypassPermissionsModeAccepted":true,"projects":{}}'
  [System.IO.File]::WriteAllText($glmJson, $seed)
}
# Never let the claude.ai login leak into the isolated dir.
$glmCreds = Join-Path $glmConfigDir ".credentials.json"
if (Test-Path $glmCreds) { Remove-Item -Force $glmCreds }

# --- per-pane cmd command lines ---
# No secret is embedded: the proxy reads the key from .glm_key on its own.
$cdRepo = 'cd /d "' + $repo + '"'

$proxyCmd  = $cdRepo + ' && set "PORT=' + $Port + '" && node "tools\llm-proxy\proxy.js"'
$codexCmd  = $cdRepo + ' && codex'
$glmCmd    = $cdRepo + ' && set "CLAUDE_CONFIG_DIR=' + $glmConfigDir + '" && set "ANTHROPIC_BASE_URL=http://127.0.0.1:' + $Port + '" && set "ANTHROPIC_API_KEY=zai-proxy" && claude --model "glm-5.2[1m]" --dangerously-skip-permissions'
# Sonnet + Opus run DIRECT (Max OAuth ignores the proxy; routing them through it is
# pointless and risks the dual-auth conflict). Default ~/.claude config = Max login.
$sonnetCmd = $cdRepo + ' && claude --model "sonnet" --dangerously-skip-permissions'   # NOT sonnet[1m] - the 1m beta throws an API error on this account
$workerCmd = $cdRepo + ' && claude --model "opus[1m]" --dangerously-skip-permissions'   # worker Opus (dispatched)
$orchCmd   = $cdRepo + ' && claude --model "opus[1m]" --dangerously-skip-permissions'   # orchestrator Opus (you type here)
# Stage-C permuter: continuous annealer swarm in WSL (native mwcceppc). Auto-refills
# from the closest winnable near-misses; writes .omc/permuter_state.json -> dashboard
# quantum panel. Runs under WSL because mwcc needs the Linux toolchain via interop.
$repoWsl   = '/mnt/' + $repo.Substring(0,1).ToLower() + ($repo.Substring(2) -replace '\\','/')
$permCmd   = 'wsl.exe bash -lc "bash ' + $repoWsl + '/tools/decomp_work/permuter/anneal_supervisor.sh"'

if ($DryRun) {
  $proxyCmd  = 'echo [DRYRUN] pane0 = NODE PROXY'
  $codexCmd  = 'echo [DRYRUN] pane = CODEX'
  $glmCmd    = 'echo [DRYRUN] pane5 = CLAUDE GLM glm-5.2[1m]'
  $sonnetCmd = 'echo [DRYRUN] pane6 = CLAUDE SONNET'
  $workerCmd = 'echo [DRYRUN] pane7 = CLAUDE OPUS worker'
  $orchCmd   = 'echo [DRYRUN] pane8 = CLAUDE OPUS orchestrator'
  $permCmd   = 'echo [DRYRUN] pane9 = STAGE-C PERMUTER (WSL annealer swarm)'
}

# Explicit positioned splits to match the cockpit diagram (NOT tiled). We capture
# each pane id at creation, so the registry is robust to psmux index shuffles.
# Send keys literally (-l) to a SPECIFIC pane id, then Enter.
function Send($target, $cmd) { & $tm send-keys -t $target -l $cmd; & $tm send-keys -t $target "Enter" }
function SplitH($target, $pct) { return ([string](& $tm split-window -h -p $pct -t $target -P -F '#{pane_id}' "cmd")).Trim() }
function SplitV($target, $pct) { return ([string](& $tm split-window -v -p $pct -t $target -P -F '#{pane_id}' "cmd")).Trim() }

# --- (re)build session ---
& $tm kill-session -t $Session 2>$null
& $tm new-session -d -s $Session "cmd"
Start-Sleep -Milliseconds 1300          # psmux server start race
& $tm set -t $Session pane-border-status top  2>$null
& $tm set -t $Session pane-border-format ' #{pane_title} ' 2>$null

# Initial pane becomes col1-top (GLM proxy) after we carve everything off it.
$P0 = ([string](& $tm list-panes -t $Session -F '#{pane_id}' | Select-Object -First 1)).Trim()

# Layout (matches the cockpit diagram):
#   col1: GLM proxy (top) / Stage-C permuter (mid) / GLM agent (below)
#   col2: Opus worker (top) / Sonnet (below)
#   middle: Opus orchestrator (full height)      right: 2x4 Codex (8 lanes: 1-8)
$REST     = SplitH $P0 66      # P0 = left group (~34%), REST = right (~66%)
$RIGHT    = SplitH $REST 62    # REST = middle/orchestrator (~25%), RIGHT = codex group (~41%)
$ORCH     = $REST
$COL2     = SplitH $P0 48      # P0 = col1, COL2 = col2
$BELOW    = SplitV $P0 84      # P0 = GLM proxy (top ~16%), BELOW = rest of col1
$PROXY    = $P0
$GLMAGENT = SplitV $BELOW 55   # BELOW = permuter (top ~45%), GLMAGENT = GLM agent (bottom ~55%)
$PERMUTER = $BELOW
$SONNET   = SplitV $COL2 52    # COL2 = Opus worker (top), SONNET = Sonnet (bottom)
$OPUS     = $COL2
$CODEXBL  = SplitV $RIGHT 50   # RIGHT = top row, CODEXBL = bottom row
$CODEX2   = SplitH $RIGHT 50   # RIGHT = codex1 (TL), CODEX2 = codex2 (TR)
$CODEX4   = SplitH $CODEXBL 50 # CODEXBL = codex3 (BL), CODEX4 = codex4 (BR)
$CODEX1   = $RIGHT
$CODEX3   = $CODEXBL
# 4 more Codex lanes (CODEX5-8): split each of the 2x2 cells -> 2x4 Codex grid (8 lanes).
$CODEX5   = SplitV $CODEX1 50
$CODEX6   = SplitV $CODEX2 50
$CODEX7   = SplitV $CODEX3 50
$CODEX8   = SplitV $CODEX4 50
Start-Sleep -Milliseconds 400

# Launch each role in its captured pane.
Send $PROXY    $proxyCmd
Send $GLMAGENT $glmCmd
Send $OPUS     $workerCmd
Send $SONNET   $sonnetCmd
Send $ORCH     $orchCmd
Send $CODEX1   $codexCmd
Send $CODEX2   $codexCmd
Send $CODEX3   $codexCmd
Send $CODEX4   $codexCmd
Send $CODEX5   $codexCmd
Send $CODEX6   $codexCmd
Send $CODEX7   $codexCmd
Send $CODEX8   $codexCmd
Send $PERMUTER $permCmd

# --- registry from CAPTURED ids (robust to layout/index shuffles) ---
if (-not $DryRun) {
  $reg = Join-Path $repo 'tools\decomp_work\tmux_control\panes.env'
  $regBody = @(
    '# panes.env - decomp cockpit registry (written by launch-decomp.ps1 from captured pane ids).',
    '# claude=orchestrator Opus (self) | worker=Opus | sonnet=Sonnet | glm=GLM | codex/codex2..8=Codex (8 lanes)',
    ('CLAUDE_PANE="'  + $ORCH     + '"'),
    ('WORKER_PANE="'  + $OPUS     + '"'),
    ('SONNET_PANE="'  + $SONNET   + '"'),
    ('GLM_PANE="'     + $GLMAGENT + '"'),
    ('CODEX_PANE="'   + $CODEX1   + '"'),
    ('CODEX2_PANE="'  + $CODEX2   + '"'),
    ('CODEX3_PANE="'  + $CODEX3   + '"'),
    ('CODEX4_PANE="'  + $CODEX4   + '"'),
    ('CODEX5_PANE="'  + $CODEX5   + '"'),
    ('CODEX6_PANE="'  + $CODEX6   + '"'),
    ('CODEX7_PANE="'  + $CODEX7   + '"'),
    ('CODEX8_PANE="'  + $CODEX8   + '"'),
    ('PROXY_PANE="'   + $PROXY    + '"'),
    ('PERMUTER_PANE="' + $PERMUTER + '"')
  ) -join "`n"
  [System.IO.File]::WriteAllText($reg, $regBody + "`n")
  Write-Host "Wrote control registry: $reg" -ForegroundColor DarkGray
}

# auto-start the decomp fleet driver once the agents have booted. fleet_up.ps1 brings up
# the wedge-proof tmux control (sole-owner pane_io + tmux-free driver); it only dispatches
# to panes that read as idle, so a ~75s delay lets the agent TUIs finish booting first.
if (-not $DryRun) {
  Start-Process powershell -WindowStyle Hidden -ArgumentList @(
    '-NoProfile','-ExecutionPolicy','Bypass','-Command',
    "Start-Sleep 75; & '$repo\tools\decomp_work\fleet_up.ps1'"
  )
  Write-Host "Scheduled fleet_up (decomp driver) to start in ~75s" -ForegroundColor Cyan
}

# land the user in the orchestrator pane (captured id)
& $tm select-pane -t $ORCH 2>$null

Write-Host ""
Write-Host "decomp cockpit built - diagram layout (col1 glm-proxy/permuter/glm-agent | col2 opus/sonnet | mid orchestrator | right 2x2 codex):" -ForegroundColor Green
& $tm list-panes -t $Session -F '  pane #{pane_index}: @#{pane_left},#{pane_top} #{pane_id} cmd=#{pane_current_command}'
Write-Host ""

if ($NoAttach) {
  Write-Host "Attach with:  tmux attach -t $Session" -ForegroundColor Cyan
} else {
  & $tm attach -t $Session
}
