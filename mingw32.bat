@echo off

set UTIL=c:\util\bin

set GIT=C:\Git\cmd

set COMPILER=C:\mingw32\bin

set TITLE=MinGW32

set PATH=%PATH%;%UTIL%;%GIT%;%COMPILER%

cmd.exe /K title %TITLE%
