
$ErrorActionPreference = "Stop"

$RepoRoot = Split-Path -Parent $PSScriptRoot
$BuildDir = Join-Path $RepoRoot "build\sprint2-tests"
$CoverageDataDir = Join-Path $BuildDir "coverage-data"

New-Item -ItemType Directory -Force -Path $BuildDir | Out-Null
New-Item -ItemType Directory -Force -Path $CoverageDataDir | Out-Null

if (-not (Get-Command g++ -ErrorAction SilentlyContinue)) {
    Write-Host "ERROR: g++ was not found in PATH." -ForegroundColor Red
    exit 1
}

Write-Host "`n====================================="
Write-Host "       SPRINT 2 TEST RUNNER"
Write-Host "=====================================`n"

$passed = 0
$total = 5

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

    # Sprint 2 automated test definitions.
    $tests = @(
        [PSCustomObject]@{
            Name = "TC_PAUSE_01"
            Sources = @("src\GameStateManager.cpp")
        }

        [PSCustomObject]@{
            Name = "TC_PAUSE_02"
            Sources = @("src\GameStateManager.cpp")
        }

        [PSCustomObject]@{
            Name = "TC_RESTART_01"
            Sources = @(
                "src\GameStateManager.cpp",
                "src\SnakeManager.cpp",
                "src\FoodManager.cpp",
                "src\DifficultyManager.cpp"
            )
        }

        [PSCustomObject]@{
            Name = "TC_EXIT_01"
            Sources = @(
                "src\InputHandler.cpp",
                "src\SnakeManager.cpp"
            )
        }
    )

    foreach ($test in $tests) {
        Write-Host "`n[TEST] $($test.Name)"

        $testSource = Join-Path $RepoRoot "tests\$($test.Name).cpp"

        if (-not (Test-Path $testSource)) {
            throw "Test source not found: $testSource"
        }

        # Keep each test's coverage files separate.
        $testBuildDir = Join-Path $CoverageDataDir $test.Name
        New-Item -ItemType Directory -Force -Path $testBuildDir | Out-Null

        $exePath = Join-Path $testBuildDir "$($test.Name).exe"

        $dependencyPaths = @(
            $test.Sources |
            ForEach-Object { Join-Path $RepoRoot $_ }
        )

        $compileArgs = @(
            "-std=c++17",
            "-Wall",
            "-Wextra",
            "-pedantic",
            "-O0",
            "-g",
            "--coverage"
        ) + @($testSource) + $dependencyPaths +
            @("-o", $exePath)

        Push-Location $testBuildDir
        try {
            & g++ @compileArgs

            if ($LASTEXITCODE -ne 0) {
                throw "$($test.Name) compilation failed."
            }

            & $exePath

            if ($LASTEXITCODE -ne 0) {
                throw "$($test.Name) execution failed."
            }
        }
        finally {
            Pop-Location
        }

        Write-Host "[PASS] $($test.Name)" -ForegroundColor Green
        $passed++
    }

    Write-Host "`n====================================="
    Write-Host "Sprint 2 Results: $passed / $total passed"
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
