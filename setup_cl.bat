@echo off
for /f "delims=" %%i in ('dir /b /s "C:\Program Files (x86)\Microsoft Visual Studio" ^| findstr "\\bin\\Hostx64\\x64\\cl.exe"') do (
    set CL_PATH=%%i
    echo Found cl.exe: !CL_PATH!
    set "PATH=!PATH!;%%~dpi"
    goto :found
)

:found
if defined CL_PATH (
    echo cl.exe added to PATH
    echo You can now use 'cl' command
) else (
    echo cl.exe not found
)