@echo off

set SOLUTION_DIR=%1
set CONFIGURATION=%2
set OUT_DIR=%3

xcopy /y /s "%SOLUTION_DIR%dev\fmod\lib\*.dll" "%OUT_DIR%"
xcopy /y /s "%SOLUTION_DIR%dev\assimp\lib\*.dll" "%OUT_DIR%"