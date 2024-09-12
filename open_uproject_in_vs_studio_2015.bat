
@echo off

cd /d %~dp0

set "vs_studio_2015_path="C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\IDE\devenv.exe""
set "uproject_sln_path=%CD%\UAM_Uproject.sln"

set "command=%vs_studio_2015_path% %uproject_sln_path%"

call %command%

exit /b


