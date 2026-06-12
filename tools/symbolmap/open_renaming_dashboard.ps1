param(
    [int]$Port = 8788,
    [switch]$Fresh,
    [switch]$NoOpen
)

$ErrorActionPreference = "Stop"
$Root = Resolve-Path (Join-Path $PSScriptRoot "..\..")

function Get-DashboardUrl {
    param([int]$CandidatePort)
    return "http://127.0.0.1:$CandidatePort/"
}

function Test-Dashboard {
    param([int]$CandidatePort)
    $HealthUrl = "$(Get-DashboardUrl $CandidatePort)api/health"
    try {
        $Response = Invoke-WebRequest -UseBasicParsing -Uri $HealthUrl -TimeoutSec 1
        return $Response.StatusCode -eq 200
    } catch {
        return $false
    }
}

$CandidatePorts = @($Port)
foreach ($Candidate in 8788..8798) {
    if ($Candidate -notin $CandidatePorts) {
        $CandidatePorts += $Candidate
    }
}

$SelectedPort = $null
foreach ($CandidatePort in $CandidatePorts) {
    if ((-not $Fresh) -and (Test-Dashboard $CandidatePort)) {
        $SelectedPort = $CandidatePort
        break
    }

    if ($Fresh -and (Test-Dashboard $CandidatePort)) {
        continue
    }

    Start-Process `
        -FilePath "python" `
        -ArgumentList @("tools\symbolmap\renaming_dashboard.py", "--port", "$CandidatePort") `
        -WorkingDirectory $Root `
        -WindowStyle Hidden | Out-Null

    $Ready = $false
    for ($i = 0; $i -lt 20; $i++) {
        Start-Sleep -Milliseconds 250
        if (Test-Dashboard $CandidatePort) {
            $Ready = $true
            break
        }
    }
    if ($Ready) {
        $SelectedPort = $CandidatePort
        break
    }
}

if ($null -eq $SelectedPort) {
    throw "Renaming dashboard did not become ready on ports $($CandidatePorts -join ', ')"
}

$Url = Get-DashboardUrl $SelectedPort
if (-not $NoOpen) {
    Start-Process $Url | Out-Null
}

Write-Output "Renaming dashboard: $Url"
