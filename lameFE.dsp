# Microsoft Developer Studio Project File - Name="lameFE" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=lameFE - Win32 Debug
!MESSAGE Dies ist kein g�ltiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und f�hren Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "lameFE.mak".
!MESSAGE 
!MESSAGE Sie k�nnen beim Ausf�hren von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "lameFE.mak" CFG="lameFE - Win32 Debug"
!MESSAGE 
!MESSAGE F�r die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "lameFE - Win32 Release" (basierend auf  "Win32 (x86) Application")
!MESSAGE "lameFE - Win32 Debug" (basierend auf  "Win32 (x86) Application")
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
# ADD CPP /nologo /MD /W3 /GX /O2 /I "LibSndFile\src" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_ENGLISH" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x407 /d "NDEBUG" /d "_AFXDLL" /d "_ENGLISH"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 winmm.lib htmlhelp.lib id3lib.lib zlib.lib /nologo /subsystem:windows /machine:I386 /libpath:"ID3Lib\libprj\Release" /libpath:"ID3Lib/zlib"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "LibSndFile\src" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "ENGLISH" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x407 /d "_DEBUG" /d "_AFXDLL" /d "_ENGLISH"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib htmlhelp.lib id3libD.lib zlibD.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept /libpath:"ID3Lib\libprj\Debug" /libpath:"ID3Lib/zlib"

!ENDIF 

# Begin Target

# Name "lameFE - Win32 Release"
# Name "lameFE - Win32 Debug"
# Begin Group "Quellcodedateien"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AlbumInfoCtrl.cpp
# End Source File
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

SOURCE=.\CDPlayerIni.cpp
# End Source File
# Begin Source File

SOURCE=.\CDROMPage.cpp
# End Source File
# Begin Source File

SOURCE=.\CDTrack.cpp
# End Source File
# Begin Source File

SOURCE=.\cfgFile.cpp
# End Source File
# Begin Source File

SOURCE=.\CompactDisk.cpp
# End Source File
# Begin Source File

SOURCE=.\CPlugin.cpp
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

SOURCE=.\ExtListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\FilenamePage.cpp
# End Source File
# Begin Source File

SOURCE=.\FolderDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\FreeDBStatusDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ID3Info.cpp
# End Source File
# Begin Source File

SOURCE=.\Ini.cpp
# End Source File
# Begin Source File

SOURCE=.\InternalPlayer.cpp
# End Source File
# Begin Source File

SOURCE=.\lameFE.cpp
# End Source File
# Begin Source File

SOURCE=.\lameFE.rc
# End Source File
# Begin Source File

SOURCE=.\lameFEDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\LameFEPlugin.cpp
# End Source File
# Begin Source File

SOURCE=.\LameFESplash.cpp
# End Source File
# Begin Source File

SOURCE=.\lameFEView.cpp
# End Source File
# Begin Source File

SOURCE=.\LogFile.cpp
# End Source File
# Begin Source File

SOURCE=.\LogginPage.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MainSettingsPage.cpp
# End Source File
# Begin Source File

SOURCE=.\Mfccddb.cpp
# End Source File
# Begin Source File

SOURCE=.\MultimediaFile.cpp
# End Source File
# Begin Source File

SOURCE=.\MyFileDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\OutPlugin.cpp
# End Source File
# Begin Source File

SOURCE=.\PathDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\PlayList.cpp
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

SOURCE=.\AlbumInfoCtrl.h
# End Source File
# Begin Source File

SOURCE=.\BladeMP3EncDLL.h
# End Source File
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

SOURCE=.\CDPlayerIni.h
# End Source File
# Begin Source File

SOURCE=.\CDROMPage.h
# End Source File
# Begin Source File

SOURCE=.\CDTrack.h
# End Source File
# Begin Source File

SOURCE=.\cfgFile.h
# End Source File
# Begin Source File

SOURCE=.\CompactDisk.h
# End Source File
# Begin Source File

SOURCE=.\CPlugin.h
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

SOURCE=.\ExtListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\FilenamePage.h
# End Source File
# Begin Source File

SOURCE=.\FolderDialog.h
# End Source File
# Begin Source File

SOURCE=.\FreeDBStatusDlg.h
# End Source File
# Begin Source File

SOURCE=.\ID3Info.h
# End Source File
# Begin Source File

SOURCE=.\Ini.h
# End Source File
# Begin Source File

SOURCE=.\InternalPlayer.h
# End Source File
# Begin Source File

SOURCE=.\lameFE.h
# End Source File
# Begin Source File

SOURCE=.\lameFEDoc.h
# End Source File
# Begin Source File

SOURCE=.\LameFEPlugin.h
# End Source File
# Begin Source File

SOURCE=.\LameFESplash.h
# End Source File
# Begin Source File

SOURCE=.\lameFEView.h
# End Source File
# Begin Source File

SOURCE=.\LogFile.h
# End Source File
# Begin Source File

SOURCE=.\LogginPage.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MainSettingsPage.h
# End Source File
# Begin Source File

SOURCE=.\Mfccddb.h
# End Source File
# Begin Source File

SOURCE=.\mmfile.h
# End Source File
# Begin Source File

SOURCE=.\MultimediaFile.h
# End Source File
# Begin Source File

SOURCE=.\MyFileDialog.h
# End Source File
# Begin Source File

SOURCE=.\out_plugin.h
# End Source File
# Begin Source File

SOURCE=.\OutPlugin.h
# End Source File
# Begin Source File

SOURCE=.\PathDialog.h
# End Source File
# Begin Source File

SOURCE=.\PlayList.h
# End Source File
# Begin Source File

SOURCE=.\plugin_api.h
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

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\busy_cd.cur
# End Source File
# Begin Source File

SOURCE=.\res\config_i.ico
# End Source File
# Begin Source File

SOURCE=.\res\config_o.ico
# End Source File
# Begin Source File

SOURCE=.\res\default1.bin
# End Source File
# Begin Source File

SOURCE=.\res\eject.ico
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

SOURCE=.\res\lameFE.ico
# End Source File
# Begin Source File

SOURCE=.\res\lameFE.rc2
# End Source File
# Begin Source File

SOURCE=.\res\lameFEDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\list_bk.bmp
# End Source File
# Begin Source File

SOURCE=.\res\log_images.bmp
# End Source File
# Begin Source File

SOURCE=.\res\next.ico
# End Source File
# Begin Source File

SOURCE=.\res\splash.bmp
# End Source File
# Begin Source File

SOURCE=.\res\stop.ico
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar256.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar256_deact.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar_hi.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar_hi_deact.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\res\finished.wav
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
