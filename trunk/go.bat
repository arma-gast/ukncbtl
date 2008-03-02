@echo off

SET MSBUILD_EXE=C:\WINDOWS\Microsoft.NET\Framework\v2.0.50727\MSBuild.exe

if "%1"=="" goto show_help

%MSBUILD_EXE% /nologo /target:%1 Makefile.msbuild
goto exit

:show_help
echo Usage: go [targets]
echo Targets:
echo     build       Build all
echo     clean       Cleanup all

:exit
