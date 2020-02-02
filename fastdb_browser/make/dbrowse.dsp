# Microsoft Developer Studio Project File - Name="dbrowse" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** NICHT BEARBEITEN **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=dbrowse - Win32 Debug
!MESSAGE Dies ist kein gültiges Makefile. Zum Erstellen dieses Projekts mit NMAKE
!MESSAGE verwenden Sie den Befehl "Makefile exportieren" und führen Sie den Befehl
!MESSAGE 
!MESSAGE NMAKE /f "dbrowse.mak".
!MESSAGE 
!MESSAGE Sie können beim Ausführen von NMAKE eine Konfiguration angeben
!MESSAGE durch Definieren des Makros CFG in der Befehlszeile. Zum Beispiel:
!MESSAGE 
!MESSAGE NMAKE /f "dbrowse.mak" CFG="dbrowse - Win32 Debug"
!MESSAGE 
!MESSAGE Für die Konfiguration stehen zur Auswahl:
!MESSAGE 
!MESSAGE "dbrowse - Win32 Release" (basierend auf  "Win32 (x86) Application")
!MESSAGE "dbrowse - Win32 Debug" (basierend auf  "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "dbrowse - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "../bin_release"
# PROP Intermediate_Dir "Release/dbrowse"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "../src/hd" /I "../src/dbrowse" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "STRICT" /D "USE_QUEUE_MANAGER" /D "USE_STD_STRING" /D "SET_NULL_DACL" /D "AUTOINCREMENT_SUPPORT" /D FASTDB_DEBUG=DEBUG_TRACE /D "CLONE_IDENTIFIERS" /D "THROW_EXCEPTION_ON_ERROR" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "dbrowse - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\bin_debug"
# PROP Intermediate_Dir "Debug/dbrowse"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "../src/hd" /I "../src/dbrowse" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "STRICT" /D "USE_STD_STRING" /D "SET_NULL_DACL" /D "AUTOINCREMENT_SUPPORT" /D FASTDB_DEBUG=DEBUG_TRACE /D "CLONE_IDENTIFIERS" /D "THROW_EXCEPTION_ON_ERROR" /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "dbrowse - Win32 Release"
# Name "dbrowse - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\src\dbrowse\aboutdlg.cpp
# End Source File
# Begin Source File

SOURCE=..\src\dbrowse\ChildFrm.cpp
# End Source File
# Begin Source File

SOURCE=..\src\dbrowse\dbrowse.cpp
# End Source File
# Begin Source File

SOURCE=..\src\dbrowse\dbrowse.rc
# End Source File
# Begin Source File

SOURCE=..\src\dbrowse\dbrowseview.cpp
# End Source File
# Begin Source File

SOURCE=..\src\dbrowse\exporter.cpp
# End Source File
# Begin Source File

SOURCE=..\src\dbrowse\helpview.cpp
# End Source File
# Begin Source File

SOURCE=..\src\dbrowse\mainfrm.cpp
# End Source File
# Begin Source File

SOURCE=..\src\dbrowse\stdafx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\src\dbrowse\aboutdlg.h
# End Source File
# Begin Source File

SOURCE=..\src\dbrowse\ChildFrm.h
# End Source File
# Begin Source File

SOURCE=..\src\dbrowse\dbrowseview.h
# End Source File
# Begin Source File

SOURCE=..\src\dbrowse\exporter.h
# End Source File
# Begin Source File

SOURCE=..\src\dbrowse\helpview.h
# End Source File
# Begin Source File

SOURCE=..\src\dbrowse\mainfrm.h
# End Source File
# Begin Source File

SOURCE=..\src\dbrowse\queryprompter.h
# End Source File
# Begin Source File

SOURCE=..\src\dbrowse\resource.h
# End Source File
# Begin Source File

SOURCE=..\src\dbrowse\stdafx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\src\dbrowse\res\arraycont.gif
# End Source File
# Begin Source File

SOURCE=..\src\dbrowse\res\dataview.gif
# End Source File
# Begin Source File

SOURCE=..\src\dbrowse\res\dbrowse.exe.manifest
# End Source File
# Begin Source File

SOURCE=..\src\dbrowse\res\dbrowse.ico
# End Source File
# Begin Source File

SOURCE=..\src\dbrowse\res\dbrowsedoc.ico
# End Source File
# Begin Source File

SOURCE=..\src\dbrowse\res\dbView.gif
# End Source File
# Begin Source File

SOURCE=..\src\dbrowse\res\detail.gif
# End Source File
# Begin Source File

SOURCE=..\src\dbrowse\res\fieldedit.gif
# End Source File
# Begin Source File

SOURCE=..\src\dbrowse\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=..\src\dbrowse\res\maingui.gif
# End Source File
# Begin Source File

SOURCE=..\src\dbrowse\res\query.gif
# End Source File
# Begin Source File

SOURCE=..\src\dbrowse\res\schemaView.gif
# End Source File
# Begin Source File

SOURCE=..\src\dbrowse\res\toolbar.bmp
# End Source File
# Begin Source File

SOURCE=..\src\dbrowse\res\toolbar.gif
# End Source File
# Begin Source File

SOURCE=..\src\dbrowse\res\uml.gif
# End Source File
# End Group
# Begin Source File

SOURCE=..\src\dbrowse\res\dbrowser.htm
# End Source File
# End Target
# End Project
