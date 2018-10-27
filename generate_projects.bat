@echo off
set build_system=vs2013

echo Generating projects for %build_system%...
.\\premake5_windows.exe %build_system%

pause