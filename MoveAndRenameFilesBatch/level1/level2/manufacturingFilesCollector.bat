@echo off 
Rem This Collects and Renames Files
set messageStart=******************* Program Started *******************
echo %messageStart%
set /A fileExist = 1

if not exist ManufacturingFiles\ (
set /A fileExist = 0
set /P pCodeAssembly="Enter Assembly name (For example P100001): "
set /P pCodePWB="Enter PWB name (For example P100002): "
mkdir ManufacturingFiles

Rem actual work copy and rename
copy %~dp0\..\folderInLevel1\fileInFolderInLevel1.txt %~dp0\ManufacturingFiles\"%pCodeAssembly%".txt




echo *************************************************************************
echo ********************** Manufacturing files created **********************
echo *************************************************************************
Rem Add Log output also
Rem copy NUL %~dp0\..\folderInLevel1\EmptyFile.txt
)

if %fileExist%==1 (
echo **************************************************************************  
echo ********************** File exists, do nothing here **********************
echo **************************************************************************
Rem Add Log output also
Rem copy NUL %~dp0\..\folderInLevel1\EmptyFile.txt
)