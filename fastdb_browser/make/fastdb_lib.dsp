# Microsoft Developer Studio Project File - Name="fastdb_lib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=fastdb_lib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "fastdb_lib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "fastdb_lib.mak" CFG="fastdb_lib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "fastdb_lib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "fastdb_lib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "fastdb_lib - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\bin_release"
# PROP Intermediate_Dir "Release/fastdb_lib"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /I "..\src\hd\fastdb" /D "NDEBUG" /D "THROW_EXCEPTION_ON_ERROR" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "USE_QUEUE_MANAGER" /D "USE_STD_STRING" /D "SET_NULL_DACL" /D "AUTOINCREMENT_SUPPORT" /D FASTDB_DEBUG=DEBUG_TRACE /D "CLONE_IDENTIFIERS" /YX /FD /c
# ADD BASE RSC /l 0x407 /d "NDEBUG"
# ADD RSC /l 0x407 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ELSEIF  "$(CFG)" == "fastdb_lib - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\bin_debug"
# PROP Intermediate_Dir "Debug\fastdb_lib"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /I "..\src\hd\fastdb" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "USE_QUEUE_MANAGER" /D "USE_STD_STRING" /D "SET_NULL_DACL" /D "AUTOINCREMENT_SUPPORT" /D FASTDB_DEBUG=DEBUG_TRACE /D "CLONE_IDENTIFIERS" /D "THROW_EXCEPTION_ON_ERROR" /YX /FD /GZ /c
# ADD BASE RSC /l 0x407 /d "_DEBUG"
# ADD RSC /l 0x407 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo

!ENDIF 

# Begin Target

# Name "fastdb_lib - Win32 Release"
# Name "fastdb_lib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\src\fastdb\class.cpp
# End Source File
# Begin Source File

SOURCE=..\src\fastdb\compiler.cpp
# End Source File
# Begin Source File

SOURCE=..\src\fastdb\container.cpp
# End Source File
# Begin Source File

SOURCE=..\src\fastdb\cursor.cpp
# End Source File
# Begin Source File

SOURCE=..\src\fastdb\database.cpp
# End Source File
# Begin Source File

SOURCE=..\src\fastdb\fastdb.cpp
# End Source File
# Begin Source File

SOURCE=..\src\fastdb\file.cpp
# End Source File
# Begin Source File

SOURCE=..\src\fastdb\hashtab.cpp
# End Source File
# Begin Source File

SOURCE=..\src\fastdb\localcli.cpp
# End Source File
# Begin Source File

SOURCE=..\src\fastdb\query.cpp
# End Source File
# Begin Source File

SOURCE=..\src\fastdb\symtab.cpp
# End Source File
# Begin Source File

SOURCE=..\src\fastdb\sync.cpp
# End Source File
# Begin Source File

SOURCE=..\src\fastdb\ttree.cpp
# End Source File
# Begin Source File

SOURCE=..\src\fastdb\w32sock.cpp
# End Source File
# Begin Source File

SOURCE=..\src\fastdb\wwwapi.cpp
# End Source File
# Begin Source File

SOURCE=..\src\fastdb\xmlio.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\src\hd\fastdb\array.h
# End Source File
# Begin Source File

SOURCE=..\src\hd\fastdb\class.h
# End Source File
# Begin Source File

SOURCE=..\src\hd\fastdb\cli.h
# End Source File
# Begin Source File

SOURCE=..\src\hd\fastdb\cliproto.h
# End Source File
# Begin Source File

SOURCE=..\src\hd\fastdb\compiler.h
# End Source File
# Begin Source File

SOURCE=..\src\hd\fastdb\config.h
# End Source File
# Begin Source File

SOURCE=..\src\hd\fastdb\container.h
# End Source File
# Begin Source File

SOURCE=..\src\hd\fastdb\cursor.h
# End Source File
# Begin Source File

SOURCE=..\src\hd\fastdb\database.h
# End Source File
# Begin Source File

SOURCE=..\src\hd\fastdb\date.h
# End Source File
# Begin Source File

SOURCE=..\src\hd\fastdb\datetime.h
# End Source File
# Begin Source File

SOURCE=..\src\hd\fastdb\exception.h
# End Source File
# Begin Source File

SOURCE=..\src\hd\fastdb\fastdb.h
# End Source File
# Begin Source File

SOURCE=..\src\hd\fastdb\file.h
# End Source File
# Begin Source File

SOURCE=..\src\hd\fastdb\harray.h
# End Source File
# Begin Source File

SOURCE=..\src\hd\fastdb\hashtab.h
# End Source File
# Begin Source File

SOURCE=..\src\hd\fastdb\iqsort.h
# End Source File
# Begin Source File

SOURCE=..\src\hd\fastdb\localcli.h
# End Source File
# Begin Source File

SOURCE=..\src\hd\fastdb\query.h
# End Source File
# Begin Source File

SOURCE=..\src\hd\fastdb\reference.h
# End Source File
# Begin Source File

SOURCE=..\src\hd\fastdb\repsock.h
# End Source File
# Begin Source File

SOURCE=..\src\hd\fastdb\server.h
# End Source File
# Begin Source File

SOURCE=..\src\hd\fastdb\set.h
# End Source File
# Begin Source File

SOURCE=..\src\hd\fastdb\sockio.h
# End Source File
# Begin Source File

SOURCE=..\src\hd\fastdb\StdAfx.h
# End Source File
# Begin Source File

SOURCE=..\src\hd\fastdb\stdtp.h
# End Source File
# Begin Source File

SOURCE=..\src\hd\fastdb\subsql.h
# End Source File
# Begin Source File

SOURCE=..\src\hd\fastdb\symtab.h
# End Source File
# Begin Source File

SOURCE=..\src\hd\fastdb\sync.h
# End Source File
# Begin Source File

SOURCE=..\src\hd\fastdb\sync_w32.h
# End Source File
# Begin Source File

SOURCE=..\src\hd\fastdb\ttree.h
# End Source File
# Begin Source File

SOURCE=..\src\hd\fastdb\w32sock.h
# End Source File
# Begin Source File

SOURCE=..\src\hd\fastdb\wwwapi.h
# End Source File
# Begin Source File

SOURCE=..\src\hd\fastdb\xmlio.h
# End Source File
# End Group
# End Target
# End Project
