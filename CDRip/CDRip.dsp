# Microsoft Developer Studio Project File - Name="CDRip" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=CDRip - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "CDRip.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "CDRip.mak" CFG="CDRip - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "CDRip - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "CDRip - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "CDRip - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "./paranoia" /I "./paranoia/win32" /I "../" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "CDEX_DLL" /D "int16_t short" /FD /c
# SUBTRACT CPP /Z<none> /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 /nologo /subsystem:windows /dll /map /machine:I386
# SUBTRACT LINK32 /nodefaultlib

!ELSEIF  "$(CFG)" == "CDRip - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 1
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "./paranoia" /I "./paranoia/win32" /I "../" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "CDEX_DLL" /D "int16_t short" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /dll /profile /map /debug /machine:I386 /libpath:"./paranoia/debug"
# SUBTRACT LINK32 /nodefaultlib

!ENDIF 

# Begin Target

# Name "CDRip - Win32 Release"
# Name "CDRip - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp"
# Begin Source File

SOURCE=.\Aspi.cpp
# End Source File
# Begin Source File

SOURCE=.\AspiCD.cpp
# End Source File
# Begin Source File

SOURCE=.\AspiDebug.cpp
# End Source File
# Begin Source File

SOURCE=.\CDExtract.cpp
# End Source File
# Begin Source File

SOURCE=.\CDRip.cpp
# End Source File
# Begin Source File

SOURCE=.\CDRip.def
# End Source File
# Begin Source File

SOURCE=.\CDRomSettings.cpp
# End Source File
# Begin Source File

SOURCE=..\FileVersion.cpp
# PROP Exclude_From_Build 1
# End Source File
# Begin Source File

SOURCE=..\Ini.cpp
# End Source File
# Begin Source File

SOURCE=.\NTScsi.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h"
# Begin Source File

SOURCE=.\Aspi.h
# End Source File
# Begin Source File

SOURCE=.\AspiCD.h
# End Source File
# Begin Source File

SOURCE=.\AspiDebug.h
# End Source File
# Begin Source File

SOURCE=.\CDExtract.h
# End Source File
# Begin Source File

SOURCE=.\CDRip.h
# End Source File
# Begin Source File

SOURCE=.\CDRomSettings.h
# End Source File
# Begin Source File

SOURCE=..\Ini.h
# End Source File
# Begin Source File

SOURCE=.\NTScsi.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "rc"
# Begin Source File

SOURCE=.\CDRip.rc
# End Source File
# End Group
# Begin Group "Paranoia"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\interface\cdda_interface.h
# End Source File
# Begin Source File

SOURCE=.\paranoia\cdda_paranoia.h
# End Source File
# Begin Source File

SOURCE=.\paranoia\gap.c
# End Source File
# Begin Source File

SOURCE=.\paranoia\gap.h
# End Source File
# Begin Source File

SOURCE=.\paranoia\isort.c
# End Source File
# Begin Source File

SOURCE=.\paranoia\isort.h
# End Source File
# Begin Source File

SOURCE=.\paranoia\overlap.c
# End Source File
# Begin Source File

SOURCE=.\paranoia\overlap.h
# End Source File
# Begin Source File

SOURCE=.\paranoia\p_block.c
# End Source File
# Begin Source File

SOURCE=.\paranoia\p_block.h
# End Source File
# Begin Source File

SOURCE=.\paranoia\paranoia.c
# End Source File
# Begin Source File

SOURCE=.\interface\smallft.h
# End Source File
# End Group
# End Target
# End Project
