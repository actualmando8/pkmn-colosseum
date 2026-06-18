<#
  launch-decomp.ps1 — build the "decomp" tmux (psmux) cockpit, 9 panes:

     +-----------+-----------+-----------+-----------+
     | 0 PROXY   | 1 CODEX-1 | 2 CODEX-2 | 3 CODEX-3 |
     +-----------+-----------+-----------+-----------+
     | 4 CODEX-4 | 5 GLM     | 6 SONNET  | 7 OPUS wk |
     +-----------+-----------+-----------+-----------+
     | 8 OPUS ORCHESTRATOR (you type here)          |
     +----------------------------------------------+

  Fleet (codex-heavy — we have far more Codex headroom than Claude):
  - pane 0  proxy    — local router (tools/llm-proxy/proxy.js); only GLM routes through it.
  - panes 1-4 codex  — 4x Codex CLI (gpt-5.5). The workhorses.
  - pane 5  glm      — Claude Code via the proxy, pinned glm-5.2[1m] (isolated config + key).
  - pane 6  sonnet   — Claude Code Sonnet (DIRECT; Max bypasses the proxy — see note).
  - pane 7  worker   — a 2nd Opus the orchestrator dispatches hard structural work to.
  - pane 8  orch.    — the orchestrator Opus; the pane you're dropped into on attach.

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
# warns "Both claude.ai and ANTHROPIC_API_KEY set · auth may not work" then throws
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
$sonnetCmd = $cdRepo + ' && claude --model "sonnet[1m]" --dangerously-skip-permissions'
$workerCmd = $cdRepo + ' && claude --model "opus[1m]" --dangerously-skip-permissions'   # worker Opus (dispatched)
$orchCmd   = $cdRepo + ' && claude --model "opus[1m]" --dangerously-skip-permissions'   # orchestrator Opus (you type here)

if ($DryRun) {
  $proxyCmd  = 'echo [DRYRUN] pane0 = NODE PROXY'
  $codexCmd  = 'echo [DRYRUN] pane = CODEX'
  $glmCmd    = 'echo [DRYRUN] pane5 = CLAUDE GLM glm-5.2[1m]'
  $sonnetCmd = 'echo [DRYRUN] pane6 = CLAUDE SONNET'
  $workerCmd = 'echo [DRYRUN] pane7 = CLAUDE OPUS worker'
  $orchCmd   = 'echo [DRYRUN] pane8 = CLAUDE OPUS orchestrator'
}

# psmux ignores custom pane titles, so panes are identified by index:
#   0 = PROXY  1-4 = CODEX x4  5 = GLM  6 = SONNET  7 = OPUS worker  8 = OPUS orchestrator
# Send keys literally (-l) so '&&', quotes and [1m] are typed verbatim, then Enter.
function Send($cmd) { & $tm send-keys -t $Session -l $cmd; & $tm send-keys -t $Session "Enter" }

# --- (re)build session ---
& $tm kill-session -t $Session 2>$null
& $tm new-session -d -s $Session "cmd"
Start-Sleep -Milliseconds 1300          # psmux server start race
& $tm set -t $Session pane-border-status top  2>$null
& $tm set -t $Session pane-border-format ' pane #{pane_index} ' 2>$null

# pane 0: proxy (initial pane)
Send $proxyCmd
Start-Sleep -Milliseconds 700

# panes 1-8: split + re-tile after each (survives small terminals). Each new pane is
# active, so Send (which targets the active pane) reaches the just-created pane.
foreach ($step in @(
    @{ cmd = $codexCmd  },   # pane 1: codex-1
    @{ cmd = $codexCmd  },   # pane 2: codex-2
    @{ cmd = $codexCmd  },   # pane 3: codex-3
    @{ cmd = $codexCmd  },   # pane 4: codex-4
    @{ cmd = $glmCmd    },   # pane 5: glm
    @{ cmd = $sonnetCmd },   # pane 6: sonnet
    @{ cmd = $workerCmd },   # pane 7: worker opus
    @{ cmd = $orchCmd   }    # pane 8: orchestrator opus
  )) {
  & $tm split-window -h -t $Session "cmd" 2>$null
  Start-Sleep -Milliseconds 600
  Send $step.cmd
  & $tm select-layout -t $Session tiled 2>$null
}

# --- write the control-lib pane registry (deterministic: we know each role) ---
if (-not $DryRun) {
  $ids = @{}
  foreach ($line in (& $tm list-panes -t $Session -F '#{pane_index} #{pane_id}')) {
    $p = $line.Trim().Split(' '); if ($p.Count -eq 2) { $ids[$p[0]] = $p[1] }
  }
  $reg = Join-Path $repo 'tools\decomp_work\tmux_control\panes.env'
  $regBody = @(
    '# panes.env - decomp cockpit registry (written by launch-decomp.ps1).',
    '# claude=orchestrator Opus (self) | worker=Opus | sonnet=Sonnet | glm=GLM | codex/codex2..4=Codex',
    ('CLAUDE_PANE="'  + $ids['8'] + '"'),
    ('WORKER_PANE="'  + $ids['7'] + '"'),
    ('SONNET_PANE="'  + $ids['6'] + '"'),
    ('GLM_PANE="'     + $ids['5'] + '"'),
    ('CODEX_PANE="'   + $ids['1'] + '"'),
    ('CODEX2_PANE="'  + $ids['2'] + '"'),
    ('CODEX3_PANE="'  + $ids['3'] + '"'),
    ('CODEX4_PANE="'  + $ids['4'] + '"'),
    ('PROXY_PANE="'   + $ids['0'] + '"')
  ) -join "`n"
  [System.IO.File]::WriteAllText($reg, $regBody + "`n")
  Write-Host "Wrote control registry: $reg" -ForegroundColor DarkGray
}

# land the user in the orchestrator pane (index 8)
& $tm select-pane -t ($Session + ":0.8") 2>$null

Write-Host ""
Write-Host "decomp cockpit built (0=proxy 1-4=codex 5=glm 6=sonnet 7=opus-worker 8=opus-orchestrator):" -ForegroundColor Green
& $tm list-panes -t $Session -F '  pane #{pane_index}: @#{pane_left},#{pane_top} #{pane_id} cmd=#{pane_current_command}'
Write-Host ""

if ($NoAttach) {
  Write-Host "Attach with:  tmux attach -t $Session" -ForegroundColor Cyan
} else {
  & $tm attach -t $Session
}
