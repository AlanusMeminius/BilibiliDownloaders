setlocal enabledelayedexpansion
set filename=%~dp0src/AppVersion.h
set tempfile=temp_version.h
set temp_hash=temp_hash.txt

type %filename%

echo "get hash"
(git rev-parse HEAD)>%temp_hash%

echo "deal file"
start wscript "version.vbs"

echo "copy"
xcopy %~dp0%tempfile% %filename%

echo "delete temp file"
del %temp_hash%
del %~dp0%tempfile%
pause
