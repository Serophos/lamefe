; lameFE 2.0 NSI script

Name "lameFE 2.0"
OutFile "lameFE2_en.exe"

InstProgressFlags smooth colored
ShowInstDetails show 
ShowUninstDetails show

LicenseText "You must agree to this license before installing."
LicenseData "license.txt"

InstallDir "$PROGRAMFILES\lameFE"
InstallDirRegKey HKEY_LOCAL_MACHINE "SOFTWARE\Nauta Clarus Software\lameFE 2.0" ""
DirShow show ; (make this hide to not let the user change it)
DirText "Select the directory to install lameFE 2.0 in:"

Section "" ; (default section)
SetOutPath "$INSTDIR"
CreateDirectory "$INSTDIR\Output"
CreateDirectory "$INSTDIR\Plugins"
File "..\Release\lameFE.exe"
File "..\Release\CDRip.dll"
; File "..\Release\dsiPlayer.dll"
File "..\Release\lame_enc.dll"
File "..\Release\tagWriter.dll"
File "..\hlp\lameFE.chm"
File "lameFE.ini"
File "cddb.cfg"
File "license.txt"
File "readme.txt"
WriteRegStr HKEY_LOCAL_MACHINE "SOFTWARE\Nauta Clarus Software\lameFE 2.0" "" "$INSTDIR"
WriteRegStr HKEY_LOCAL_MACHINE "Software\Microsoft\Windows\CurrentVersion\Uninstall\lameFE 2.0" "DisplayName" "lameFE 2.0 (remove only)"
WriteRegStr HKEY_LOCAL_MACHINE "Software\Microsoft\Windows\CurrentVersion\Uninstall\lameFE 2.0" "UninstallString" '"$INSTDIR\uninst.exe"'
; write out uninstaller
WriteUninstaller "$INSTDIR\uninst.exe"

SectionEnd ; end of default section


Section "Starmenu"
SetOutPath $INSTDIR
CreateDirectory "$SMPROGRAMS\lameFE 2.0"
CreateShortCut "$SMPROGRAMS\lameFE 2.0\lameFE 2.0.lnk" \
                 "$INSTDIR\lameFE.exe"
CreateShortCut "$SMPROGRAMS\lameFE 2.0\Uninstall lameFE.lnk" \
               "$INSTDIR\uninst.exe"
SectionEnd ;

Function .onInstSuccess

;Successfully installed now write ini strings to lameFE ini

 
  WriteINIStr "$INSTDIR\lameFE.ini" \
               "setup" \
               "output"  \
                "$INSTDIR\Output"

  WriteINIStr "$INSTDIR\lameFE.ini" \
              "setup" \
              "plugindir" \
	      "$INSTDIR\Plugins"              

; Readme?
  MessageBox MB_YESNO|MB_ICONQUESTION \
             "Setup has completed. View readme file now?" \
             IDNO NoReadme
    ExecShell open '$INSTDIR\readme.txt'
  NoReadme:
FunctionEnd


; begin uninstall settings/section
UninstallText "This will uninstall lameFE 2.0 from your system"

Section Uninstall
; add delete commands to delete whatever files/registry keys/etc you installed here.
Delete "$INSTDIR\lameFE.exe"
Delete "$INSTDIR\tagWriter.dll"
Delete "$INSTDIR\CDRip.dll"
Delete "$INSTDIR\lame_enc.dll"
Delete "$INSTDIR\lameFE.log"
Delete "$INSTDIR\license.txt"
Delete "$INSTDIR\readme.txt"
Delete "$INSTDIR\lameFE.ini"
Delete "$INSTDIR\lameFE.chm"
Delete "$INSTDIR\cddb.cfg"
Delete "$SMPROGRAMS\lameFE 2.0\*.lnk"
Delete "$INSTDIR\uninst.exe"
DeleteRegKey HKEY_LOCAL_MACHINE "SOFTWARE\Nauta Clarus Software\lameFE 2.0"
DeleteRegKey HKEY_LOCAL_MACHINE "SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\lameFE 2.0"
RMDir "$INSTDIR\Output"
RMDir "$INSTDIR\Plugins" 
RMDir "$SMPROGRAMS\lameFE 2.0"
RMDir "$INSTDIR"
SectionEnd ; end of uninstall section

; eof
