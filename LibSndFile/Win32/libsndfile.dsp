# Microsoft Developer Studio Project File - Name="libsndfile" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=libsndfile - Win32 Debug UNICODE
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "libsndfile.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "libsndfile.mak" CFG="libsndfile - Win32 Debug UNICODE"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "libsndfile - Win32 Release" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE "libsndfile - Win32 Debug" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE "libsndfile - Win32 Debug UNICODE" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE "libsndfile - Win32 Release UNICODE" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "libsndfile - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LIBSNDFILE_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\win32" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 /nologo /dll /machine:I386

!ELSEIF  "$(CFG)" == "libsndfile - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "LIBSNDFILE_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\win32" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /dll /map /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "libsndfile - Win32 Debug UNICODE"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "libsndfile___Win32_Debug_UNICODE"
# PROP BASE Intermediate_Dir "libsndfile___Win32_Debug_UNICODE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug_UNICODE"
# PROP Intermediate_Dir "Debug_UNICODE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\win32" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\win32" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_UNICODE" /D "UNICODE" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /map /debug /machine:I386 /pdbtype:sept
# SUBTRACT BASE LINK32 /pdb:none
# ADD LINK32 /nologo /dll /map /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "libsndfile - Win32 Release UNICODE"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "libsndfile___Win32_Release_UNICODE"
# PROP BASE Intermediate_Dir "libsndfile___Win32_Release_UNICODE"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release_UNICODE"
# PROP Intermediate_Dir "Release_UNICODE"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /I "..\win32" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\win32" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "_UNICODE" /D "UNICODE" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /machine:I386
# ADD LINK32 /nologo /dll /machine:I386

!ENDIF 

# Begin Target

# Name "libsndfile - Win32 Release"
# Name "libsndfile - Win32 Debug"
# Name "libsndfile - Win32 Debug UNICODE"
# Name "libsndfile - Win32 Release UNICODE"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\src\GSM610\add.c
# End Source File
# Begin Source File

SOURCE=..\src\aiff.c
# End Source File
# Begin Source File

SOURCE=..\src\alaw.c
# End Source File
# Begin Source File

SOURCE=..\src\au.c
# End Source File
# Begin Source File

SOURCE=..\src\au_g72x.c
# End Source File
# Begin Source File

SOURCE=..\src\GSM610\code.c
# End Source File
# Begin Source File

SOURCE=..\src\command.c
# End Source File
# Begin Source File

SOURCE=..\src\common.c
# End Source File
# Begin Source File

SOURCE=..\src\GSM610\decode.c
# End Source File
# Begin Source File

SOURCE=..\src\double64.c
# End Source File
# Begin Source File

SOURCE=..\src\dwd.c
# End Source File
# Begin Source File

SOURCE=..\src\dwvw.c
# End Source File
# Begin Source File

SOURCE=..\src\file_io.c
# End Source File
# Begin Source File

SOURCE=..\src\float32.c
# End Source File
# Begin Source File

SOURCE=..\src\G72x\g721.c
# End Source File
# Begin Source File

SOURCE=..\src\G72x\g723_16.c
# End Source File
# Begin Source File

SOURCE=..\src\G72x\g723_24.c
# End Source File
# Begin Source File

SOURCE=..\src\G72x\g723_40.c
# End Source File
# Begin Source File

SOURCE=..\src\G72x\g72x.c
# End Source File
# Begin Source File

SOURCE=..\src\G72x\g72x_test.c
# End Source File
# Begin Source File

SOURCE=..\src\gsm610.c
# End Source File
# Begin Source File

SOURCE=..\src\GSM610\gsm_create.c
# End Source File
# Begin Source File

SOURCE=..\src\GSM610\gsm_decode.c
# End Source File
# Begin Source File

SOURCE=..\src\GSM610\gsm_destroy.c
# End Source File
# Begin Source File

SOURCE=..\src\GSM610\gsm_encode.c
# End Source File
# Begin Source File

SOURCE=..\src\GSM610\gsm_option.c
# End Source File
# Begin Source File

SOURCE=..\src\ima_adpcm.c
# End Source File
# Begin Source File

SOURCE=..\src\ircam.c
# End Source File
# Begin Source File

SOURCE=.\libsndfile.def
# End Source File
# Begin Source File

SOURCE=.\libsndfile.rc
# End Source File
# Begin Source File

SOURCE=..\src\GSM610\long_term.c
# End Source File
# Begin Source File

SOURCE=..\src\GSM610\lpc.c
# End Source File
# Begin Source File

SOURCE=..\src\ms_adpcm.c
# End Source File
# Begin Source File

SOURCE=..\src\nist.c
# End Source File
# Begin Source File

SOURCE=..\src\paf.c
# End Source File
# Begin Source File

SOURCE=..\src\pcm.c
# End Source File
# Begin Source File

SOURCE=..\src\GSM610\preprocess.c
# End Source File
# Begin Source File

SOURCE=..\src\raw.c
# End Source File
# Begin Source File

SOURCE=..\src\GSM610\rpe.c
# End Source File
# Begin Source File

SOURCE=..\src\rx2.c
# End Source File
# Begin Source File

SOURCE=..\src\sd2.c
# End Source File
# Begin Source File

SOURCE=..\src\sds.c
# End Source File
# Begin Source File

SOURCE=..\src\sfendian.c
# End Source File
# Begin Source File

SOURCE=..\src\GSM610\short_term.c
# End Source File
# Begin Source File

SOURCE=..\src\sndfile.c
# End Source File
# Begin Source File

SOURCE=..\src\svx.c
# End Source File
# Begin Source File

SOURCE=..\src\GSM610\table.c
# End Source File
# Begin Source File

SOURCE=..\src\txw.c
# End Source File
# Begin Source File

SOURCE=..\src\ulaw.c
# End Source File
# Begin Source File

SOURCE=..\src\voc.c
# End Source File
# Begin Source File

SOURCE=..\src\w64.c
# End Source File
# Begin Source File

SOURCE=..\src\wav.c
# End Source File
# Begin Source File

SOURCE=..\src\wav_w64.c
# End Source File
# Begin Source File

SOURCE=..\src\wve.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\src\au.h
# End Source File
# Begin Source File

SOURCE=..\src\common.h
# End Source File
# Begin Source File

SOURCE=..\src\GSM610\config.h
# End Source File
# Begin Source File

SOURCE=.\config.h
# End Source File
# Begin Source File

SOURCE=..\src\float_cast.h
# End Source File
# Begin Source File

SOURCE=..\src\G72x\g72x.h
# End Source File
# Begin Source File

SOURCE=..\src\G72x\g72x_priv.h
# End Source File
# Begin Source File

SOURCE=..\src\GSM610\gsm.h
# End Source File
# Begin Source File

SOURCE=..\src\GSM610\gsm610_priv.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# Begin Source File

SOURCE=..\src\sfendian.h
# End Source File
# Begin Source File

SOURCE=..\src\sndfile.h
# End Source File
# Begin Source File

SOURCE=.\unistd.h
# End Source File
# Begin Source File

SOURCE=..\src\wav_w64.h
# End Source File
# End Group
# End Target
# End Project
