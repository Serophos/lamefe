# Microsoft Developer Studio Project File - Name="VorbisDLL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=VorbisDLL - Win32 Debug
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "VorbisDLL.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "VorbisDLL.mak" CFG="VorbisDLL - Win32 Debug"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "VorbisDLL - Win32 Release" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE "VorbisDLL - Win32 Debug" (basierend auf  "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "VorbisDLL - Win32 Release"

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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../include" /I "../lib" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_VORBISDLL" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /machine:I386 /out:"./Release/Vorbis.dll"

!ELSEIF  "$(CFG)" == "VorbisDLL - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "VorbisDL"
# PROP BASE Intermediate_Dir "VorbisDL"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../include" /I "../lib" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_VORBISDLL" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"Debug\Vorbis.dll" /pdbtype:sept
# SUBTRACT LINK32 /incremental:no /nodefaultlib

!ENDIF 

# Begin Target

# Name "VorbisDLL - Win32 Release"
# Name "VorbisDLL - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\lib\analysis.c
# End Source File
# Begin Source File

SOURCE=..\lib\barkmel.c
# End Source File
# Begin Source File

SOURCE=..\lib\bitbuffer.c
# End Source File
# Begin Source File

SOURCE=..\lib\ogg\bitwise.c
# End Source File
# Begin Source File

SOURCE=..\lib\block.c
# End Source File
# Begin Source File

SOURCE=..\lib\codebook.c
# End Source File
# Begin Source File

SOURCE=..\lib\envelope.c
# End Source File
# Begin Source File

SOURCE=..\lib\floor0.c
# End Source File
# Begin Source File

SOURCE=..\lib\floor1.c
# End Source File
# Begin Source File

SOURCE=..\lib\ogg\framing.c
# End Source File
# Begin Source File

SOURCE=..\lib\info.c
# End Source File
# Begin Source File

SOURCE=..\lib\lpc.c
# End Source File
# Begin Source File

SOURCE=..\lib\lsp.c
# End Source File
# Begin Source File

SOURCE=..\lib\mapping0.c
# End Source File
# Begin Source File

SOURCE=..\lib\mdct.c
# End Source File
# Begin Source File

SOURCE=..\lib\psy.c
# End Source File
# Begin Source File

SOURCE=..\lib\registry.c
# End Source File
# Begin Source File

SOURCE=..\lib\res0.c
# End Source File
# Begin Source File

SOURCE=..\lib\sharedbook.c
# End Source File
# Begin Source File

SOURCE=..\lib\smallft.c
# End Source File
# Begin Source File

SOURCE=..\lib\synthesis.c
# End Source File
# Begin Source File

SOURCE=..\lib\time0.c
# End Source File
# Begin Source File

SOURCE=.\utf8.c
# End Source File
# Begin Source File

SOURCE=.\VorbisDLL.c
# End Source File
# Begin Source File

SOURCE=.\VorbisDLL.def
# End Source File
# Begin Source File

SOURCE=..\lib\vorbisenc.c
# End Source File
# Begin Source File

SOURCE=..\lib\vorbisfile.c
# End Source File
# Begin Source File

SOURCE=..\lib\window.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\lib\bitbuffer.h
# End Source File
# Begin Source File

SOURCE=..\lib\iir.h
# End Source File
# Begin Source File

SOURCE=..\lib\sharedbook.h
# End Source File
# Begin Source File

SOURCE=.\utf8.h
# End Source File
# Begin Source File

SOURCE=.\VorbisDLL.h
# End Source File
# End Group
# End Target
# End Project
