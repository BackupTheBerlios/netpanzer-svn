# Microsoft Developer Studio Project File - Name="netPanzer" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=netPanzer - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "netPanzer.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "netPanzer.mak" CFG="netPanzer - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "netPanzer - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "netPanzer - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "netPanzer - Win32 Release"

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
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /G5 /MT /W3 /Od /Oy /Ob1 /I "..\..\..\directX\ddraw" /I "..\..\..\directX\directInput" /I "..\..\..\directX\directPlay" /I "..\..\..\directX\directSound" /I "..\..\..\Win32" /I "..\..\..\Lib" /I "..\..\..\Lib\interfaces" /I "..\..\..\netPanzer\structs" /I "..\..\..\netPanzer\views" /I "..\..\..\netPanzer\port" /I "..\..\..\netPanzer\Interfaces" /I "..\..\..\netPanzer\Classes" /I "..\..\..\netPanzer\Classes\AI" /I "..\..\..\netPanzer\Classes\Network" /I "..\..\..\netPanzer\Core" /I "..\..\..\Lib\2D" /I "..\..\..\Lib\Interfaces" /I "..\..\..\Lib\Types" /I "..\..\..\Lib\View" /I "..\..\..\Lib\Particles" /I "..\..\..\netPanzer\Classes\weapons" /I "..\..\..\netPanzer\views\mainmenu" /I "..\..\..\netPanzer\views\mainmenu\options" /I "..\..\..\netPanzer\views\mainmenu\multi" /I "..\..\..\netPanzer\views\game" /I "..\..\..\netPanzer\classes\units" /I "..\..\..\Lib\PObject\Classes" /I "..\..\..\Lib\PObject\Templates" /I "..\..\..\netPanzer\resources" /I "..\..\..\Win32\WinSock" /I "..\..\..\Lua\Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_LOGGING_ON" /Yc /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 dplay.lib dplayx.lib dsound.lib dinput.lib dxguid.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib winmm.lib ddraw.lib wininet.lib wsock32.lib /nologo /subsystem:windows /machine:I386 /out:".\Release\netPanzer.exe"
# SUBTRACT LINK32 /profile /pdb:none /incremental:yes /map /debug

!ELSEIF  "$(CFG)" == "netPanzer - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /G5 /MTd /W3 /GX /Zi /Od /Ob1 /I "..\..\..\UILib" /I "..\..\..\UILib\SDL" /I "..\..\..\directX\ddraw" /I "..\..\..\directX\directInput" /I "..\..\..\directX\directPlay" /I "..\..\..\directX\directSound" /I "..\..\..\Win32" /I "..\..\..\Lib" /I "..\..\..\Lib\interfaces" /I "..\..\..\netPanzer\structs" /I "..\..\..\netPanzer\views" /I "..\..\..\netPanzer\port" /I "..\..\..\netPanzer\Interfaces" /I "..\..\..\netPanzer\Classes" /I "..\..\..\netPanzer\Classes\AI" /I "..\..\..\netPanzer\Classes\Network" /I "..\..\..\netPanzer\Core" /I "..\..\..\Lib\2D" /I "..\..\..\Lib\Interfaces" /I "..\..\..\Lib\Types" /I "..\..\..\Lib\View" /I "..\..\..\Lib\Particles" /I "..\..\..\netPanzer\Classes\weapons" /I "..\..\..\netPanzer\views\mainmenu" /I "..\..\..\netPanzer\views\mainmenu\options" /I "..\..\..\netPanzer\views\mainmenu\multi" /I "..\..\..\netPanzer\views\game" /I "..\..\..\netPanzer\classes\units" /I "..\..\..\Lib\PObject\Classes" /I "..\..\..\Lib\PObject\Templates" /I "..\..\..\netPanzer\resources" /I "..\..\..\Win32\WinSock" /I "..\..\..\Lua\Include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_LOGGING_ON" /FR /YX"stdafx.hpp" /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 dplayx.lib dsound.lib dinput.lib dxguid.lib odbc32.lib odbccp32.lib winmm.lib ddraw.lib wsock32.lib SDL.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib /nologo /subsystem:windows /debug /machine:I386 /out:".\Debug\netPanzerD.exe"
# SUBTRACT LINK32 /profile /pdb:none /incremental:no

!ENDIF 

# Begin Target

# Name "netPanzer - Win32 Release"
# Name "netPanzer - Win32 Debug"
# Begin Group "netPanzer"

# PROP Default_Filter ""
# Begin Group "Core"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\NetPanzer\Core\loadPics.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Core\loadPics.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Core\netPanzerGlobals.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Core\netPanzerGlobals.h
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Core\netPanzerMain.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Core\netPanzerMain.h
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Core\netPanzerThread.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Core\netPanzerThread.h
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Core\netPanzerWinMain.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Core\netPanzerWinMain.h
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Core\netPanzerWinProc.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Core\netPanzerWinProc.h
# End Source File
# End Group
# Begin Group "Port"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\NetPanzer\Port\blitters.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Port\blitters.h
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Port\Gdatstct.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Port\Gdatstct.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Port\Globals.h
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Port\Globedef.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Port\Mapdata.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Port\pzrportglobals.cpp
# End Source File
# End Group
# Begin Group "Structs"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\NetPanzer\Structs\Aliasdef.h
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Structs\KeyScanCodeDefs.h
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Structs\MapFileStruct.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Structs\TileSetStruct.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Structs\UnitTypes.hpp
# End Source File
# End Group
# Begin Group "Views"

# PROP Default_Filter ""
# Begin Group "MainMenu"

# PROP Default_Filter ""
# Begin Group "Options"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\MainMenu\Options\ControlsView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\MainMenu\Options\ControlsView.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\MainMenu\Options\InterfaceView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\MainMenu\Options\InterfaceView.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\MainMenu\Options\SoundView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\MainMenu\Options\SoundView.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\MainMenu\Options\VisualsView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\MainMenu\Options\VisualsView.hpp
# End Source File
# End Group
# Begin Group "Multi"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\MainMenu\Multi\DirectPlayErrorExceptionView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\MainMenu\Multi\DirectPlayErrorExceptionView.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\MainMenu\Multi\FlagSelectionView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\MainMenu\Multi\FlagSelectionView.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\MainMenu\Multi\GetConnectionTypeView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\MainMenu\Multi\GetConnectionTypeView.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\MainMenu\Multi\GetSessionHostView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\MainMenu\Multi\GetSessionHostView.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\MainMenu\Multi\GetSessionView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\MainMenu\Multi\GetSessionView.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\MainMenu\Multi\HostJoinTemplateView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\MainMenu\Multi\HostJoinTemplateView.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\MainMenu\Multi\HostOptionsView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\MainMenu\Multi\HostOptionsView.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\MainMenu\Multi\HostView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\MainMenu\Multi\HostView.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\MainMenu\Multi\IPAddressView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\MainMenu\Multi\IPAddressView.h
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\MainMenu\Multi\JoinView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\MainMenu\Multi\JoinView.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\MainMenu\Multi\MapSelectionView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\MainMenu\Multi\MapSelectionView.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\MainMenu\Multi\PlayerNameView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\MainMenu\Multi\PlayerNameView.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\MainMenu\Multi\UnitColorView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\MainMenu\Multi\UnitColorView.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\MainMenu\Multi\UnitSelectionView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\MainMenu\Multi\UnitSelectionView.hpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\MainMenu\HelpView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\MainMenu\HelpView.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\MainMenu\MainMenuView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\MainMenu\MainMenuView.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\MainMenu\MenuTemplateView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\MainMenu\MenuTemplateView.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\MainMenu\OptionsTemplateView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\MainMenu\OptionsTemplateView.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\MainMenu\OrderingView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\MainMenu\OrderingView.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\MainMenu\RMouseHackView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\MainMenu\RMouseHackView.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\MainMenu\SkirmishView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\MainMenu\SkirmishView.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\MainMenu\SpecialButtonView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\MainMenu\SpecialButtonView.hpp
# End Source File
# End Group
# Begin Group "Game"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\Game\AreYouSureExitView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\Game\AreYouSureExitView.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\Game\AreYouSureResignView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\Game\AreYouSureResignView.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\Game\ChatView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\Game\ChatView.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\Game\CodeStatsView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\Game\CodeStatsView.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\Game\ControlPaletteView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\Game\ControlPaletteView.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\Game\ControlView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\Game\ControlView.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\Game\GameInfoView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\Game\GameInfoView.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\Game\GameTemplateView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\Game\GameTemplateView.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\Game\GameToolbarView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\Game\GameToolbarView.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\Game\GameView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\Game\GameView.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\Game\HelpScrollView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\Game\HelpScrollView.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\Game\LobbyView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\Game\LobbyView.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\Game\MiniMapOptionsView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\Game\MiniMapOptionsView.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\Game\MiniMapView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\Game\MiniMapView.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\Game\MiniRankView.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\Game\ProgressView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\Game\ProgressView.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\Game\RankView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\Game\RankView.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\Game\ResignView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\Game\ResignView.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\Game\VehicleSelectionView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\Game\VehicleSelectionView.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\Game\WinnerMesgView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\Game\WinnerMesgView.hpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\GameViewGlobals.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Views\GameViewGlobals.hpp
# End Source File
# End Group
# Begin Group "Resources"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\NetPanzer\Resources\netp.ico
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Resources\netPanzer.rc
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Resources\resource.h
# End Source File
# End Group
# Begin Group "Classes"

# PROP Default_Filter ""
# Begin Group "Units"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Units\Abrams.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Units\Abrams.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Units\Archer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Units\Archer.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Units\Hammerhead.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Units\Hammerhead.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Units\Humvee.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Units\Humvee.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Units\Leopard.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Units\Leopard.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Units\Lynx.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Units\Lynx.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Units\M109.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Units\M109.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Units\Scorpion.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Units\Scorpion.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Units\SpahPanzer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Units\SpahPanzer.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Units\UnitGlobals.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Units\UnitGlobals.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Units\Valentine.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Units\Valentine.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Units\Vehicle.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Units\Vehicle.hpp
# End Source File
# End Group
# Begin Group "Weapons"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Weapons\BulletWeapon.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Weapons\BulletWeapon.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Weapons\MissleWeapon.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Weapons\MissleWeapon.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Weapons\ShellWeapon.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Weapons\ShellWeapon.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Weapons\Weapon.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Weapons\Weapon.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Weapons\WeaponGlobals.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Weapons\WeaponGlobals.hpp
# End Source File
# End Group
# Begin Group "AI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Ai\Astar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Ai\Astar.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Ai\PathingState.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Ai\PathingState.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Ai\PathList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Ai\PathList.hpp
# End Source File
# End Group
# Begin Group "Network"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Network\ClientConnectDaemon.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Network\ClientConnectDaemon.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Network\ClientMessageRouter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Network\ClientMessageRouter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Network\ClientServerNetMessage.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Network\ConnectNetMessage.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Network\GameControlNetMessage.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Network\NetMessageEncoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Network\NetMessageEncoder.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Network\NetMessageEnums.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Network\NetMessageLog.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Network\NetMessageLog.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Network\NetPacket.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Network\NetPacketQueues.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Network\NetPacketQueues.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Network\NetworkClient.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Network\NetworkClient.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Network\NetworkInterface.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Network\NetworkInterface.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Network\NetworkReturnCodes.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Network\NetworkServer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Network\NetworkServer.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Network\NetworkState.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Network\NetworkState.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Network\ObjectiveNetMessage.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Network\PlayerNetMessage.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Network\ServerConnectDaemon.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Network\ServerConnectDaemon.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Network\ServerMessageRouter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Network\ServerMessageRouter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Network\SystemNetMessage.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Network\TerminalNetMesg.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Network\UnitNetMessage.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Network\UnitNetMessage.hpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\BonusUnitPowerUp.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\BonusUnitPowerUp.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\PObject\Templates\BucketArrayTemplate.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\DDHardSurface.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\DDHardSurface.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\EnemyRadarPowerUp.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\EnemyRadarPowerUp.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\GameManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\GameManager.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\InternalLink.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\KeyBinder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\KeyBinder.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Core\NetworkGlobals.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Core\NetworkGlobals.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Objective.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Objective.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\ObjectiveMessageTypes.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Outpost.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\OutPost.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\PlacementMatrix.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\PlacementMatrix.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\PlayerID.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\PlayerID.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\PlayerState.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\PlayerState.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\PlayerUnitConfig.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\PlayerUnitConfig.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\PowerUp.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\PowerUp.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\PowerUpInterface.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\PowerUpInterface.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Network\PowerUpNetMessage.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Projectile.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Projectile.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\SelectionBoxSprite.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\SelectionBoxSprite.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\SelectionList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\SelectionList.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\SpawnList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\SpawnList.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Sprite.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Sprite.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\SpriteSorter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\SpriteSorter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\SurfaceDesc.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\TileEngine.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\TileEngine.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\TileSet.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\TileSet.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Unit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\Unit.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\UnitBase.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\UnitBlackBoard.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\UnitBlackBoard.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\UnitBucketArray.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\UnitBucketArray.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\UnitID.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\UnitIDList.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\UnitLifecycles.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\UnitList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\UnitList.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\UnitMessage.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\UnitMessage.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\UnitMessageTypes.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\UnitOpcodeEncoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\UnitOpcodeEncoder.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\UnitOpcodes.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\UnitPowerUp.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\UnitPowerUp.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\UnitState.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\UnitState.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\ViewCamera.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\ViewCamera.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\WadMapTable.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\WorldInputCmdProcessor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\WorldInputCmdProcessor.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\WorldMap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Classes\WorldMap.hpp
# End Source File
# End Group
# Begin Group "Interfaces"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\ChatInterface.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\ChatInterface.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\Client.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\Client.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\ConsoleInterface.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\ConsoleInterface.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\GameConfig.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\GameConfig.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\GameControlRulesDaemon.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\GameControlRulesDaemon.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\KeyboardInterface.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\KeyboardInterface.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\MapInterface.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\MapInterface.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\MapsManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\MapsManager.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\MiniMapInterface.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\MiniMapInterface.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\MouseInterface.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\MouseInterface.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\NetworkClientDPlay.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\NetworkClientDPlay.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\NetworkClientWinSock.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\NetworkClientWinSock.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\NetworkServerDPlay.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\NetworkServerDPlay.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\NetworkServerWinSock.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\NetworkServerWinSock.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\ObjectiveInterface.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\ObjectiveInterface.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\PathScheduler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\PathScheduler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\PlayerInterface.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\PlayerInterface.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\ProjectileInterface.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\ProjectileInterface.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\Server.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\Server.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\StaticsInterface.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\StaticsInterface.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\Stats.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\Stats.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\TileInterface.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\TileInterface.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\UnitInterface.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\UnitInterface.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\UnitProfileInterface.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\UnitProfileInterface.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\WorldViewInterface.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\NetPanzer\Interfaces\WorldViewInterface.hpp
# End Source File
# End Group
# End Group
# Begin Group "Win32"

# PROP Default_Filter ""
# Begin Group "WinSock"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\Win32\Winsock\WinsockClient.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Win32\Winsock\WinsockClient.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Win32\Winsock\WinSockClientList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Win32\Winsock\WinSockClientList.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Win32\Winsock\winsockglobals.h
# End Source File
# Begin Source File

SOURCE=..\..\..\Win32\Winsock\WinSockMessage.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Win32\Winsock\WinSockMessage.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Win32\Winsock\WinsockServer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Win32\Winsock\WinsockServer.h
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\Win32\gapp.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Win32\gapp.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Win32\Win32Console.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Win32\Win32Console.hpp
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
# Begin Group "DirectX"

# PROP Default_Filter ""
# Begin Group "DirectDraw"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\DirectX\DDraw\DirectDraw.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\DirectX\DDraw\DirectDraw.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\DirectX\DDraw\DirectDrawError.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\DirectX\DDraw\DirectDrawError.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\DirectX\DDraw\DirectDrawglobals.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\DirectX\DDraw\DirectDrawPalette.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\DirectX\DDraw\DirectDrawPalette.hpp
# End Source File
# End Group
# Begin Group "DirectSound"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\DirectX\DirectSound\DSound.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\DirectX\DirectSound\DSound.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\DirectX\DirectSound\sfx.h
# End Source File
# Begin Source File

SOURCE=..\..\..\DirectX\DirectSound\wave.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\DirectX\DirectSound\wave.h
# End Source File
# Begin Source File

SOURCE=..\..\..\DirectX\DirectSound\wavfile.h
# End Source File
# End Group
# Begin Group "DirectInput"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\DirectX\DirectInput\DirectInput.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\DirectX\DirectInput\DirectInput.hpp
# End Source File
# End Group
# Begin Group "DirectPlay"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\DirectX\DirectPlay\DirectPlay.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\DirectX\DirectPlay\DirectPlay.h
# End Source File
# Begin Source File

SOURCE=..\..\..\DirectX\DirectPlay\DPlayError.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\DirectX\DirectPlay\DPlayError.hpp
# End Source File
# End Group
# End Group
# Begin Group "Lib"

# PROP Default_Filter ""
# Begin Group "PObject"

# PROP Default_Filter ""
# Begin Group "PObject-Classes"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\Lib\PObject\Classes\Angle.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\PObject\Classes\Angle.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\PObject\Classes\BitArray.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\PObject\Classes\BitArray.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\PObject\Classes\BoundBox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\PObject\Classes\BoundBox.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\PObject\Classes\BresenhamLine.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\PObject\Classes\BresenhamLine.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\PObject\Classes\FileList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\PObject\Classes\FileList.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\PObject\Classes\PArray.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\PObject\Classes\PArray.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\PObject\Classes\PArrayGrowable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\PObject\Classes\PArrayGrowable.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\PObject\Classes\PHeap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\PObject\Classes\PHeap.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\PObject\Classes\PObject.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\PObject\Classes\PObject.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\PObject\Classes\Point.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\PObject\Classes\Point.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\PObject\Classes\PPriorityQueue.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\PObject\Classes\PPriorityQueue.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\PObject\Classes\PQueue.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\PObject\Classes\PQueue.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\PObject\Classes\SafePointer.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\PObject\Classes\Timer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\PObject\Classes\Timer.hpp
# End Source File
# End Group
# Begin Group "PObject-Templates"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\Lib\PObject\Templates\ArrayGrowableTemplate.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\PObject\Templates\ArrayTemplate.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\PObject\Templates\LinkListDoubleTemplate.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\PObject\Templates\LinkListSingleTemplate.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\PObject\Templates\QueueTemplate.hpp
# End Source File
# End Group
# End Group
# Begin Group "Environment"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\Lib\Environment\Weather.hpp
# End Source File
# End Group
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
# Begin Group "LibInterfaces"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\Lib\Interfaces\JoystickInterface.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Interfaces\JoystickInterface.hpp
# End Source File
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
# Begin Group "Particles"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\Lib\Particles\ChunkTrajectoryParticle2D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Particles\ChunkTrajectoryParticle2D.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Particles\CloudParticle2D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Particles\CloudParticle2D.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Particles\CraterParticle2D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Particles\CraterParticle2D.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Particles\ExplosionFlashParticle2D.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Particles\FireParticle2D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Particles\FireParticle2D.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Particles\FireParticleSystem2D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Particles\FireParticleSystem2D.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Particles\FlameParticle2D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Particles\FlameParticle2D.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Particles\FlashParticle2D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Particles\FlashParticle2D.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Particles\GroundExplosionParticleSystem2D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Particles\GroundExplosionParticleSystem2D.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Particles\MovementDirtPuffParticle2D.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Particles\Particle2D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Particles\Particle2D.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Particles\ParticleInterface.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Particles\ParticleInterface.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Particles\ParticleSystem2D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Particles\ParticleSystem2D.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Particles\ParticleSystemGlobals.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Particles\ParticleSystemGlobals.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Particles\ParticleTweakView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Particles\ParticleTweakView.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Particles\PuffParticle2D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Particles\PuffParticle2D.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Particles\SmolderParticleSystem2D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Particles\SmolderParticleSystem2D.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Particles\SnowParticle2D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Particles\SnowParticle2D.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Particles\SnowParticleSystem2D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Particles\SnowParticleSystem2D.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Particles\TemplateExplosionSystem.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Particles\TemplateExplosionSystem.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Particles\TemplateMuzzleSystem.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Particles\TemplateMuzzleSystem.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Particles\TrajectoryParticle2D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Particles\TrajectoryParticle2D.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Particles\VectorPuffParticle2D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Particles\VectorPuffParticle2D.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Particles\WindParticle2D.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Particles\WindParticle2D.hpp
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

SOURCE=..\..\..\Lib\View\AbstractButton.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\View\AbstractButton.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\View\Button.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\View\Button.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\View\cButton.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\View\cButton.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\View\CheckBox.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\View\CheckBox.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\View\Choice.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\View\Choice.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\View\cInputField.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\View\cInputField.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\View\Component.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\View\Component.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\View\Cursor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\View\Cursor.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\View\Desktop.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\View\Desktop.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\View\Dialog.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\View\Dialog.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\View\FileDialog.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\View\FileDialog.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\View\Icon.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\View\Icon.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\View\InputEvent.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\View\Label.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\View\Label.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\View\List.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\View\List.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\View\MouseEvent.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\View\ScrollBar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\View\ScrollBar.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\View\String.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\View\String.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\View\TextArea.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\View\TextArea.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\View\TextComponent.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\View\TextComponent.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\View\TextField.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\View\TextField.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\View\TitledBorder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\View\TitledBorder.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\View\View.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\View\View.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\View\ViewGlobals.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\View\ViewGlobals.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\View\Window.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\View\Window.hpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\Lib\cGrowList.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\cMouse.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\cMouse.hpp
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

SOURCE=..\..\..\Lib\LibView.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\LibView.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\List.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\loadSaveData.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\loadSaveData.hpp
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

SOURCE=..\..\..\Lib\Physics.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\Physics.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\Lib\terminate.cpp
# End Source File
# End Group
# Begin Group "UILib"

# PROP Default_Filter ""
# Begin Group "SDL"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\UILib\SDL\SDLDraw.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\UILib\SDL\SDLDraw.hpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\UILib\UIDraw.hpp
# End Source File
# End Group
# End Target
# End Project
