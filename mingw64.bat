@echo off

set UTIL=c:\util\bin

set GIT=C:\Git\cmd

set COMPILER=C:\mingw64\bin

set TITLE=MinGW64

set PATH=%PATH%;%UTIL%;%GIT%;%COMPILER%

cmd.exe /K title %TITLE%
