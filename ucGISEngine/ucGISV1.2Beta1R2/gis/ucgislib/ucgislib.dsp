# Microsoft Developer Studio Project File - Name="ucgislib" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=ucgislib - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ucgislib.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ucgislib.mak" CFG="ucgislib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ucgislib - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "ucgislib - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ucgislib - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /c
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"E:\uC-GUI\Start\gis\uc_gislib\lib\ucgislib.lib"

!ELSEIF  "$(CFG)" == "ucgislib - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"E:\hz\Softpack\Softpack\uCGUI-4.14\start\uc_gislib\lib\ucgislib.lib"

!ENDIF 

# Begin Target

# Name "ucgislib - Win32 Release"
# Name "ucgislib - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\syb\1.c
# End Source File
# Begin Source File

SOURCE=..\syb\1000.c
# End Source File
# Begin Source File

SOURCE=..\syb\1100.c
# End Source File
# Begin Source File

SOURCE=..\syb\1200.c
# End Source File
# Begin Source File

SOURCE=..\syb\1300.c
# End Source File
# Begin Source File

SOURCE=..\syb\1400.c
# End Source File
# Begin Source File

SOURCE=..\syb\1401.c
# End Source File
# Begin Source File

SOURCE=..\syb\1500.c
# End Source File
# Begin Source File

SOURCE=..\syb\1600.c
# End Source File
# Begin Source File

SOURCE=..\syb\1700.c
# End Source File
# Begin Source File

SOURCE=..\syb\1800.c
# End Source File
# Begin Source File

SOURCE=..\syb\1900.c
# End Source File
# Begin Source File

SOURCE=..\syb\2000.c
# End Source File
# Begin Source File

SOURCE=..\syb\2201.c
# End Source File
# Begin Source File

SOURCE=..\syb\2202.c
# End Source File
# Begin Source File

SOURCE=..\syb\2300.c
# End Source File
# Begin Source File

SOURCE=..\syb\2401.c
# End Source File
# Begin Source File

SOURCE=..\syb\2402.c
# End Source File
# Begin Source File

SOURCE=..\syb\2403.c
# End Source File
# Begin Source File

SOURCE=..\syb\2404.c
# End Source File
# Begin Source File

SOURCE=..\syb\2405.c
# End Source File
# Begin Source File

SOURCE=..\syb\2406.c
# End Source File
# Begin Source File

SOURCE=..\syb\2700.c
# End Source File
# Begin Source File

SOURCE=..\car.c
# End Source File
# Begin Source File

SOURCE=..\syb\center.c
# End Source File
# Begin Source File

SOURCE=..\syb\centertrans.c
# End Source File
# Begin Source File

SOURCE=..\CoorTrans.c
# End Source File
# Begin Source File

SOURCE=..\Encrypt.c
# End Source File
# Begin Source File

SOURCE=..\..\GUI\chinese\gb_uc_conver.c
# End Source File
# Begin Source File

SOURCE=..\GisApp.c
# End Source File
# Begin Source File

SOURCE=..\gisbase.c
# End Source File
# Begin Source File

SOURCE=..\gisfunction.c
# End Source File
# Begin Source File

SOURCE=..\gisfunction.h
# End Source File
# Begin Source File

SOURCE=..\GisInterface.c
# End Source File
# Begin Source File

SOURCE=..\syb\GPS_BitMap.c
# End Source File
# Begin Source File

SOURCE=..\gpsdatainfo.c
# End Source File
# Begin Source File

SOURCE=..\GridIndex.c
# End Source File
# Begin Source File

SOURCE=..\..\GUI\Core\GUI_UC_EncodeGBK.c
# End Source File
# Begin Source File

SOURCE=..\Layer.c
# End Source File
# Begin Source File

SOURCE=..\Line.c
# End Source File
# Begin Source File

SOURCE=..\mypoint.c
# End Source File
# Begin Source File

SOURCE=..\PLine.c
# End Source File
# Begin Source File

SOURCE=..\PLineRgn.c
# End Source File
# Begin Source File

SOURCE=..\syb\Return.c
# End Source File
# Begin Source File

SOURCE=..\syb\Search.c
# End Source File
# Begin Source File

SOURCE=..\userlayer.c
# End Source File
# Begin Source File

SOURCE=..\syb\ZOOMIN.c
# End Source File
# Begin Source File

SOURCE=..\syb\ZOOMOUT.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\car.h
# End Source File
# Begin Source File

SOURCE=..\CoorTrans.h
# End Source File
# Begin Source File

SOURCE=..\..\GUI\chinese\gb2312.h
# End Source File
# Begin Source File

SOURCE=..\GisApi.h
# End Source File
# Begin Source File

SOURCE=..\gisbase.h
# End Source File
# Begin Source File

SOURCE=..\GISVersion.h
# End Source File
# Begin Source File

SOURCE=..\gpsdatainfo.h
# End Source File
# Begin Source File

SOURCE=..\gridindex.h
# End Source File
# Begin Source File

SOURCE=..\Layer.h
# End Source File
# Begin Source File

SOURCE=..\Line.h
# End Source File
# Begin Source File

SOURCE=..\mypoint.h
# End Source File
# Begin Source File

SOURCE=..\PLine.h
# End Source File
# Begin Source File

SOURCE=..\PlineRgn.h
# End Source File
# Begin Source File

SOURCE=..\userlayer.h
# End Source File
# End Group
# End Target
# End Project
