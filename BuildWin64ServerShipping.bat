@echo off
REM ============================================================
REM  Xuanming - Build & Cook Win64 Dedicated Server (Shipping)
REM  打包 Win64 DS，Shipping 配置，可用于上线
REM ============================================================
setlocal

set ENGINE_ROOT=F:\work\UnrealEngine
set PROJECT_FILE=%~dp0Xuanming.uproject
set OUTPUT_DIR=%~dp0Build\WindowsServer
set CONFIG=Shipping

echo [Xuanming] Building Win64 Dedicated Server (%CONFIG%)...
echo [Xuanming] Output: %OUTPUT_DIR%
echo.

call "%ENGINE_ROOT%\Engine\Build\BatchFiles\RunUAT.bat" ^
    BuildCookRun ^
    -project="%PROJECT_FILE%" ^
    -noP4 ^
    -platform=Win64 ^
    -serverplatform=Win64 ^
    -serverconfig=%CONFIG% ^
    -cook ^
    -allmaps ^
    -build ^
    -stage ^
    -pak ^
    -archive ^
    -archivedirectory="%OUTPUT_DIR%" ^
    -server ^
    -noclient ^
    -utf8output

if errorlevel 1 (
    echo [Xuanming] ERROR: Win64 server build failed.
    pause
    exit /b 1
)

echo.
echo [Xuanming] Win64 Server build complete.
echo Output: %OUTPUT_DIR%\WindowsServer\
echo Run:    XuanmingServer.exe -log -port=7777
pause
