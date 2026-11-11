$ErrorActionPreference = "Stop"

# Locate project root.
$RepoRoot = Split-Path -Parent $PSScriptRoot

# Store generated executables in the ignored build directory.
$BuildDir = Join-Path $RepoRoot "build\sprint2-tests"

New-Item -ItemType Directory -Force -Path $BuildDir |
    Out-Null

if (-not (Get-Command g++ -ErrorAction SilentlyContinue)) {
    Write-Host "ERROR: g++ was not found in PATH." -ForegroundColor Red
    exit 1
}

Write-Host "`n====================================="
Write-Host "       SPRINT 2 TEST RUNNER"
Write-Host "=====================================`n"

$passed = 0
$total = 1

try {
    # Build the complete console application.
    Write-Host "[BUILD] Full console application"

    $mainSources = @(
        Get-ChildItem -Path (Join-Path $RepoRoot "src") `
            -Filter "*.cpp" -File |
        Sort-Object Name |
        ForEach-Object { $_.FullName }
    )

    $mainExe = Join-Path $BuildDir "snake_game_console.exe"

    $mainArgs = @(
        "-std=c++17",
        "-Wall",
        "-Wextra",
        "-pedantic",
        "-DCONSOLE_MODE"
    ) + $mainSources + @("-o", $mainExe)

    & g++ @mainArgs

    if ($LASTEXITCODE -ne 0) {
        throw "Main application compilation failed."
    }

    Write-Host "[PASS] Main application build" -ForegroundColor Green
    $passed++

    # Every tests\TC_*.cpp is run. Game logic is linked in; main.cpp and the
    # Windows-only GameController.cpp are excluded.
    $logicSources = @(
        Get-ChildItem -Path (Join-Path $RepoRoot "src") -Filter "*.cpp" -File |
        Where-Object { $_.Name -notin @("main.cpp", "GameController.cpp") } |
        Sort-Object Name |
        ForEach-Object { $_.FullName }
    )

    $tests = @(
        Get-ChildItem -Path (Join-Path $RepoRoot "tests") -Filter "TC_*.cpp" -File |
        Sort-Object Name |
        ForEach-Object {
            [PSCustomObject]@{ Name = $_.BaseName; Sources = $logicSources }
        }
    )
    $total = 1 + $tests.Count

    # Run from the repo root (TC_SEC_04 scans .\src).
    Set-Location $RepoRoot

    foreach ($test in $tests) {
        Write-Host "`n[TEST] $($test.Name)"

        $testSource = Join-Path $RepoRoot "tests\$($test.Name).cpp"

        if (-not (Test-Path $testSource)) {
            throw "Test source not found: $testSource"
        }

        $exePath = Join-Path $BuildDir "$($test.Name).exe"

        $dependencyPaths = @($test.Sources)

        $compileArgs = @(
            "-std=c++17",
            "-Wall",
            "-Wextra",
            "-pedantic"
        ) + @($testSource) + $dependencyPaths +
            @("-o", $exePath)

        & g++ @compileArgs

        if ($LASTEXITCODE -ne 0) {
            throw "$($test.Name) compilation failed."
        }

        & $exePath

        if ($LASTEXITCODE -ne 0) {
            throw "$($test.Name) execution failed."
        }

        Write-Host "[PASS] $($test.Name)" -ForegroundColor Green
        $passed++
    }

    Write-Host "`n====================================="
    Write-Host "Test Results: $passed / $total passed"
    Write-Host "====================================="

    if ($passed -ne $total) {
        exit 1
    }

    exit 0
}
catch {
    Write-Host "`n[FAIL] $($_.Exception.Message)" -ForegroundColor Red
    Write-Host "Sprint 2 test runner stopped."
    exit 1
}