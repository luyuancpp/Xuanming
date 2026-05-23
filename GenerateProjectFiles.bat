@echo off
chcp 65001 > nul
REM ============================================================
REM  Xuanming - Generate Visual Studio Project Files
REM  Uses freshly built UBT (Source/.../bin/Development) instead of
REM  the prebuilt Binaries one, which may be stale on .NET version.
REM ============================================================
setlocal

set ENGINE_ROOT=F:\work\UnrealEngine
set PROJECT_FILE=%~dp0Xuanming.uproject
set UBT_DLL_FRESH=%ENGINE_ROOT%\Engine\Source\Programs\UnrealBuildTool\bin\Development\UnrealBuildTool.dll
set UBT_DLL_PREBUILT=%ENGINE_ROOT%\Engine\Binaries\DotNET\UnrealBuildTool\UnrealBuildTool.dll

REM Prefer freshly built UBT (newer .NET, supports C# 12 collection expressions)
if exist "%UBT_DLL_FRESH%" (
    set UBT_DLL=%UBT_DLL_FRESH%
) else (
    set UBT_DLL=%UBT_DLL_PREBUILT%
)

echo [Xuanming] Using UBT: %UBT_DLL%
echo [Xuanming] Generating Visual Studio project files...
dotnet "%UBT_DLL%" -projectfiles -project="%PROJECT_FILE%" -game -rocket -progress

if errorlevel 1 (
    echo [Xuanming] ERROR: Failed to generate project files.
    pause
    exit /b 1
)

echo [Xuanming] Done. Open Xuanming.sln in Visual Studio.
pause
