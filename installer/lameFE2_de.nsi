; lameFE 2.0 NSI script

Name "lameFE 2.0"
OutFile "lameFE2_de.exe"

InstProgressFlags smooth colored
ShowInstDetails show 
ShowUninstDetails show

DetailsButtonText "Details"
CompletedText "lameFE 2.0 Setup wurde erfolgreich abgeschlossen."
MiscButtonText "Zurück" \ 
               "Weiter" \ 
               "Abbrechen" \
               "Schließen" 
InstallButtonText "Installieren"
FileErrortext "Fehler! Konnte Datei $0 nicht schreiben."
LicenseText "Sie müssen dem Lizenztext zustimmen bevor Sie das Programm installieren dürfen." \
             "Ich stimme zu"
LicenseData "license.txt"

InstallDir "$PROGRAMFILES\lameFE"
InstallDirRegKey HKEY_LOCAL_MACHINE "SOFTWARE\Nauta Clarus Software\lameFE 2.0" ""
DirShow show ; (make this hide to not let the user change it)
DirText "Installationsverzeichnis auswählen." \
        "Bitte wählen Sie das Verzeichnis, in das Sie lameFE 2.0 installieren möchten." \
        "Durchsuchen..."



Section "" ; (default section)
SetOutPath "$INSTDIR"
CreateDirectory "$INSTDIR\Output"
CreateDirectory "$INSTDIR\Plugins"
File "..\lameFE___Win32_Release_German\lameFE.exe"
File "..\lameFE___Win32_Release_German\CDRip.dll"
; File "..\lameFE___Win32_Release_German\dsiPlayer.dll"
File "..\lameFE___Win32_Release_German\lame_enc.dll"
File "..\lameFE___Win32_Release_German\tagWriter.dll"
File "..\hlp\lameFE.chm"
File "lameFE.ini"
File "cddb.cfg"
File "license.txt"
File "liesmich.txt"
WriteRegStr HKEY_LOCAL_MACHINE "SOFTWARE\Nauta Clarus Software\lameFE 2.0" "" "$INSTDIR"
WriteRegStr HKEY_LOCAL_MACHINE "Software\Microsoft\Windows\CurrentVersion\Uninstall\lameFE 2.0" "DisplayName" "lameFE 2.0 (Nur Deinstalation)"
WriteRegStr HKEY_LOCAL_MACHINE "Software\Microsoft\Windows\CurrentVersion\Uninstall\lameFE 2.0" "UninstallString" '"$INSTDIR\uninst.exe"'
WriteUninstaller "$INSTDIR\uninst.exe"

SectionEnd ; end of default section


Section "Starmenu"
SetOutPath $INSTDIR
CreateDirectory "$SMPROGRAMS\lameFE 2.0"
CreateShortCut "$SMPROGRAMS\lameFE 2.0\lameFE 2.0.lnk" \
                 "$INSTDIR\lameFE.exe"
CreateShortCut "$SMPROGRAMS\lameFE 2.0\lameFE Deinstallieren.lnk" \
               "$INSTDIR\uninst.exe"
SectionEnd ;

Function .onInstSuccess
  WriteINIStr "$INSTDIR\lameFE.ini" \
               "setup" \
               "output"  \
               "$INSTDIR\Output"

  WriteINIStr "$INSTDIR\lameFE.ini" \
              "setup" \
              "plugindir" \
	      "$INSTDIR\Plugins"              

  MessageBox MB_YESNO|MB_ICONQUESTION \
             "Die Installation wurde erfolgreich beendet. Möchten Sie die Datei liesmich.txt jetzt lesen?" \
             IDNO NoReadme
    ExecShell open '$INSTDIR\liesmich.txt'
  NoReadme:
FunctionEnd


; begin uninstall settings/section
UninstallText "Dieses Programm entfernt lameFE 2.0 von Ihrem System."
UninstallButtonText "Deinstallieren"
ShowUninstDetails show
UninstallCaption "lameFE 2.0 Deinstalltion"

Section Uninstall
; add delete commands to delete whatever files/registry keys/etc you installed here.
Delete "$INSTDIR\lameFE.exe"
Delete "$INSTDIR\tagWriter.dll"
Delete "$INSTDIR\CDRip.dll"
Delete "$INSTDIR\lame_enc.dll"
Delete "$INSTDIR\liesmich.txt"
Delete "$INSTDIR\lameFE.log"
Delete "$INSTDIR\license.txt"
Delete "$INSTDIR\lameFE.ini"
Delete "$INSTDIR\cddb.cfg"
Delete "$INSTDIR\lameFE.chm"
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
