
$ErrorActionPreference = "Stop"

# Always run from the project root
$ProjectRoot = Split-Path -Parent $PSScriptRoot
Set-Location $ProjectRoot

$BuildDir = Join-Path $ProjectRoot "build\all-tests"
$CoverageDataDir = Join-Path $BuildDir "coverage-data"

# Check compiler
if (-not (Get-Command g++ -ErrorAction SilentlyContinue)) {
    Write-Host "ERROR: g++ not found. Add the MSYS2 UCRT64 compiler to PATH."
    exit 1
}

# Start with a clean coverage directory
if (Test-Path $BuildDir) {
    Remove-Item $BuildDir -Recurse -Force
}

New-Item -ItemType Directory -Force -Path $CoverageDataDir | Out-Null

# Build the complete application separately
Write-Host "`n========== BUILDING FULL GAME =========="

$appSources = @(
    Get-ChildItem (Join-Path $ProjectRoot "src") -Filter "*.cpp" |
    ForEach-Object { $_.FullName }
)

$appExe = Join-Path $BuildDir "snake_game_console.exe"

& g++ -std=c++17 -Wall -Wextra -pedantic -O0 -g `
    -I "src" -o $appExe @appSources

if ($LASTEXITCODE -ne 0) {
    Write-Host "ERROR: Full application build failed."
    exit 1
}

Write-Host "Full application build: PASS"

# Test names and their required source dependencies
$tests = @(

    @{ Name = "TC_COLL_01"; Sources = @("src\SnakeManager.cpp", "src\CollisionManager.cpp") }
    @{ Name = "TC_COLL_02"; Sources = @("src\SnakeManager.cpp", "src\CollisionManager.cpp") }

    @{ Name = "TC_EXIT_01"; Sources = @("src\InputHandler.cpp", "src\SnakeManager.cpp") }

    @{ Name = "TC_FOOD_01"; Sources = @("src\FoodManager.cpp") }
    @{ Name = "TC_FOOD_02"; Sources = @("src\SnakeManager.cpp", "src\CollisionManager.cpp", "src\FoodManager.cpp") }
    @{ Name = "TC_FOOD_03"; Sources = @("src\FoodManager.cpp") }

    @{ Name = "TC_GAMEOVER_01"; Sources = @("src\GameStateManager.cpp") }
    @{ Name = "TC_GROW_01"; Sources = @("src\SnakeManager.cpp") }

    @{ Name = "TC_INPUT_01"; Sources = @("src\InputHandler.cpp", "src\SnakeManager.cpp") }

    @{ Name = "TC_MAINT_01"; Sources = @("src\SnakeManager.cpp", "src\FoodManager.cpp", "src\CollisionManager.cpp", "src\ScoreManager.cpp") }

    @{ Name = "TC_MOVE_01"; Sources = @("src\SnakeManager.cpp") }
    @{ Name = "TC_MOVE_02"; Sources = @("src\SnakeManager.cpp") }
    @{ Name = "TC_MOVE_03"; Sources = @("src\SnakeManager.cpp") }

    @{ Name = "TC_PAUSE_01"; Sources = @("src\GameStateManager.cpp") }
    @{ Name = "TC_PAUSE_02"; Sources = @("src\GameStateManager.cpp") }

    @{ Name = "TC_PERF_01"; Sources = @("src\InputHandler.cpp", "src\SnakeManager.cpp") }
    @{ Name = "TC_PORT_01"; Sources = @() }

    @{ Name = "TC_REL_01"; Sources = @("src\GameStateManager.cpp") }

    @{ Name = "TC_RESTART_01"; Sources = @("src\GameStateManager.cpp", "src\SnakeManager.cpp", "src\FoodManager.cpp", "src\DifficultyManager.cpp") }

    @{ Name = "TC_SCORE_01"; Sources = @("src\ScoreManager.cpp") }
    @{ Name = "TC_SCORE_02"; Sources = @("src\ScoreManager.cpp", "src\GameRenderer.cpp") }

    @{ Name = "TC_SEC_05"; Sources = @("src\GameStateManager.cpp", "src\SnakeManager.cpp", "src\FoodManager.cpp") }

    @{ Name = "TC_SPEED_01"; Sources = @("src\DifficultyManager.cpp") }

    @{ Name = "TC_START_01"; Sources = @("src\GameStateManager.cpp") }
    @{ Name = "TC_STATE_01"; Sources = @("src\GameStateManager.cpp") }

    @{ Name = "TC_STATE_RENDER_01"; Sources = @("src\GameRenderer.cpp") }
    @{ Name = "TC_UX_01"; Sources = @("src\GameRenderer.cpp") }
)

$total = $tests.Count
$passed = 0
$failed = 0

Write-Host "`n========== RUNNING $total AUTOMATED TESTS =========="

foreach ($test in $tests) {

    Write-Host "`n---------- $($test.Name) ----------"

    $testDir = Join-Path $CoverageDataDir $test.Name
    New-Item -ItemType Directory -Force -Path $testDir | Out-Null

    $testFile = Join-Path $ProjectRoot "tests\$($test.Name).cpp"
    $exe = Join-Path $testDir "$($test.Name).exe"

    $compileFiles = @($testFile)

    foreach ($source in $test.Sources) {
        $compileFiles += Join-Path $ProjectRoot $source
    }

    # Compile with coverage instrumentation
    & g++ -std=c++17 -Wall -Wextra -pedantic -O0 -g --coverage `
        -I "src" -o $exe @compileFiles

    if ($LASTEXITCODE -ne 0) {
        Write-Host "$($test.Name): COMPILE FAILED"
        $failed++
        continue
    }

    # Execute the test
    & $exe

    if ($LASTEXITCODE -eq 0) {
        Write-Host "$($test.Name): PASS"
        $passed++
    }
    else {
        Write-Host "$($test.Name): TEST FAILED"
        $failed++
    }
}

Write-Host "`n======================================"
Write-Host "AUTOMATED TEST RESULTS"
Write-Host "Passed : $passed"
Write-Host "Failed : $failed"
Write-Host "Total  : $total"
Write-Host "======================================"

if ($failed -gt 0) {
    exit 1
}

Write-Host "`nAll automated tests passed."
exit 0
