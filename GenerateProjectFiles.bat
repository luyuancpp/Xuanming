@echo off
chcp 65001 > nul
REM ============================================================
REM  Xuanming - Generate Visual Studio Project Files
REM ============================================================
setlocal

set ENGINE_ROOT=F:\work\UnrealEngine
set PROJECT_FILE=%~dp0Xuanming.uproject

echo [Xuanming] Generating Visual Studio project files...
"%ENGINE_ROOT%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.exe" -projectfiles -project="%PROJECT_FILE%" -game -rocket -progress

if errorlevel 1 (
    echo [Xuanming] ERROR: Failed to generate project files.
    pause
    exit /b 1
)

echo [Xuanming] Done. Open Xuanming.sln in Visual Studio.
pause
