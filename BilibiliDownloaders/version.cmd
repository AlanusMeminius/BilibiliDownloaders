setlocal enabledelayedexpansion

cd/d %~dp0

set filename=%~dp0src\AppVersion.h
set tempfile=temp_version.h
set temp_hash=temp_hash.txt

echo "get hash"
(git rev-parse HEAD)>%temp_hash%

echo "deal file"
start /wait wscript "version.vbs"

echo "copy"
copy %~dp0%tempfile% %filename%

echo "delete temp file"
del %temp_hash%
del %~dp0%tempfile%
pause
