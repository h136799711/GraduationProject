# Microsoft Developer Studio Project File - Name="GraduationProject" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=GraduationProject - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "GraduationProject.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "GraduationProject.mak" CFG="GraduationProject - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "GraduationProject - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "GraduationProject - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "GraduationProject - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "GraduationProject - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "GraduationProject - Win32 Release"
# Name "GraduationProject - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "2DEngineCPP"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\HCLib\HCDDrawLib.cpp
# End Source File
# Begin Source File

SOURCE=.\HCLib\HCDXBuilder.cpp
# End Source File
# Begin Source File

SOURCE=.\HCLib\HCInput.cpp
# End Source File
# Begin Source File

SOURCE=.\HCLib\HCLog.cpp
# End Source File
# Begin Source File

SOURCE=.\HCLib\HCMath.cpp
# End Source File
# Begin Source File

SOURCE=.\HCLib\HCPhysical.cpp
# End Source File
# Begin Source File

SOURCE=.\HCLib\HCResource.cpp
# End Source File
# Begin Source File

SOURCE=.\HCLib\HCSound.cpp
# End Source File
# Begin Source File

SOURCE=.\HCLib\HCUtility.cpp
# End Source File
# Begin Source File

SOURCE=.\HCLib\HCWindowBuilder.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "2DEngineH"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\HCLib\HCDDrawLib.h
# End Source File
# Begin Source File

SOURCE=.\HCLib\HCDXBuilder.h
# End Source File
# Begin Source File

SOURCE=.\HCLib\HCInput.h
# End Source File
# Begin Source File

SOURCE=.\HCLib\HCLog.h
# End Source File
# Begin Source File

SOURCE=.\HCLib\HCMacros.h
# End Source File
# Begin Source File

SOURCE=.\HCLib\HCMath.h
# End Source File
# Begin Source File

SOURCE=.\HCLib\HCPhysical.h
# End Source File
# Begin Source File

SOURCE=.\HCLib\HCResource.h
# End Source File
# Begin Source File

SOURCE=.\HCReturnType.h
# End Source File
# Begin Source File

SOURCE=.\HCLib\HCSound.h
# End Source File
# Begin Source File

SOURCE=.\HCLib\HCUtility.h
# End Source File
# Begin Source File

SOURCE=.\HCLib\HCWindowBuilder.h
# End Source File
# End Group
# Begin Group "3DMathH"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Geom.h
# End Source File
# Begin Source File

SOURCE=.\Math3D.h
# End Source File
# Begin Source File

SOURCE=.\MathDef.h
# End Source File
# Begin Source File

SOURCE=.\Matrix.h
# End Source File
# Begin Source File

SOURCE=.\Vector.h
# End Source File
# End Group
# Begin Group "Render3D"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Draw3DV1.h
# End Source File
# Begin Source File

SOURCE=.\ObjectDef.h
# End Source File
# Begin Source File

SOURCE=.\RenderPipeline3DV1.h
# End Source File
# End Group
# Begin Group "ModelLoader"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\PLGLoader.h
# End Source File
# End Group
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
