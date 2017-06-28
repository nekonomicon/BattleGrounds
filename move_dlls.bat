echo off

REM copys the .dlls to the game directory
REM as all developers have their own different game
REM paths, the project files were too hard to share
REM they MUST always be shared however, to keep file
REM addition and removal easier.

set CODEDIR=C:\coding\battlegrounds\bin
set BGGAMEDIR=C:\games\steam\steamapps\ben@bgmod.com\half-life\bg-dev
set RELEASEDIR=C:\bg_patches\bg-dev
set HLDSDIR=C:\games\hl_server\bg-dev

echo on

copy %CODEDIR%\bg.dll     %BGGAMEDIR%\dlls
copy %CODEDIR%\bg.dll     %RELEASEDIR%\dlls
copy %CODEDIR%\bg.dll     %HLDSDIR%\dlls
copy %CODEDIR%\client.dll %BGGAMEDIR%\cl_dlls
copy %CODEDIR%\client.dll %RELEASEDIR%\cl_dlls
copy %CODEDIR%\client.dll %HLDSDIR%\cl_dlls