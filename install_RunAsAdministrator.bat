@setlocal enableextensions
@cd /d "%~dp0"

copy /y LibOVRRT32*.dll %windir%\syswow64
copy /y LibOVRRT64*.dll %windir%\system32

pause