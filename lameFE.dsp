# Microsoft Developer Studio Project File - Name="lameFE" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=lameFE - Win32 Debug
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "lameFE.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "lameFE.mak" CFG="lameFE - Win32 Debug"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "lameFE - Win32 Release" (basierend auf  "Win32 (x86) Application")
!MESSAGE "lameFE - Win32 Debug" (basierend auf  "Win32 (x86) Application")
!MESSAGE "lameFE - Win32 Release German" (basierend auf  "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "lameFE - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL" /d "_ENGLISH"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 winmm.lib htmlhelp.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "lameFE - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL" /d "_ENGLISH"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib htmlhelp.lib /nologo /subsystem:windows /map /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "lameFE - Win32 Release German"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "lameFE___Win32_Release_German0"
# PROP BASE Intermediate_Dir "lameFE___Win32_Release_German0"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "lameFE___Win32_Release_German0"
# PROP Intermediate_Dir "lameFE___Win32_Release_German0"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /Ob2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Fp"lameFE___Win32_Release_German/lameFE.pch" /Yu"stdafx.h" /Fo"lameFE___Win32_Release_German/" /Fd"lameFE___Win32_Release_German/" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x407 /fo"lameFE___Win32_Release_German/lameFE.res" /d "NDEBUG" /d "_AFXDLL" /d "_GERMAN"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 winmm.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 winmm.lib htmlhelp.lib /nologo /subsystem:windows /pdb:"lameFE___Win32_Release_German/lameFE.pdb" /machine:I386 /out:"lameFE___Win32_Release_German/lameFE.exe"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "lameFE - Win32 Release"
# Name "lameFE - Win32 Debug"
# Name "lameFE - Win32 Release German"
# Begin Group "Quellcodedateien"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\Buffer.cpp
# End Source File
# Begin Source File

SOURCE=.\CDDBPage.cpp
# End Source File
# Begin Source File

SOURCE=.\CDdbQueryDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CDROMPage.cpp
# End Source File
# Begin Source File

SOURCE=.\cfgFile.cpp
# End Source File
# Begin Source File

SOURCE=.\Encoder.cpp
# End Source File
# Begin Source File

SOURCE=.\EncoderFeeder.cpp
# End Source File
# Begin Source File

SOURCE=.\EncoderSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\EncodingStatusDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\FolderDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\ID3Info.cpp
# End Source File
# Begin Source File

SOURCE=.\ID3TagEditor.cpp
# End Source File
# Begin Source File

SOURCE=.\InternalPlayer.cpp
# End Source File
# Begin Source File

SOURCE=".\hlp\LameFE XP.hpj"
# End Source File
# Begin Source File

SOURCE=.\lameFE.cpp
# End Source File
# Begin Source File

SOURCE=.\hlp\LameFE.hpj
# End Source File
# Begin Source File

SOURCE=.\lameFE.rc
# End Source File
# Begin Source File

SOURCE=.\lameFEDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LameFESplash.cpp
# End Source File
# Begin Source File

SOURCE=.\MainSettingsPage.cpp
# End Source File
# Begin Source File

SOURCE=.\cddb\Mfccddb.cpp
# End Source File
# Begin Source File

SOURCE=.\MP3File.cpp
# End Source File
# Begin Source File

SOURCE=.\PluginPage.cpp
# End Source File
# Begin Source File

SOURCE=.\SettingsSheet.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TimeStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\TrayDialog.cpp
# End Source File
# End Group
# Begin Group "Header-Dateien"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\Buffer.h
# End Source File
# Begin Source File

SOURCE=.\CDDBPage.h
# End Source File
# Begin Source File

SOURCE=.\CDdbQueryDlg.h
# End Source File
# Begin Source File

SOURCE=.\CDROMPage.h
# End Source File
# Begin Source File

SOURCE=.\cfgFile.h
# End Source File
# Begin Source File

SOURCE=.\Encoder.h
# End Source File
# Begin Source File

SOURCE=.\EncoderFeeder.h
# End Source File
# Begin Source File

SOURCE=.\EncoderSheet.h
# End Source File
# Begin Source File

SOURCE=.\EncodingStatusDlg.h
# End Source File
# Begin Source File

SOURCE=.\FolderDialog.h
# End Source File
# Begin Source File

SOURCE=.\ID3Info.h
# End Source File
# Begin Source File

SOURCE=.\ID3TagEditor.h
# End Source File
# Begin Source File

SOURCE=.\InternalPlayer.h
# End Source File
# Begin Source File

SOURCE=".\hlp\LameFE XP.hm"
# End Source File
# Begin Source File

SOURCE=.\lameFE.h
# End Source File
# Begin Source File

SOURCE=.\hlp\LameFE.hm
# End Source File
# Begin Source File

SOURCE=.\lameFEDlg.h
# End Source File
# Begin Source File

SOURCE=.\LameFESplash.h
# End Source File
# Begin Source File

SOURCE=.\MainSettingsPage.h
# End Source File
# Begin Source File

SOURCE=.\cddb\Mfccddb.h
# End Source File
# Begin Source File

SOURCE=.\MP3File.h
# End Source File
# Begin Source File

SOURCE=.\PluginPage.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SettingsSheet.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TimeStatus.h
# End Source File
# Begin Source File

SOURCE=.\TrayDialog.h
# End Source File
# End Group
# Begin Group "Ressourcendateien"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\res\cd_mp3.ico
# End Source File
# Begin Source File

SOURCE=.\res\cd_wav.ico
# End Source File
# Begin Source File

SOURCE=.\res\cddb.ico
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\default1.bin
# End Source File
# Begin Source File

SOURCE=.\res\eject.ico
# End Source File
# Begin Source File

SOURCE=.\res\encoder.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon3.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon4.ico
# End Source File
# Begin Source File

SOURCE=.\res\id3.ico
# End Source File
# Begin Source File

SOURCE=.\res\lameFE.ico
# End Source File
# Begin Source File

SOURCE=.\res\lameFE.rc2
# End Source File
# Begin Source File

SOURCE=.\res\next.ico
# End Source File
# Begin Source File

SOURCE=.\res\refresh.ico
# End Source File
# Begin Source File

SOURCE=.\res\start.ico
# End Source File
# Begin Source File

SOURCE=.\res\stop.ico
# End Source File
# End Group
# Begin Source File

SOURCE=.\hlp\AfxDlg.rtf
# End Source File
# Begin Source File

SOURCE=.\hlp\LameFE.cnt
# End Source File
# Begin Source File

SOURCE=.\lameFE___Win32_Release_German\lameFE.exe.manifest
# End Source File
# Begin Source File

SOURCE=.\hlp\lameFE.HLP
# End Source File
# Begin Source File

SOURCE=.\hlp\LameFE.ph
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
