# Microsoft Developer Studio Project File - Name="ImageRobot" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ImageRobot - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "imagerobot.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "imagerobot.mak" CFG="ImageRobot - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ImageRobot - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ImageRobot - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ImageRobot - Win32 Release"

# PROP BASE Use_MFC 5
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /G5 /MT /W3 /GX /O2 /I "..\..\..\mattlib\m2d" /I "..\..\..\mattlib" /I "..\..\..\mattlib\mTypes" /I "..\..\..\mattlib\mInterfaces" /I "..\..\..\Lib\2D" /I "..\..\..\Lib" /I "..\..\..\Lib\Types" /I "..\..\..\Lib\Interfaces" /I "..\..\..\win32" /I "..\..\..\Lib\View" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX"stdafx.hpp" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /subsystem:windows /machine:I386 /out:"imagerobot.exe"

!ELSEIF  "$(CFG)" == "ImageRobot - Win32 Debug"

# PROP BASE Use_MFC 5
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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /G5 /MDd /W3 /Gm /GX /Zi /Od /I "..\..\..\Lib\2D" /I "..\..\..\Lib" /I "..\..\..\Lib\Types" /I "..\..\..\Lib\Interfaces" /I "..\..\..\win32" /I "..\..\..\Lib\View" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /YX"stdafx.hpp" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib /nologo /subsystem:windows /debug /machine:I386 /out:"imagerobot.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "ImageRobot - Win32 Release"
# Name "ImageRobot - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\PyroUtil\ImgRobot\ColorPakDialog.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\PyroUtil\ImgRobot\cropinfodialog.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\PyroUtil\ImgRobot\ImageRobot.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\PyroUtil\ImgRobot\ImageRobot.rc
# End Source File
# Begin Source File

SOURCE=..\..\..\PyroUtil\ImgRobot\ImageRobotDlg.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\PyroUtil\ImgRobot\ImageRobotGlobals.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\PyroUtil\ImgRobot\ImageUtils.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\PyroUtil\ImgRobot\PackFileDialog.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\PyroUtil\ImgRobot\PObject.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\PyroUtil\ImgRobot\RenameInfoDialog.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\PyroUtil\ImgRobot\StdAfx.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\PyroUtil\ImgRobot\Aliasdef.h
# End Source File
# Begin Source File

SOURCE=..\..\..\PyroUtil\ImgRobot\ArrayTemplate.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\PyroUtil\ImgRobot\ColorPakDialog.h
# End Source File
# Begin Source File

SOURCE=..\..\..\PyroUtil\ImgRobot\cropinfodialog.h
# End Source File
# Begin Source File

SOURCE=..\..\..\PyroUtil\ImgRobot\ImageRobot.h
# End Source File
# Begin Source File

SOURCE=..\..\..\PyroUtil\ImgRobot\ImageRobotDlg.h
# End Source File
# Begin Source File

SOURCE=..\..\..\PyroUtil\ImgRobot\ImageRobotGlobals.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\PyroUtil\ImgRobot\ImageUtils.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\PyroUtil\ImgRobot\PackFileDialog.h
# End Source File
# Begin Source File

SOURCE=..\..\..\PyroUtil\ImgRobot\PObject.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\PyroUtil\ImgRobot\RenameInfoDialog.h
# End Source File
# Begin Source File

SOURCE=..\..\..\PyroUtil\ImgRobot\resource.h
# End Source File
# Begin Source File

SOURCE=..\..\..\PyroUtil\ImgRobot\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;cnt;rtf;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\..\..\PyroUtil\ImgRobot\ImageRobot.ico
# End Source File
# Begin Source File

SOURCE=..\..\..\PyroUtil\ImgRobot\ImageRobot.rc2
# End Source File
# End Group
# Begin Group "Win32"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\Win32\gapp.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Win32\gapp.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Win32\Win32Mouse.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Win32\Win32Mouse.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Win32\WinTimer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Win32\WinTimer.hpp
# End Source File
# End Group
# Begin Group "Lib"

# PROP Default_Filter ""
# Begin Group "2D"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\Lib\2D\Color.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\2D\Color.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\2D\ColorTable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\2D\ColorTable.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\2D\DigitText.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\2D\DigitText.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\2D\FontSystem2D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\2D\FontSystem2D.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\2D\PackedSurface.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\2D\PackedSurface.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\2D\Palette.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\2D\Palette.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\2D\RGBColor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\2D\RGBColor.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\2D\Span.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\2D\Span.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\2D\Surface.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\2D\Surface.hpp
# End Source File
# End Group
# Begin Group "Types"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\Lib\Types\fRect.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Types\fRect.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Types\fXY.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Types\fXYZ.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Types\fXYZ.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Types\iRect.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Types\iRect.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Types\iXY.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Types\iXY.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Types\LibTypes.hpp
# End Source File
# End Group
# Begin Group "View"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\Lib\View\String.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\View\String.hpp
# End Source File
# End Group
# Begin Group "Interfaces"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\Lib\Interfaces\TimerInterface.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Interfaces\TimerInterface.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Interfaces\UtilInterface.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Interfaces\UtilInterface.hpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\Lib\cGrowList.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\codewiz.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\cSafeString.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\cTimeStamp.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\cTimeStamp.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\List.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\log.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Math.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Math.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Stdafx.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\terminate.cpp
# End Source File
# End Group
# End Target
# End Project
