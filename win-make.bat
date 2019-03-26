call "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\Tools\VsDevCmd.bat"

msbuild %~dp0/build/RagiMagick.sln

exit /b
