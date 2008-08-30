#
# Gererated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/mac-debug/GNU-MacOSX

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/src/NetPanzer/Views/Game/HelpScrollView.o \
	${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi/MasterServer/ServerList.o \
	${OBJECTDIR}/src/NetPanzer/Views/Game/LoadingView.o \
	${OBJECTDIR}/src/NetPanzer/Particles/WindParticle2D.o \
	${OBJECTDIR}/src/NetPanzer/System/SDLSound.o \
	${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi/HostJoinTemplateView.o \
	${OBJECTDIR}/src/NetPanzer/Interfaces/BaseGameManager.o \
	${OBJECTDIR}/src/NetPanzer/Classes/PlayerState.o \
	${OBJECTDIR}/src/NetPanzer/Views/Components/cButton.o \
	${OBJECTDIR}/src/NetPanzer/Interfaces/InfoSocket.o \
	${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Options/InterfaceView.o \
	${OBJECTDIR}/src/Lib/lua/ltm.o \
	${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi/MapSelectionView.o \
	${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi/MasterServer/ServerQueryThread.o \
	${OBJECTDIR}/src/NetPanzer/Classes/Weapons/WeaponGlobals.o \
	${OBJECTDIR}/src/NetPanzer/Classes/Network/ConnectNetMessage.o \
	${OBJECTDIR}/src/NetPanzer/Views/Components/ViewGlobals.o \
	${OBJECTDIR}/src/NetPanzer/Classes/Network/ClientMessageRouter.o \
	${OBJECTDIR}/src/NetPanzer/Particles/TemplateMuzzleSystem.o \
	${OBJECTDIR}/src/Lib/2D/BuiltinFont.o \
	${OBJECTDIR}/src/NetPanzer/Particles/ParticleInterface.o \
	${OBJECTDIR}/src/NetPanzer/Classes/Projectile.o \
	${OBJECTDIR}/src/NetPanzer/Particles/ParticleTweakView.o \
	${OBJECTDIR}/src/NetPanzer/Classes/Network/NetworkServer.o \
	${OBJECTDIR}/src/NetPanzer/Bot/BotPlayer.o \
	${OBJECTDIR}/src/NetPanzer/Classes/Weapons/ShellWeapon.o \
	${OBJECTDIR}/src/Lib/Network/Address.o \
	${OBJECTDIR}/src/NetPanzer/Classes/WorldInputCmdProcessor.o \
	${OBJECTDIR}/src/Lib/physfs/archivers/zip.o \
	${OBJECTDIR}/src/NetPanzer/Classes/Units/Vehicle.o \
	${OBJECTDIR}/src/NetPanzer/Classes/MapFile.o \
	${OBJECTDIR}/src/Lib/Util/StringUtil.o \
	${OBJECTDIR}/src/Lib/Util/TimeStamp.o \
	${OBJECTDIR}/src/NetPanzer/Classes/ScreenSurface.o \
	${OBJECTDIR}/src/NetPanzer/Classes/Network/NetMessageDecoder.o \
	${OBJECTDIR}/src/Lib/2D/Palette.o \
	${OBJECTDIR}/src/Lib/2D/Color.o \
	${OBJECTDIR}/src/NetPanzer/Particles/FlashParticle2D.o \
	${OBJECTDIR}/src/Lib/lua/lvm.o \
	${OBJECTDIR}/src/NetPanzer/Classes/Network/NetworkClient.o \
	${OBJECTDIR}/src/Lib/lua/lstate.o \
	${OBJECTDIR}/src/NetPanzer/Classes/Network/NetworkState.o \
	${OBJECTDIR}/src/NetPanzer/Interfaces/Heartbeat.o \
	${OBJECTDIR}/src/NetPanzer/Interfaces/KeyboardInterface.o \
	${OBJECTDIR}/src/NetPanzer/Interfaces/ServerConsole.o \
	${OBJECTDIR}/src/NetPanzer/Interfaces/PlayerInterface.o \
	${OBJECTDIR}/src/NetPanzer/Classes/SpriteSorter.o \
	${OBJECTDIR}/src/NetPanzer/Port/blitters.o \
	${OBJECTDIR}/src/Lib/2D/Surface.o \
	${OBJECTDIR}/src/Lib/physfs/zlib123/inflate.o \
	${OBJECTDIR}/src/Lib/lua/lmathlib.o \
	${OBJECTDIR}/src/Lib/Util/BresenhamLine.o \
	${OBJECTDIR}/src/NetPanzer/Interfaces/MapInterface.o \
	${OBJECTDIR}/src/NetPanzer/System/SDLEvents.o \
	${OBJECTDIR}/src/NetPanzer/Views/MainMenu/SpecialButtonView.o \
	${OBJECTDIR}/src/NetPanzer/Interfaces/UnitProfileInterface.o \
	${OBJECTDIR}/src/NetPanzer/Classes/TileSet.o \
	${OBJECTDIR}/src/Lib/lua/loadlib.o \
	${OBJECTDIR}/src/NetPanzer/Core/main.o \
	${OBJECTDIR}/src/Lib/lua/lgc.o \
	${OBJECTDIR}/src/Lib/2D/Span.o \
	${OBJECTDIR}/src/NetPanzer/Interfaces/unix/NetworkServerUnix.o \
	${OBJECTDIR}/src/NetPanzer/Classes/UnitBucketArray.o \
	${OBJECTDIR}/src/NetPanzer/Particles/TrajectoryParticle2D.o \
	${OBJECTDIR}/src/NetPanzer/Particles/SmokingTrajectoryParticle2D.o \
	${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi/GetSessionView.o \
	${OBJECTDIR}/src/NetPanzer/Interfaces/Console.o \
	${OBJECTDIR}/src/NetPanzer/Particles/GroundExplosionParticle2D.o \
	${OBJECTDIR}/src/NetPanzer/Interfaces/DedicatedGameManager.o \
	${OBJECTDIR}/src/Lib/lua/lopcodes.o \
	${OBJECTDIR}/src/NetPanzer/Classes/Network/NetworkInterface.o \
	${OBJECTDIR}/src/NetPanzer/Interfaces/TileInterface.o \
	${OBJECTDIR}/src/Lib/Types/fRect.o \
	${OBJECTDIR}/src/NetPanzer/Particles/CloudParticle2D.o \
	${OBJECTDIR}/src/NetPanzer/Classes/EnemyRadarPowerUp.o \
	${OBJECTDIR}/src/NetPanzer/Classes/OpcodeDebugger.o \
	${OBJECTDIR}/src/NetPanzer/System/DummySound.o \
	${OBJECTDIR}/src/Lib/lua/lfunc.o \
	${OBJECTDIR}/src/NetPanzer/Classes/Outpost.o \
	${OBJECTDIR}/src/NetPanzer/Classes/Weapons/BulletWeapon.o \
	${OBJECTDIR}/src/NetPanzer/Classes/UnitPowerUp.o \
	${OBJECTDIR}/src/NetPanzer/Interfaces/UnitInterface.o \
	${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi/UnitColorView.o \
	${OBJECTDIR}/src/NetPanzer/Particles/SmolderParticleSystem2D.o \
	${OBJECTDIR}/src/NetPanzer/Views/Game/LibView.o \
	${OBJECTDIR}/src/NetPanzer/Views/Components/CheckBox.o \
	${OBJECTDIR}/src/Lib/INIParser/Store.o \
	${OBJECTDIR}/src/NetPanzer/Classes/UnitOpcodeEncoder.o \
	${OBJECTDIR}/src/Lib/physfs/physfs_byteorder.o \
	${OBJECTDIR}/src/Lib/physfs/physfs_unicode.o \
	${OBJECTDIR}/src/NetPanzer/Particles/GroundExplosionParticleSystem2D.o \
	${OBJECTDIR}/src/NetPanzer/Views/Game/RankView.o \
	${OBJECTDIR}/src/NetPanzer/Classes/Network/NetMessageEncoder.o \
	${OBJECTDIR}/src/Lib/physfs/zlib123/compress.o \
	${OBJECTDIR}/src/NetPanzer/Particles/FlameParticle2D.o \
	${OBJECTDIR}/src/Lib/lua/lparser.o \
	${OBJECTDIR}/src/Lib/lua/linit.o \
	${OBJECTDIR}/src/NetPanzer/Particles/DirtKickParticle2D.o \
	${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi/FlagSelectionView.o \
	${OBJECTDIR}/src/Lib/lua/lstrlib.o \
	${OBJECTDIR}/src/Lib/2D/ColorTable.o \
	${OBJECTDIR}/src/Lib/physfs/zlib123/inftrees.o \
	${OBJECTDIR}/src/NetPanzer/Interfaces/ObjectiveInterface.o \
	${OBJECTDIR}/src/NetPanzer/Views/Game/DisconectedView.o \
	${OBJECTDIR}/src/Lib/Types/Angle.o \
	${OBJECTDIR}/src/Lib/physfs/platform/posix.o \
	${OBJECTDIR}/src/Lib/physfs/zlib123/infback.o \
	${OBJECTDIR}/src/NetPanzer/Views/Components/Desktop.o \
	${OBJECTDIR}/src/NetPanzer/Classes/ViewCamera.o \
	${OBJECTDIR}/src/NetPanzer/Views/MainMenu/RMouseHackView.o \
	${OBJECTDIR}/src/Lib/lua/lauxlib.o \
	${OBJECTDIR}/src/Lib/physfs/platform/windows.o \
	${OBJECTDIR}/src/NetPanzer/Interfaces/ConsoleInterface.o \
	${OBJECTDIR}/src/NetPanzer/Interfaces/BotGameManager.o \
	${OBJECTDIR}/src/NetPanzer/Views/Game/GameInfoView.o \
	${OBJECTDIR}/src/Lib/physfs/zlib123/uncompr.o \
	${OBJECTDIR}/src/Lib/lua/lcode.o \
	${OBJECTDIR}/src/Lib/2D/DigitText.o \
	${OBJECTDIR}/src/NetPanzer/Particles/PuffParticle2D.o \
	${OBJECTDIR}/src/NetPanzer/Views/MainMenu/OrderingView.o \
	${OBJECTDIR}/src/Lib/Types/fXYZ.o \
	${OBJECTDIR}/src/NetPanzer/Views/Game/AreYouSureExitView.o \
	${OBJECTDIR}/src/NetPanzer/Classes/PowerUp.o \
	${OBJECTDIR}/src/Lib/physfs/zlib123/gzio.o \
	${OBJECTDIR}/src/NetPanzer/Classes/PlayerUnitConfig.o \
	${OBJECTDIR}/src/Lib/Types/iXY.o \
	${OBJECTDIR}/src/NetPanzer/Interfaces/ChatInterface.o \
	${OBJECTDIR}/src/Lib/lua/lapi.o \
	${OBJECTDIR}/src/Lib/physfs/physfs.o \
	${OBJECTDIR}/src/NetPanzer/Views/MainMenu/HelpView.o \
	${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi/JoinView.o \
	${OBJECTDIR}/src/NetPanzer/Network/ProxyServer.o \
	${OBJECTDIR}/src/NetPanzer/Classes/Network/ServerMessageRouter.o \
	${OBJECTDIR}/src/NetPanzer/Interfaces/Server.o \
	${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi/PlayerNameView.o \
	${OBJECTDIR}/src/NetPanzer/System/Sound.o \
	${OBJECTDIR}/src/NetPanzer/Particles/FireParticleSystem2D.o \
	${OBJECTDIR}/src/Lib/lua/lzio.o \
	${OBJECTDIR}/src/NetPanzer/Views/Components/ScrollBar.o \
	${OBJECTDIR}/src/NetPanzer/Classes/Network/NetPacketDebugger.o \
	${OBJECTDIR}/src/NetPanzer/Classes/UnitBlackBoard.o \
	${OBJECTDIR}/src/Lib/lua/print.o \
	${OBJECTDIR}/src/NetPanzer/Particles/ParticleSystem2D.o \
	${OBJECTDIR}/src/NetPanzer/Classes/WorldMap.o \
	${OBJECTDIR}/src/Lib/physfs/archivers/dir.o \
	${OBJECTDIR}/src/Lib/lua/ldblib.o \
	${OBJECTDIR}/src/NetPanzer/Views/MainMenu/MainMenuView.o \
	${OBJECTDIR}/src/NetPanzer/Classes/UnitState.o \
	${OBJECTDIR}/src/NetPanzer/Interfaces/PowerUpInterface.o \
	${OBJECTDIR}/src/NetPanzer/Views/MainMenu/SkirmishView.o \
	${OBJECTDIR}/src/Lib/physfs/zlib123/crc32.o \
	${OBJECTDIR}/src/NetPanzer/Interfaces/PlayerGameManager.o \
	${OBJECTDIR}/src/NetPanzer/Interfaces/GameConfig.o \
	${OBJECTDIR}/src/Lib/Util/Math.o \
	${OBJECTDIR}/src/Lib/Util/FileSystem.o \
	${OBJECTDIR}/src/NetPanzer/Particles/ParticleSystemGlobals.o \
	${OBJECTDIR}/src/NetPanzer/Core/NetworkGlobals.o \
	${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi/HostOptionsView.o \
	${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi/MasterServer/ServerInfo.o \
	${OBJECTDIR}/src/Lib/Util/Exception.o \
	${OBJECTDIR}/src/Lib/ArrayUtil/BoundBox.o \
	${OBJECTDIR}/src/NetPanzer/Interfaces/ProjectileInterface.o \
	${OBJECTDIR}/src/NetPanzer/Interfaces/MouseInterface.o \
	${OBJECTDIR}/src/NetPanzer/Particles/VectorPuffParticle2D.o \
	${OBJECTDIR}/src/NetPanzer/Classes/PlayerID.o \
	${OBJECTDIR}/src/NetPanzer/Views/Game/MiniMapView.o \
	${OBJECTDIR}/src/NetPanzer/Interfaces/ConfigVariable.o \
	${OBJECTDIR}/src/Lib/2D/PackedSurface.o \
	${OBJECTDIR}/src/NetPanzer/Classes/SelectionBoxSprite.o \
	${OBJECTDIR}/src/NetPanzer/Network/ClientSocket.o \
	${OBJECTDIR}/src/Lib/physfs/zlib123/adler32.o \
	${OBJECTDIR}/src/NetPanzer/Interfaces/GameControlRulesDaemon.o \
	${OBJECTDIR}/src/Lib/physfs/platform/beos.o \
	${OBJECTDIR}/src/NetPanzer/Particles/CraterParticle2D.o \
	${OBJECTDIR}/src/NetPanzer/Views/Components/cInputField.o \
	${OBJECTDIR}/src/NetPanzer/Views/Components/Component.o \
	${OBJECTDIR}/src/NetPanzer/Classes/Network/UnitSync.o \
	${OBJECTDIR}/src/NetPanzer/Views/Components/Label.o \
	${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi/HostView.o \
	${OBJECTDIR}/src/Lib/Util/UtilInterface.o \
	${OBJECTDIR}/src/NetPanzer/Classes/SelectionList.o \
	${OBJECTDIR}/src/NetPanzer/Interfaces/MiniMapInterface.o \
	${OBJECTDIR}/src/NetPanzer/Classes/TileEngine.o \
	${OBJECTDIR}/src/Lib/physfs/zlib123/trees.o \
	${OBJECTDIR}/src/NetPanzer/Particles/SnowParticleSystem2D.o \
	${OBJECTDIR}/src/NetPanzer/Interfaces/Client.o \
	${OBJECTDIR}/src/Lib/lua/lua.o \
	${OBJECTDIR}/src/NetPanzer/Classes/BonusUnitPowerUp.o \
	${OBJECTDIR}/src/NetPanzer/Particles/SparkParticle2D.o \
	${OBJECTDIR}/src/Lib/physfs/zlib123/zutil.o \
	${OBJECTDIR}/src/NetPanzer/Bot/Bot.o \
	${OBJECTDIR}/src/Lib/lua/lmem.o \
	${OBJECTDIR}/src/Lib/lua/liolib.o \
	${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Options/VisualsView.o \
	${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi/ServerListView.o \
	${OBJECTDIR}/src/NetPanzer/Interfaces/unix/NetworkClientUnix.o \
	${OBJECTDIR}/src/NetPanzer/Classes/AI/PathList.o \
	${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Options/ControlsView.o \
	${OBJECTDIR}/src/Lib/lua/ldump.o \
	${OBJECTDIR}/src/NetPanzer/Classes/Weapons/MissleWeapon.o \
	${OBJECTDIR}/src/NetPanzer/Classes/Sprite.o \
	${OBJECTDIR}/src/NetPanzer/Particles/SnowParticle2D.o \
	${OBJECTDIR}/src/NetPanzer/Classes/Units/UnitGlobals.o \
	${OBJECTDIR}/src/NetPanzer/Views/Components/Button.o \
	${OBJECTDIR}/src/Lib/Types/iRect.o \
	${OBJECTDIR}/src/Lib/lua/lundump.o \
	${OBJECTDIR}/src/Lib/Util/TimerInterface.o \
	${OBJECTDIR}/src/Lib/physfs/zlib123/inffast.o \
	${OBJECTDIR}/src/NetPanzer/Interfaces/WorldViewInterface.o \
	${OBJECTDIR}/src/NetPanzer/Particles/Physics.o \
	${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Options/SoundView.o \
	${OBJECTDIR}/src/Lib/Network/TCPListenSocket.o \
	${OBJECTDIR}/src/Lib/Network/TCPSocket.o \
	${OBJECTDIR}/src/NetPanzer/Views/Game/GameTemplateView.o \
	${OBJECTDIR}/src/NetPanzer/Interfaces/GameManager.o \
	${OBJECTDIR}/src/NetPanzer/Bot/BotTaskList.o \
	${OBJECTDIR}/src/Lib/Util/Timer.o \
	${OBJECTDIR}/src/NetPanzer/Views/Game/CodeStatsView.o \
	${OBJECTDIR}/src/NetPanzer/Network/ServerSocket.o \
	${OBJECTDIR}/src/Lib/physfs/platform/pocketpc.o \
	${OBJECTDIR}/src/Lib/lua/ltable.o \
	${OBJECTDIR}/src/NetPanzer/Particles/RadarPingParticle2D.o \
	${OBJECTDIR}/src/NetPanzer/Particles/Particle2D.o \
	${OBJECTDIR}/src/NetPanzer/Interfaces/MapsManager.o \
	${OBJECTDIR}/src/Lib/lua/lstring.o \
	${OBJECTDIR}/src/NetPanzer/System/SDLVideo.o \
	${OBJECTDIR}/src/Lib/lua/ldo.o \
	${OBJECTDIR}/src/NetPanzer/Views/MainMenu/OptionsTemplateView.o \
	${OBJECTDIR}/src/Lib/optionmm/option.o \
	${OBJECTDIR}/src/Lib/lua/lobject.o \
	${OBJECTDIR}/src/Lib/lua/llex.o \
	${OBJECTDIR}/src/NetPanzer/Views/Game/ResignView.o \
	${OBJECTDIR}/src/Lib/Util/Log.o \
	${OBJECTDIR}/src/Lib/optionmm/command_line.o \
	${OBJECTDIR}/src/NetPanzer/Views/MainMenu/MenuTemplateView.o \
	${OBJECTDIR}/src/NetPanzer/Views/Game/VehicleSelectionView.o \
	${OBJECTDIR}/src/NetPanzer/Classes/UnitOpcodeDecoder.o \
	${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi/IPAddressView.o \
	${OBJECTDIR}/src/Lib/physfs/zlib123/deflate.o \
	${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi/UnitSelectionView.o \
	${OBJECTDIR}/src/NetPanzer/Classes/Network/ClientConnectDaemon.o \
	${OBJECTDIR}/src/Lib/lua/lbaselib.o \
	${OBJECTDIR}/src/NetPanzer/Views/Game/AreYouSureResignView.o \
	${OBJECTDIR}/src/Lib/physfs/platform/macosx.o \
	${OBJECTDIR}/src/NetPanzer/Particles/FireParticle2D.o \
	${OBJECTDIR}/src/NetPanzer/Classes/Network/NetPacketQueues.o \
	${OBJECTDIR}/src/Lib/physfs/platform/os2.o \
	${OBJECTDIR}/src/Lib/Network/SocketBase.o \
	${OBJECTDIR}/src/NetPanzer/Interfaces/PathScheduler.o \
	${OBJECTDIR}/src/Lib/Network/UDPSocket.o \
	${OBJECTDIR}/src/NetPanzer/Classes/TipOfDay.o \
	${OBJECTDIR}/src/NetPanzer/Classes/Objective.o \
	${OBJECTDIR}/src/NetPanzer/Particles/TemplateExplosionSystem.o \
	${OBJECTDIR}/src/Lib/physfs/platform/unix.o \
	${OBJECTDIR}/src/NetPanzer/Classes/SpawnList.o \
	${OBJECTDIR}/src/Lib/lua/ldebug.o \
	${OBJECTDIR}/src/Lib/Network/SocketManager.o \
	${OBJECTDIR}/src/Lib/ArrayUtil/BitArray.o \
	${OBJECTDIR}/src/NetPanzer/Classes/AI/Astar.o \
	${OBJECTDIR}/src/Lib/INIParser/Section.o \
	${OBJECTDIR}/src/NetPanzer/Views/Components/View.o \
	${OBJECTDIR}/src/NetPanzer/Classes/Weapons/Weapon.o \
	${OBJECTDIR}/src/Lib/lua/loslib.o \
	${OBJECTDIR}/src/Lib/2D/RGBColor.o \
	${OBJECTDIR}/src/NetPanzer/Classes/Network/ServerConnectDaemon.o \
	${OBJECTDIR}/src/NetPanzer/Views/Game/GameView.o \
	${OBJECTDIR}/src/NetPanzer/Views/Components/Choice.o \
	${OBJECTDIR}/src/NetPanzer/Particles/ChunkTrajectoryParticle2D.o \
	${OBJECTDIR}/src/NetPanzer/Classes/PlacementMatrix.o \
	${OBJECTDIR}/src/Lib/lua/ltablib.o \
	${OBJECTDIR}/src/NetPanzer/Views/GameViewGlobals.o \
	${OBJECTDIR}/src/NetPanzer/Classes/AI/PathingState.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS} ./netpanzer-debug

./netpanzer-debug: ${OBJECTFILES}
	${MKDIR} -p .
	${LINK.cc} -lSDLmain -framework SDL -framework SDL_mixer -framework IOKit -framework Cocoa -o ./netpanzer-debug ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/src/NetPanzer/Views/Game/HelpScrollView.o: src/NetPanzer/Views/Game/HelpScrollView.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Views/Game
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Views/Game/HelpScrollView.o src/NetPanzer/Views/Game/HelpScrollView.cpp

${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi/MasterServer/ServerList.o: src/NetPanzer/Views/MainMenu/Multi/MasterServer/ServerList.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi/MasterServer
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi/MasterServer/ServerList.o src/NetPanzer/Views/MainMenu/Multi/MasterServer/ServerList.cpp

${OBJECTDIR}/src/NetPanzer/Views/Game/LoadingView.o: src/NetPanzer/Views/Game/LoadingView.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Views/Game
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Views/Game/LoadingView.o src/NetPanzer/Views/Game/LoadingView.cpp

${OBJECTDIR}/src/NetPanzer/Particles/WindParticle2D.o: src/NetPanzer/Particles/WindParticle2D.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Particles
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Particles/WindParticle2D.o src/NetPanzer/Particles/WindParticle2D.cpp

${OBJECTDIR}/src/NetPanzer/System/SDLSound.o: src/NetPanzer/System/SDLSound.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/System
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/System/SDLSound.o src/NetPanzer/System/SDLSound.cpp

${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi/HostJoinTemplateView.o: src/NetPanzer/Views/MainMenu/Multi/HostJoinTemplateView.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi/HostJoinTemplateView.o src/NetPanzer/Views/MainMenu/Multi/HostJoinTemplateView.cpp

${OBJECTDIR}/src/NetPanzer/Interfaces/BaseGameManager.o: src/NetPanzer/Interfaces/BaseGameManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Interfaces
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Interfaces/BaseGameManager.o src/NetPanzer/Interfaces/BaseGameManager.cpp

${OBJECTDIR}/src/NetPanzer/Classes/PlayerState.o: src/NetPanzer/Classes/PlayerState.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/PlayerState.o src/NetPanzer/Classes/PlayerState.cpp

${OBJECTDIR}/src/NetPanzer/Views/Components/cButton.o: src/NetPanzer/Views/Components/cButton.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Views/Components
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Views/Components/cButton.o src/NetPanzer/Views/Components/cButton.cpp

${OBJECTDIR}/src/NetPanzer/Interfaces/InfoSocket.o: src/NetPanzer/Interfaces/InfoSocket.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Interfaces
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Interfaces/InfoSocket.o src/NetPanzer/Interfaces/InfoSocket.cpp

${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Options/InterfaceView.o: src/NetPanzer/Views/MainMenu/Options/InterfaceView.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Options
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Options/InterfaceView.o src/NetPanzer/Views/MainMenu/Options/InterfaceView.cpp

${OBJECTDIR}/src/Lib/lua/ltm.o: src/Lib/lua/ltm.c 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/lua
	$(COMPILE.c) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/lua/ltm.o src/Lib/lua/ltm.c

${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi/MapSelectionView.o: src/NetPanzer/Views/MainMenu/Multi/MapSelectionView.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi/MapSelectionView.o src/NetPanzer/Views/MainMenu/Multi/MapSelectionView.cpp

${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi/MasterServer/ServerQueryThread.o: src/NetPanzer/Views/MainMenu/Multi/MasterServer/ServerQueryThread.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi/MasterServer
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi/MasterServer/ServerQueryThread.o src/NetPanzer/Views/MainMenu/Multi/MasterServer/ServerQueryThread.cpp

${OBJECTDIR}/src/NetPanzer/Classes/Weapons/WeaponGlobals.o: src/NetPanzer/Classes/Weapons/WeaponGlobals.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes/Weapons
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/Weapons/WeaponGlobals.o src/NetPanzer/Classes/Weapons/WeaponGlobals.cpp

${OBJECTDIR}/src/NetPanzer/Classes/Network/ConnectNetMessage.o: src/NetPanzer/Classes/Network/ConnectNetMessage.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes/Network
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/Network/ConnectNetMessage.o src/NetPanzer/Classes/Network/ConnectNetMessage.cpp

${OBJECTDIR}/src/NetPanzer/Views/Components/ViewGlobals.o: src/NetPanzer/Views/Components/ViewGlobals.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Views/Components
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Views/Components/ViewGlobals.o src/NetPanzer/Views/Components/ViewGlobals.cpp

${OBJECTDIR}/src/NetPanzer/Classes/Network/ClientMessageRouter.o: src/NetPanzer/Classes/Network/ClientMessageRouter.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes/Network
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/Network/ClientMessageRouter.o src/NetPanzer/Classes/Network/ClientMessageRouter.cpp

${OBJECTDIR}/src/NetPanzer/Particles/TemplateMuzzleSystem.o: src/NetPanzer/Particles/TemplateMuzzleSystem.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Particles
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Particles/TemplateMuzzleSystem.o src/NetPanzer/Particles/TemplateMuzzleSystem.cpp

${OBJECTDIR}/src/Lib/2D/BuiltinFont.o: src/Lib/2D/BuiltinFont.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/2D
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/2D/BuiltinFont.o src/Lib/2D/BuiltinFont.cpp

${OBJECTDIR}/src/NetPanzer/Particles/ParticleInterface.o: src/NetPanzer/Particles/ParticleInterface.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Particles
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Particles/ParticleInterface.o src/NetPanzer/Particles/ParticleInterface.cpp

${OBJECTDIR}/src/NetPanzer/Classes/Projectile.o: src/NetPanzer/Classes/Projectile.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/Projectile.o src/NetPanzer/Classes/Projectile.cpp

${OBJECTDIR}/src/NetPanzer/Particles/ParticleTweakView.o: src/NetPanzer/Particles/ParticleTweakView.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Particles
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Particles/ParticleTweakView.o src/NetPanzer/Particles/ParticleTweakView.cpp

${OBJECTDIR}/src/NetPanzer/Classes/Network/NetworkServer.o: src/NetPanzer/Classes/Network/NetworkServer.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes/Network
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/Network/NetworkServer.o src/NetPanzer/Classes/Network/NetworkServer.cpp

${OBJECTDIR}/src/NetPanzer/Bot/BotPlayer.o: src/NetPanzer/Bot/BotPlayer.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Bot
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Bot/BotPlayer.o src/NetPanzer/Bot/BotPlayer.cpp

${OBJECTDIR}/src/NetPanzer/Classes/Weapons/ShellWeapon.o: src/NetPanzer/Classes/Weapons/ShellWeapon.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes/Weapons
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/Weapons/ShellWeapon.o src/NetPanzer/Classes/Weapons/ShellWeapon.cpp

${OBJECTDIR}/src/Lib/Network/Address.o: src/Lib/Network/Address.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/Network
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/Network/Address.o src/Lib/Network/Address.cpp

${OBJECTDIR}/src/NetPanzer/Classes/WorldInputCmdProcessor.o: src/NetPanzer/Classes/WorldInputCmdProcessor.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/WorldInputCmdProcessor.o src/NetPanzer/Classes/WorldInputCmdProcessor.cpp

${OBJECTDIR}/src/Lib/physfs/archivers/zip.o: src/Lib/physfs/archivers/zip.c 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/physfs/archivers
	$(COMPILE.c) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/physfs/archivers/zip.o src/Lib/physfs/archivers/zip.c

${OBJECTDIR}/src/NetPanzer/Classes/Units/Vehicle.o: src/NetPanzer/Classes/Units/Vehicle.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes/Units
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/Units/Vehicle.o src/NetPanzer/Classes/Units/Vehicle.cpp

${OBJECTDIR}/src/NetPanzer/Classes/MapFile.o: src/NetPanzer/Classes/MapFile.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/MapFile.o src/NetPanzer/Classes/MapFile.cpp

${OBJECTDIR}/src/Lib/Util/StringUtil.o: src/Lib/Util/StringUtil.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/Util
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/Util/StringUtil.o src/Lib/Util/StringUtil.cpp

${OBJECTDIR}/src/Lib/Util/TimeStamp.o: src/Lib/Util/TimeStamp.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/Util
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/Util/TimeStamp.o src/Lib/Util/TimeStamp.cpp

${OBJECTDIR}/src/NetPanzer/Classes/ScreenSurface.o: src/NetPanzer/Classes/ScreenSurface.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/ScreenSurface.o src/NetPanzer/Classes/ScreenSurface.cpp

${OBJECTDIR}/src/NetPanzer/Classes/Network/NetMessageDecoder.o: src/NetPanzer/Classes/Network/NetMessageDecoder.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes/Network
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/Network/NetMessageDecoder.o src/NetPanzer/Classes/Network/NetMessageDecoder.cpp

${OBJECTDIR}/src/Lib/2D/Palette.o: src/Lib/2D/Palette.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/2D
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/2D/Palette.o src/Lib/2D/Palette.cpp

${OBJECTDIR}/src/Lib/2D/Color.o: src/Lib/2D/Color.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/2D
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/2D/Color.o src/Lib/2D/Color.cpp

${OBJECTDIR}/src/NetPanzer/Particles/FlashParticle2D.o: src/NetPanzer/Particles/FlashParticle2D.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Particles
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Particles/FlashParticle2D.o src/NetPanzer/Particles/FlashParticle2D.cpp

${OBJECTDIR}/src/Lib/lua/lvm.o: src/Lib/lua/lvm.c 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/lua
	$(COMPILE.c) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/lua/lvm.o src/Lib/lua/lvm.c

${OBJECTDIR}/src/NetPanzer/Classes/Network/NetworkClient.o: src/NetPanzer/Classes/Network/NetworkClient.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes/Network
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/Network/NetworkClient.o src/NetPanzer/Classes/Network/NetworkClient.cpp

${OBJECTDIR}/src/Lib/lua/lstate.o: src/Lib/lua/lstate.c 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/lua
	$(COMPILE.c) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/lua/lstate.o src/Lib/lua/lstate.c

${OBJECTDIR}/src/NetPanzer/Classes/Network/NetworkState.o: src/NetPanzer/Classes/Network/NetworkState.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes/Network
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/Network/NetworkState.o src/NetPanzer/Classes/Network/NetworkState.cpp

${OBJECTDIR}/src/NetPanzer/Interfaces/Heartbeat.o: src/NetPanzer/Interfaces/Heartbeat.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Interfaces
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Interfaces/Heartbeat.o src/NetPanzer/Interfaces/Heartbeat.cpp

${OBJECTDIR}/src/NetPanzer/Interfaces/KeyboardInterface.o: src/NetPanzer/Interfaces/KeyboardInterface.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Interfaces
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Interfaces/KeyboardInterface.o src/NetPanzer/Interfaces/KeyboardInterface.cpp

${OBJECTDIR}/src/NetPanzer/Interfaces/ServerConsole.o: src/NetPanzer/Interfaces/ServerConsole.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Interfaces
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Interfaces/ServerConsole.o src/NetPanzer/Interfaces/ServerConsole.cpp

${OBJECTDIR}/src/NetPanzer/Interfaces/PlayerInterface.o: src/NetPanzer/Interfaces/PlayerInterface.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Interfaces
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Interfaces/PlayerInterface.o src/NetPanzer/Interfaces/PlayerInterface.cpp

${OBJECTDIR}/src/NetPanzer/Classes/SpriteSorter.o: src/NetPanzer/Classes/SpriteSorter.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/SpriteSorter.o src/NetPanzer/Classes/SpriteSorter.cpp

${OBJECTDIR}/src/NetPanzer/Port/blitters.o: src/NetPanzer/Port/blitters.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Port
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Port/blitters.o src/NetPanzer/Port/blitters.cpp

${OBJECTDIR}/src/Lib/2D/Surface.o: src/Lib/2D/Surface.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/2D
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/2D/Surface.o src/Lib/2D/Surface.cpp

${OBJECTDIR}/src/Lib/physfs/zlib123/inflate.o: src/Lib/physfs/zlib123/inflate.c 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/physfs/zlib123
	$(COMPILE.c) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/physfs/zlib123/inflate.o src/Lib/physfs/zlib123/inflate.c

${OBJECTDIR}/src/Lib/lua/lmathlib.o: src/Lib/lua/lmathlib.c 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/lua
	$(COMPILE.c) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/lua/lmathlib.o src/Lib/lua/lmathlib.c

${OBJECTDIR}/src/Lib/Util/BresenhamLine.o: src/Lib/Util/BresenhamLine.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/Util
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/Util/BresenhamLine.o src/Lib/Util/BresenhamLine.cpp

${OBJECTDIR}/src/NetPanzer/Interfaces/MapInterface.o: src/NetPanzer/Interfaces/MapInterface.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Interfaces
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Interfaces/MapInterface.o src/NetPanzer/Interfaces/MapInterface.cpp

${OBJECTDIR}/src/NetPanzer/System/SDLEvents.o: src/NetPanzer/System/SDLEvents.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/System
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/System/SDLEvents.o src/NetPanzer/System/SDLEvents.cpp

${OBJECTDIR}/src/NetPanzer/Views/MainMenu/SpecialButtonView.o: src/NetPanzer/Views/MainMenu/SpecialButtonView.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Views/MainMenu
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Views/MainMenu/SpecialButtonView.o src/NetPanzer/Views/MainMenu/SpecialButtonView.cpp

${OBJECTDIR}/src/NetPanzer/Interfaces/UnitProfileInterface.o: src/NetPanzer/Interfaces/UnitProfileInterface.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Interfaces
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Interfaces/UnitProfileInterface.o src/NetPanzer/Interfaces/UnitProfileInterface.cpp

${OBJECTDIR}/src/NetPanzer/Classes/TileSet.o: src/NetPanzer/Classes/TileSet.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/TileSet.o src/NetPanzer/Classes/TileSet.cpp

${OBJECTDIR}/src/Lib/lua/loadlib.o: src/Lib/lua/loadlib.c 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/lua
	$(COMPILE.c) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/lua/loadlib.o src/Lib/lua/loadlib.c

${OBJECTDIR}/src/NetPanzer/Core/main.o: src/NetPanzer/Core/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Core
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Core/main.o src/NetPanzer/Core/main.cpp

${OBJECTDIR}/src/Lib/lua/lgc.o: src/Lib/lua/lgc.c 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/lua
	$(COMPILE.c) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/lua/lgc.o src/Lib/lua/lgc.c

${OBJECTDIR}/src/Lib/2D/Span.o: src/Lib/2D/Span.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/2D
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/2D/Span.o src/Lib/2D/Span.cpp

${OBJECTDIR}/src/NetPanzer/Interfaces/unix/NetworkServerUnix.o: src/NetPanzer/Interfaces/unix/NetworkServerUnix.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Interfaces/unix
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Interfaces/unix/NetworkServerUnix.o src/NetPanzer/Interfaces/unix/NetworkServerUnix.cpp

${OBJECTDIR}/src/NetPanzer/Classes/UnitBucketArray.o: src/NetPanzer/Classes/UnitBucketArray.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/UnitBucketArray.o src/NetPanzer/Classes/UnitBucketArray.cpp

${OBJECTDIR}/src/NetPanzer/Particles/TrajectoryParticle2D.o: src/NetPanzer/Particles/TrajectoryParticle2D.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Particles
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Particles/TrajectoryParticle2D.o src/NetPanzer/Particles/TrajectoryParticle2D.cpp

${OBJECTDIR}/src/NetPanzer/Particles/SmokingTrajectoryParticle2D.o: src/NetPanzer/Particles/SmokingTrajectoryParticle2D.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Particles
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Particles/SmokingTrajectoryParticle2D.o src/NetPanzer/Particles/SmokingTrajectoryParticle2D.cpp

${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi/GetSessionView.o: src/NetPanzer/Views/MainMenu/Multi/GetSessionView.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi/GetSessionView.o src/NetPanzer/Views/MainMenu/Multi/GetSessionView.cpp

${OBJECTDIR}/src/NetPanzer/Interfaces/Console.o: src/NetPanzer/Interfaces/Console.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Interfaces
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Interfaces/Console.o src/NetPanzer/Interfaces/Console.cpp

${OBJECTDIR}/src/NetPanzer/Particles/GroundExplosionParticle2D.o: src/NetPanzer/Particles/GroundExplosionParticle2D.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Particles
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Particles/GroundExplosionParticle2D.o src/NetPanzer/Particles/GroundExplosionParticle2D.cpp

${OBJECTDIR}/src/NetPanzer/Interfaces/DedicatedGameManager.o: src/NetPanzer/Interfaces/DedicatedGameManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Interfaces
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Interfaces/DedicatedGameManager.o src/NetPanzer/Interfaces/DedicatedGameManager.cpp

${OBJECTDIR}/src/Lib/lua/lopcodes.o: src/Lib/lua/lopcodes.c 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/lua
	$(COMPILE.c) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/lua/lopcodes.o src/Lib/lua/lopcodes.c

${OBJECTDIR}/src/NetPanzer/Classes/Network/NetworkInterface.o: src/NetPanzer/Classes/Network/NetworkInterface.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes/Network
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/Network/NetworkInterface.o src/NetPanzer/Classes/Network/NetworkInterface.cpp

${OBJECTDIR}/src/NetPanzer/Interfaces/TileInterface.o: src/NetPanzer/Interfaces/TileInterface.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Interfaces
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Interfaces/TileInterface.o src/NetPanzer/Interfaces/TileInterface.cpp

${OBJECTDIR}/src/Lib/Types/fRect.o: src/Lib/Types/fRect.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/Types
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/Types/fRect.o src/Lib/Types/fRect.cpp

${OBJECTDIR}/src/NetPanzer/Particles/CloudParticle2D.o: src/NetPanzer/Particles/CloudParticle2D.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Particles
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Particles/CloudParticle2D.o src/NetPanzer/Particles/CloudParticle2D.cpp

${OBJECTDIR}/src/NetPanzer/Classes/EnemyRadarPowerUp.o: src/NetPanzer/Classes/EnemyRadarPowerUp.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/EnemyRadarPowerUp.o src/NetPanzer/Classes/EnemyRadarPowerUp.cpp

${OBJECTDIR}/src/NetPanzer/Classes/OpcodeDebugger.o: src/NetPanzer/Classes/OpcodeDebugger.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/OpcodeDebugger.o src/NetPanzer/Classes/OpcodeDebugger.cpp

${OBJECTDIR}/src/NetPanzer/System/DummySound.o: src/NetPanzer/System/DummySound.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/System
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/System/DummySound.o src/NetPanzer/System/DummySound.cpp

${OBJECTDIR}/src/Lib/lua/lfunc.o: src/Lib/lua/lfunc.c 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/lua
	$(COMPILE.c) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/lua/lfunc.o src/Lib/lua/lfunc.c

${OBJECTDIR}/src/NetPanzer/Classes/Outpost.o: src/NetPanzer/Classes/Outpost.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/Outpost.o src/NetPanzer/Classes/Outpost.cpp

${OBJECTDIR}/src/NetPanzer/Classes/Weapons/BulletWeapon.o: src/NetPanzer/Classes/Weapons/BulletWeapon.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes/Weapons
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/Weapons/BulletWeapon.o src/NetPanzer/Classes/Weapons/BulletWeapon.cpp

${OBJECTDIR}/src/NetPanzer/Classes/UnitPowerUp.o: src/NetPanzer/Classes/UnitPowerUp.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/UnitPowerUp.o src/NetPanzer/Classes/UnitPowerUp.cpp

${OBJECTDIR}/src/NetPanzer/Interfaces/UnitInterface.o: src/NetPanzer/Interfaces/UnitInterface.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Interfaces
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Interfaces/UnitInterface.o src/NetPanzer/Interfaces/UnitInterface.cpp

${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi/UnitColorView.o: src/NetPanzer/Views/MainMenu/Multi/UnitColorView.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi/UnitColorView.o src/NetPanzer/Views/MainMenu/Multi/UnitColorView.cpp

${OBJECTDIR}/src/NetPanzer/Particles/SmolderParticleSystem2D.o: src/NetPanzer/Particles/SmolderParticleSystem2D.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Particles
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Particles/SmolderParticleSystem2D.o src/NetPanzer/Particles/SmolderParticleSystem2D.cpp

${OBJECTDIR}/src/NetPanzer/Views/Game/LibView.o: src/NetPanzer/Views/Game/LibView.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Views/Game
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Views/Game/LibView.o src/NetPanzer/Views/Game/LibView.cpp

${OBJECTDIR}/src/NetPanzer/Views/Components/CheckBox.o: src/NetPanzer/Views/Components/CheckBox.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Views/Components
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Views/Components/CheckBox.o src/NetPanzer/Views/Components/CheckBox.cpp

${OBJECTDIR}/src/Lib/INIParser/Store.o: src/Lib/INIParser/Store.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/INIParser
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/INIParser/Store.o src/Lib/INIParser/Store.cpp

${OBJECTDIR}/src/NetPanzer/Classes/UnitOpcodeEncoder.o: src/NetPanzer/Classes/UnitOpcodeEncoder.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/UnitOpcodeEncoder.o src/NetPanzer/Classes/UnitOpcodeEncoder.cpp

${OBJECTDIR}/src/Lib/physfs/physfs_byteorder.o: src/Lib/physfs/physfs_byteorder.c 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/physfs
	$(COMPILE.c) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/physfs/physfs_byteorder.o src/Lib/physfs/physfs_byteorder.c

${OBJECTDIR}/src/Lib/physfs/physfs_unicode.o: src/Lib/physfs/physfs_unicode.c 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/physfs
	$(COMPILE.c) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/physfs/physfs_unicode.o src/Lib/physfs/physfs_unicode.c

${OBJECTDIR}/src/NetPanzer/Particles/GroundExplosionParticleSystem2D.o: src/NetPanzer/Particles/GroundExplosionParticleSystem2D.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Particles
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Particles/GroundExplosionParticleSystem2D.o src/NetPanzer/Particles/GroundExplosionParticleSystem2D.cpp

${OBJECTDIR}/src/NetPanzer/Views/Game/RankView.o: src/NetPanzer/Views/Game/RankView.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Views/Game
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Views/Game/RankView.o src/NetPanzer/Views/Game/RankView.cpp

${OBJECTDIR}/src/NetPanzer/Classes/Network/NetMessageEncoder.o: src/NetPanzer/Classes/Network/NetMessageEncoder.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes/Network
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/Network/NetMessageEncoder.o src/NetPanzer/Classes/Network/NetMessageEncoder.cpp

${OBJECTDIR}/src/Lib/physfs/zlib123/compress.o: src/Lib/physfs/zlib123/compress.c 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/physfs/zlib123
	$(COMPILE.c) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/physfs/zlib123/compress.o src/Lib/physfs/zlib123/compress.c

${OBJECTDIR}/src/NetPanzer/Particles/FlameParticle2D.o: src/NetPanzer/Particles/FlameParticle2D.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Particles
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Particles/FlameParticle2D.o src/NetPanzer/Particles/FlameParticle2D.cpp

${OBJECTDIR}/src/Lib/lua/lparser.o: src/Lib/lua/lparser.c 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/lua
	$(COMPILE.c) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/lua/lparser.o src/Lib/lua/lparser.c

${OBJECTDIR}/src/Lib/lua/linit.o: src/Lib/lua/linit.c 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/lua
	$(COMPILE.c) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/lua/linit.o src/Lib/lua/linit.c

${OBJECTDIR}/src/NetPanzer/Particles/DirtKickParticle2D.o: src/NetPanzer/Particles/DirtKickParticle2D.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Particles
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Particles/DirtKickParticle2D.o src/NetPanzer/Particles/DirtKickParticle2D.cpp

${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi/FlagSelectionView.o: src/NetPanzer/Views/MainMenu/Multi/FlagSelectionView.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi/FlagSelectionView.o src/NetPanzer/Views/MainMenu/Multi/FlagSelectionView.cpp

${OBJECTDIR}/src/Lib/lua/lstrlib.o: src/Lib/lua/lstrlib.c 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/lua
	$(COMPILE.c) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/lua/lstrlib.o src/Lib/lua/lstrlib.c

${OBJECTDIR}/src/Lib/2D/ColorTable.o: src/Lib/2D/ColorTable.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/2D
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/2D/ColorTable.o src/Lib/2D/ColorTable.cpp

${OBJECTDIR}/src/Lib/physfs/zlib123/inftrees.o: src/Lib/physfs/zlib123/inftrees.c 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/physfs/zlib123
	$(COMPILE.c) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/physfs/zlib123/inftrees.o src/Lib/physfs/zlib123/inftrees.c

${OBJECTDIR}/src/NetPanzer/Interfaces/ObjectiveInterface.o: src/NetPanzer/Interfaces/ObjectiveInterface.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Interfaces
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Interfaces/ObjectiveInterface.o src/NetPanzer/Interfaces/ObjectiveInterface.cpp

${OBJECTDIR}/src/NetPanzer/Views/Game/DisconectedView.o: src/NetPanzer/Views/Game/DisconectedView.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Views/Game
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Views/Game/DisconectedView.o src/NetPanzer/Views/Game/DisconectedView.cpp

${OBJECTDIR}/src/Lib/Types/Angle.o: src/Lib/Types/Angle.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/Types
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/Types/Angle.o src/Lib/Types/Angle.cpp

${OBJECTDIR}/src/Lib/physfs/platform/posix.o: src/Lib/physfs/platform/posix.c 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/physfs/platform
	$(COMPILE.c) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/physfs/platform/posix.o src/Lib/physfs/platform/posix.c

${OBJECTDIR}/src/Lib/physfs/zlib123/infback.o: src/Lib/physfs/zlib123/infback.c 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/physfs/zlib123
	$(COMPILE.c) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/physfs/zlib123/infback.o src/Lib/physfs/zlib123/infback.c

${OBJECTDIR}/src/NetPanzer/Views/Components/Desktop.o: src/NetPanzer/Views/Components/Desktop.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Views/Components
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Views/Components/Desktop.o src/NetPanzer/Views/Components/Desktop.cpp

${OBJECTDIR}/src/NetPanzer/Classes/ViewCamera.o: src/NetPanzer/Classes/ViewCamera.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/ViewCamera.o src/NetPanzer/Classes/ViewCamera.cpp

${OBJECTDIR}/src/NetPanzer/Views/MainMenu/RMouseHackView.o: src/NetPanzer/Views/MainMenu/RMouseHackView.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Views/MainMenu
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Views/MainMenu/RMouseHackView.o src/NetPanzer/Views/MainMenu/RMouseHackView.cpp

${OBJECTDIR}/src/Lib/lua/lauxlib.o: src/Lib/lua/lauxlib.c 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/lua
	$(COMPILE.c) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/lua/lauxlib.o src/Lib/lua/lauxlib.c

${OBJECTDIR}/src/Lib/physfs/platform/windows.o: src/Lib/physfs/platform/windows.c 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/physfs/platform
	$(COMPILE.c) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/physfs/platform/windows.o src/Lib/physfs/platform/windows.c

${OBJECTDIR}/src/NetPanzer/Interfaces/ConsoleInterface.o: src/NetPanzer/Interfaces/ConsoleInterface.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Interfaces
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Interfaces/ConsoleInterface.o src/NetPanzer/Interfaces/ConsoleInterface.cpp

${OBJECTDIR}/src/NetPanzer/Interfaces/BotGameManager.o: src/NetPanzer/Interfaces/BotGameManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Interfaces
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Interfaces/BotGameManager.o src/NetPanzer/Interfaces/BotGameManager.cpp

${OBJECTDIR}/src/NetPanzer/Views/Game/GameInfoView.o: src/NetPanzer/Views/Game/GameInfoView.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Views/Game
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Views/Game/GameInfoView.o src/NetPanzer/Views/Game/GameInfoView.cpp

${OBJECTDIR}/src/Lib/physfs/zlib123/uncompr.o: src/Lib/physfs/zlib123/uncompr.c 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/physfs/zlib123
	$(COMPILE.c) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/physfs/zlib123/uncompr.o src/Lib/physfs/zlib123/uncompr.c

${OBJECTDIR}/src/Lib/lua/lcode.o: src/Lib/lua/lcode.c 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/lua
	$(COMPILE.c) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/lua/lcode.o src/Lib/lua/lcode.c

${OBJECTDIR}/src/Lib/2D/DigitText.o: src/Lib/2D/DigitText.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/2D
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/2D/DigitText.o src/Lib/2D/DigitText.cpp

${OBJECTDIR}/src/NetPanzer/Particles/PuffParticle2D.o: src/NetPanzer/Particles/PuffParticle2D.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Particles
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Particles/PuffParticle2D.o src/NetPanzer/Particles/PuffParticle2D.cpp

${OBJECTDIR}/src/NetPanzer/Views/MainMenu/OrderingView.o: src/NetPanzer/Views/MainMenu/OrderingView.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Views/MainMenu
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Views/MainMenu/OrderingView.o src/NetPanzer/Views/MainMenu/OrderingView.cpp

${OBJECTDIR}/src/Lib/Types/fXYZ.o: src/Lib/Types/fXYZ.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/Types
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/Types/fXYZ.o src/Lib/Types/fXYZ.cpp

${OBJECTDIR}/src/NetPanzer/Views/Game/AreYouSureExitView.o: src/NetPanzer/Views/Game/AreYouSureExitView.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Views/Game
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Views/Game/AreYouSureExitView.o src/NetPanzer/Views/Game/AreYouSureExitView.cpp

${OBJECTDIR}/src/NetPanzer/Classes/PowerUp.o: src/NetPanzer/Classes/PowerUp.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/PowerUp.o src/NetPanzer/Classes/PowerUp.cpp

${OBJECTDIR}/src/Lib/physfs/zlib123/gzio.o: src/Lib/physfs/zlib123/gzio.c 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/physfs/zlib123
	$(COMPILE.c) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/physfs/zlib123/gzio.o src/Lib/physfs/zlib123/gzio.c

${OBJECTDIR}/src/NetPanzer/Classes/PlayerUnitConfig.o: src/NetPanzer/Classes/PlayerUnitConfig.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/PlayerUnitConfig.o src/NetPanzer/Classes/PlayerUnitConfig.cpp

${OBJECTDIR}/src/Lib/Types/iXY.o: src/Lib/Types/iXY.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/Types
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/Types/iXY.o src/Lib/Types/iXY.cpp

${OBJECTDIR}/src/NetPanzer/Interfaces/ChatInterface.o: src/NetPanzer/Interfaces/ChatInterface.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Interfaces
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Interfaces/ChatInterface.o src/NetPanzer/Interfaces/ChatInterface.cpp

${OBJECTDIR}/src/Lib/lua/lapi.o: src/Lib/lua/lapi.c 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/lua
	$(COMPILE.c) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/lua/lapi.o src/Lib/lua/lapi.c

${OBJECTDIR}/src/Lib/physfs/physfs.o: src/Lib/physfs/physfs.c 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/physfs
	$(COMPILE.c) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/physfs/physfs.o src/Lib/physfs/physfs.c

${OBJECTDIR}/src/NetPanzer/Views/MainMenu/HelpView.o: src/NetPanzer/Views/MainMenu/HelpView.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Views/MainMenu
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Views/MainMenu/HelpView.o src/NetPanzer/Views/MainMenu/HelpView.cpp

${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi/JoinView.o: src/NetPanzer/Views/MainMenu/Multi/JoinView.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi/JoinView.o src/NetPanzer/Views/MainMenu/Multi/JoinView.cpp

${OBJECTDIR}/src/NetPanzer/Network/ProxyServer.o: src/NetPanzer/Network/ProxyServer.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Network
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Network/ProxyServer.o src/NetPanzer/Network/ProxyServer.cpp

${OBJECTDIR}/src/NetPanzer/Classes/Network/ServerMessageRouter.o: src/NetPanzer/Classes/Network/ServerMessageRouter.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes/Network
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/Network/ServerMessageRouter.o src/NetPanzer/Classes/Network/ServerMessageRouter.cpp

${OBJECTDIR}/src/NetPanzer/Interfaces/Server.o: src/NetPanzer/Interfaces/Server.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Interfaces
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Interfaces/Server.o src/NetPanzer/Interfaces/Server.cpp

${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi/PlayerNameView.o: src/NetPanzer/Views/MainMenu/Multi/PlayerNameView.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi/PlayerNameView.o src/NetPanzer/Views/MainMenu/Multi/PlayerNameView.cpp

${OBJECTDIR}/src/NetPanzer/System/Sound.o: src/NetPanzer/System/Sound.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/System
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/System/Sound.o src/NetPanzer/System/Sound.cpp

${OBJECTDIR}/src/NetPanzer/Particles/FireParticleSystem2D.o: src/NetPanzer/Particles/FireParticleSystem2D.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Particles
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Particles/FireParticleSystem2D.o src/NetPanzer/Particles/FireParticleSystem2D.cpp

${OBJECTDIR}/src/Lib/lua/lzio.o: src/Lib/lua/lzio.c 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/lua
	$(COMPILE.c) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/lua/lzio.o src/Lib/lua/lzio.c

${OBJECTDIR}/src/NetPanzer/Views/Components/ScrollBar.o: src/NetPanzer/Views/Components/ScrollBar.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Views/Components
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Views/Components/ScrollBar.o src/NetPanzer/Views/Components/ScrollBar.cpp

${OBJECTDIR}/src/NetPanzer/Classes/Network/NetPacketDebugger.o: src/NetPanzer/Classes/Network/NetPacketDebugger.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes/Network
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/Network/NetPacketDebugger.o src/NetPanzer/Classes/Network/NetPacketDebugger.cpp

${OBJECTDIR}/src/NetPanzer/Classes/UnitBlackBoard.o: src/NetPanzer/Classes/UnitBlackBoard.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/UnitBlackBoard.o src/NetPanzer/Classes/UnitBlackBoard.cpp

${OBJECTDIR}/src/Lib/lua/print.o: src/Lib/lua/print.c 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/lua
	$(COMPILE.c) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/lua/print.o src/Lib/lua/print.c

${OBJECTDIR}/src/NetPanzer/Particles/ParticleSystem2D.o: src/NetPanzer/Particles/ParticleSystem2D.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Particles
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Particles/ParticleSystem2D.o src/NetPanzer/Particles/ParticleSystem2D.cpp

${OBJECTDIR}/src/NetPanzer/Classes/WorldMap.o: src/NetPanzer/Classes/WorldMap.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/WorldMap.o src/NetPanzer/Classes/WorldMap.cpp

${OBJECTDIR}/src/Lib/physfs/archivers/dir.o: src/Lib/physfs/archivers/dir.c 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/physfs/archivers
	$(COMPILE.c) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/physfs/archivers/dir.o src/Lib/physfs/archivers/dir.c

${OBJECTDIR}/src/Lib/lua/ldblib.o: src/Lib/lua/ldblib.c 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/lua
	$(COMPILE.c) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/lua/ldblib.o src/Lib/lua/ldblib.c

${OBJECTDIR}/src/NetPanzer/Views/MainMenu/MainMenuView.o: src/NetPanzer/Views/MainMenu/MainMenuView.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Views/MainMenu
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Views/MainMenu/MainMenuView.o src/NetPanzer/Views/MainMenu/MainMenuView.cpp

${OBJECTDIR}/src/NetPanzer/Classes/UnitState.o: src/NetPanzer/Classes/UnitState.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/UnitState.o src/NetPanzer/Classes/UnitState.cpp

${OBJECTDIR}/src/NetPanzer/Interfaces/PowerUpInterface.o: src/NetPanzer/Interfaces/PowerUpInterface.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Interfaces
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Interfaces/PowerUpInterface.o src/NetPanzer/Interfaces/PowerUpInterface.cpp

${OBJECTDIR}/src/NetPanzer/Views/MainMenu/SkirmishView.o: src/NetPanzer/Views/MainMenu/SkirmishView.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Views/MainMenu
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Views/MainMenu/SkirmishView.o src/NetPanzer/Views/MainMenu/SkirmishView.cpp

${OBJECTDIR}/src/Lib/physfs/zlib123/crc32.o: src/Lib/physfs/zlib123/crc32.c 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/physfs/zlib123
	$(COMPILE.c) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/physfs/zlib123/crc32.o src/Lib/physfs/zlib123/crc32.c

${OBJECTDIR}/src/NetPanzer/Interfaces/PlayerGameManager.o: src/NetPanzer/Interfaces/PlayerGameManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Interfaces
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Interfaces/PlayerGameManager.o src/NetPanzer/Interfaces/PlayerGameManager.cpp

${OBJECTDIR}/src/NetPanzer/Interfaces/GameConfig.o: src/NetPanzer/Interfaces/GameConfig.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Interfaces
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Interfaces/GameConfig.o src/NetPanzer/Interfaces/GameConfig.cpp

${OBJECTDIR}/src/Lib/Util/Math.o: src/Lib/Util/Math.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/Util
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/Util/Math.o src/Lib/Util/Math.cpp

${OBJECTDIR}/src/Lib/Util/FileSystem.o: src/Lib/Util/FileSystem.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/Util
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/Util/FileSystem.o src/Lib/Util/FileSystem.cpp

${OBJECTDIR}/src/NetPanzer/Particles/ParticleSystemGlobals.o: src/NetPanzer/Particles/ParticleSystemGlobals.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Particles
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Particles/ParticleSystemGlobals.o src/NetPanzer/Particles/ParticleSystemGlobals.cpp

${OBJECTDIR}/src/NetPanzer/Core/NetworkGlobals.o: src/NetPanzer/Core/NetworkGlobals.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Core
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Core/NetworkGlobals.o src/NetPanzer/Core/NetworkGlobals.cpp

${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi/HostOptionsView.o: src/NetPanzer/Views/MainMenu/Multi/HostOptionsView.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi/HostOptionsView.o src/NetPanzer/Views/MainMenu/Multi/HostOptionsView.cpp

${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi/MasterServer/ServerInfo.o: src/NetPanzer/Views/MainMenu/Multi/MasterServer/ServerInfo.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi/MasterServer
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi/MasterServer/ServerInfo.o src/NetPanzer/Views/MainMenu/Multi/MasterServer/ServerInfo.cpp

${OBJECTDIR}/src/Lib/Util/Exception.o: src/Lib/Util/Exception.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/Util
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/Util/Exception.o src/Lib/Util/Exception.cpp

${OBJECTDIR}/src/Lib/ArrayUtil/BoundBox.o: src/Lib/ArrayUtil/BoundBox.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/ArrayUtil
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/ArrayUtil/BoundBox.o src/Lib/ArrayUtil/BoundBox.cpp

${OBJECTDIR}/src/NetPanzer/Interfaces/ProjectileInterface.o: src/NetPanzer/Interfaces/ProjectileInterface.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Interfaces
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Interfaces/ProjectileInterface.o src/NetPanzer/Interfaces/ProjectileInterface.cpp

${OBJECTDIR}/src/NetPanzer/Interfaces/MouseInterface.o: src/NetPanzer/Interfaces/MouseInterface.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Interfaces
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Interfaces/MouseInterface.o src/NetPanzer/Interfaces/MouseInterface.cpp

${OBJECTDIR}/src/NetPanzer/Particles/VectorPuffParticle2D.o: src/NetPanzer/Particles/VectorPuffParticle2D.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Particles
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Particles/VectorPuffParticle2D.o src/NetPanzer/Particles/VectorPuffParticle2D.cpp

${OBJECTDIR}/src/NetPanzer/Classes/PlayerID.o: src/NetPanzer/Classes/PlayerID.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/PlayerID.o src/NetPanzer/Classes/PlayerID.cpp

${OBJECTDIR}/src/NetPanzer/Views/Game/MiniMapView.o: src/NetPanzer/Views/Game/MiniMapView.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Views/Game
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Views/Game/MiniMapView.o src/NetPanzer/Views/Game/MiniMapView.cpp

${OBJECTDIR}/src/NetPanzer/Interfaces/ConfigVariable.o: src/NetPanzer/Interfaces/ConfigVariable.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Interfaces
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Interfaces/ConfigVariable.o src/NetPanzer/Interfaces/ConfigVariable.cpp

${OBJECTDIR}/src/Lib/2D/PackedSurface.o: src/Lib/2D/PackedSurface.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/2D
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/2D/PackedSurface.o src/Lib/2D/PackedSurface.cpp

${OBJECTDIR}/src/NetPanzer/Classes/SelectionBoxSprite.o: src/NetPanzer/Classes/SelectionBoxSprite.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/SelectionBoxSprite.o src/NetPanzer/Classes/SelectionBoxSprite.cpp

${OBJECTDIR}/src/NetPanzer/Network/ClientSocket.o: src/NetPanzer/Network/ClientSocket.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Network
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Network/ClientSocket.o src/NetPanzer/Network/ClientSocket.cpp

${OBJECTDIR}/src/Lib/physfs/zlib123/adler32.o: src/Lib/physfs/zlib123/adler32.c 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/physfs/zlib123
	$(COMPILE.c) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/physfs/zlib123/adler32.o src/Lib/physfs/zlib123/adler32.c

${OBJECTDIR}/src/NetPanzer/Interfaces/GameControlRulesDaemon.o: src/NetPanzer/Interfaces/GameControlRulesDaemon.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Interfaces
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Interfaces/GameControlRulesDaemon.o src/NetPanzer/Interfaces/GameControlRulesDaemon.cpp

${OBJECTDIR}/src/Lib/physfs/platform/beos.o: src/Lib/physfs/platform/beos.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/physfs/platform
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/physfs/platform/beos.o src/Lib/physfs/platform/beos.cpp

${OBJECTDIR}/src/NetPanzer/Particles/CraterParticle2D.o: src/NetPanzer/Particles/CraterParticle2D.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Particles
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Particles/CraterParticle2D.o src/NetPanzer/Particles/CraterParticle2D.cpp

${OBJECTDIR}/src/NetPanzer/Views/Components/cInputField.o: src/NetPanzer/Views/Components/cInputField.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Views/Components
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Views/Components/cInputField.o src/NetPanzer/Views/Components/cInputField.cpp

${OBJECTDIR}/src/NetPanzer/Views/Components/Component.o: src/NetPanzer/Views/Components/Component.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Views/Components
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Views/Components/Component.o src/NetPanzer/Views/Components/Component.cpp

${OBJECTDIR}/src/NetPanzer/Classes/Network/UnitSync.o: src/NetPanzer/Classes/Network/UnitSync.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes/Network
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/Network/UnitSync.o src/NetPanzer/Classes/Network/UnitSync.cpp

${OBJECTDIR}/src/NetPanzer/Views/Components/Label.o: src/NetPanzer/Views/Components/Label.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Views/Components
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Views/Components/Label.o src/NetPanzer/Views/Components/Label.cpp

${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi/HostView.o: src/NetPanzer/Views/MainMenu/Multi/HostView.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi/HostView.o src/NetPanzer/Views/MainMenu/Multi/HostView.cpp

${OBJECTDIR}/src/Lib/Util/UtilInterface.o: src/Lib/Util/UtilInterface.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/Util
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/Util/UtilInterface.o src/Lib/Util/UtilInterface.cpp

${OBJECTDIR}/src/NetPanzer/Classes/SelectionList.o: src/NetPanzer/Classes/SelectionList.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/SelectionList.o src/NetPanzer/Classes/SelectionList.cpp

${OBJECTDIR}/src/NetPanzer/Interfaces/MiniMapInterface.o: src/NetPanzer/Interfaces/MiniMapInterface.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Interfaces
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Interfaces/MiniMapInterface.o src/NetPanzer/Interfaces/MiniMapInterface.cpp

${OBJECTDIR}/src/NetPanzer/Classes/TileEngine.o: src/NetPanzer/Classes/TileEngine.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/TileEngine.o src/NetPanzer/Classes/TileEngine.cpp

${OBJECTDIR}/src/Lib/physfs/zlib123/trees.o: src/Lib/physfs/zlib123/trees.c 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/physfs/zlib123
	$(COMPILE.c) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/physfs/zlib123/trees.o src/Lib/physfs/zlib123/trees.c

${OBJECTDIR}/src/NetPanzer/Particles/SnowParticleSystem2D.o: src/NetPanzer/Particles/SnowParticleSystem2D.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Particles
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Particles/SnowParticleSystem2D.o src/NetPanzer/Particles/SnowParticleSystem2D.cpp

${OBJECTDIR}/src/NetPanzer/Interfaces/Client.o: src/NetPanzer/Interfaces/Client.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Interfaces
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Interfaces/Client.o src/NetPanzer/Interfaces/Client.cpp

${OBJECTDIR}/src/Lib/lua/lua.o: src/Lib/lua/lua.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/lua
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/lua/lua.o src/Lib/lua/lua.cpp

${OBJECTDIR}/src/NetPanzer/Classes/BonusUnitPowerUp.o: src/NetPanzer/Classes/BonusUnitPowerUp.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/BonusUnitPowerUp.o src/NetPanzer/Classes/BonusUnitPowerUp.cpp

${OBJECTDIR}/src/NetPanzer/Particles/SparkParticle2D.o: src/NetPanzer/Particles/SparkParticle2D.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Particles
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Particles/SparkParticle2D.o src/NetPanzer/Particles/SparkParticle2D.cpp

${OBJECTDIR}/src/Lib/physfs/zlib123/zutil.o: src/Lib/physfs/zlib123/zutil.c 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/physfs/zlib123
	$(COMPILE.c) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/physfs/zlib123/zutil.o src/Lib/physfs/zlib123/zutil.c

${OBJECTDIR}/src/NetPanzer/Bot/Bot.o: src/NetPanzer/Bot/Bot.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Bot
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Bot/Bot.o src/NetPanzer/Bot/Bot.cpp

${OBJECTDIR}/src/Lib/lua/lmem.o: src/Lib/lua/lmem.c 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/lua
	$(COMPILE.c) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/lua/lmem.o src/Lib/lua/lmem.c

${OBJECTDIR}/src/Lib/lua/liolib.o: src/Lib/lua/liolib.c 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/lua
	$(COMPILE.c) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/lua/liolib.o src/Lib/lua/liolib.c

${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Options/VisualsView.o: src/NetPanzer/Views/MainMenu/Options/VisualsView.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Options
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Options/VisualsView.o src/NetPanzer/Views/MainMenu/Options/VisualsView.cpp

${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi/ServerListView.o: src/NetPanzer/Views/MainMenu/Multi/ServerListView.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi/ServerListView.o src/NetPanzer/Views/MainMenu/Multi/ServerListView.cpp

${OBJECTDIR}/src/NetPanzer/Interfaces/unix/NetworkClientUnix.o: src/NetPanzer/Interfaces/unix/NetworkClientUnix.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Interfaces/unix
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Interfaces/unix/NetworkClientUnix.o src/NetPanzer/Interfaces/unix/NetworkClientUnix.cpp

${OBJECTDIR}/src/NetPanzer/Classes/AI/PathList.o: src/NetPanzer/Classes/AI/PathList.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes/AI
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/AI/PathList.o src/NetPanzer/Classes/AI/PathList.cpp

${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Options/ControlsView.o: src/NetPanzer/Views/MainMenu/Options/ControlsView.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Options
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Options/ControlsView.o src/NetPanzer/Views/MainMenu/Options/ControlsView.cpp

${OBJECTDIR}/src/Lib/lua/ldump.o: src/Lib/lua/ldump.c 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/lua
	$(COMPILE.c) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/lua/ldump.o src/Lib/lua/ldump.c

${OBJECTDIR}/src/NetPanzer/Classes/Weapons/MissleWeapon.o: src/NetPanzer/Classes/Weapons/MissleWeapon.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes/Weapons
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/Weapons/MissleWeapon.o src/NetPanzer/Classes/Weapons/MissleWeapon.cpp

${OBJECTDIR}/src/NetPanzer/Classes/Sprite.o: src/NetPanzer/Classes/Sprite.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/Sprite.o src/NetPanzer/Classes/Sprite.cpp

${OBJECTDIR}/src/NetPanzer/Particles/SnowParticle2D.o: src/NetPanzer/Particles/SnowParticle2D.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Particles
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Particles/SnowParticle2D.o src/NetPanzer/Particles/SnowParticle2D.cpp

${OBJECTDIR}/src/NetPanzer/Classes/Units/UnitGlobals.o: src/NetPanzer/Classes/Units/UnitGlobals.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes/Units
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/Units/UnitGlobals.o src/NetPanzer/Classes/Units/UnitGlobals.cpp

${OBJECTDIR}/src/NetPanzer/Views/Components/Button.o: src/NetPanzer/Views/Components/Button.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Views/Components
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Views/Components/Button.o src/NetPanzer/Views/Components/Button.cpp

${OBJECTDIR}/src/Lib/Types/iRect.o: src/Lib/Types/iRect.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/Types
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/Types/iRect.o src/Lib/Types/iRect.cpp

${OBJECTDIR}/src/Lib/lua/lundump.o: src/Lib/lua/lundump.c 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/lua
	$(COMPILE.c) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/lua/lundump.o src/Lib/lua/lundump.c

${OBJECTDIR}/src/Lib/Util/TimerInterface.o: src/Lib/Util/TimerInterface.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/Util
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/Util/TimerInterface.o src/Lib/Util/TimerInterface.cpp

${OBJECTDIR}/src/Lib/physfs/zlib123/inffast.o: src/Lib/physfs/zlib123/inffast.c 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/physfs/zlib123
	$(COMPILE.c) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/physfs/zlib123/inffast.o src/Lib/physfs/zlib123/inffast.c

${OBJECTDIR}/src/NetPanzer/Interfaces/WorldViewInterface.o: src/NetPanzer/Interfaces/WorldViewInterface.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Interfaces
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Interfaces/WorldViewInterface.o src/NetPanzer/Interfaces/WorldViewInterface.cpp

${OBJECTDIR}/src/NetPanzer/Particles/Physics.o: src/NetPanzer/Particles/Physics.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Particles
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Particles/Physics.o src/NetPanzer/Particles/Physics.cpp

${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Options/SoundView.o: src/NetPanzer/Views/MainMenu/Options/SoundView.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Options
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Options/SoundView.o src/NetPanzer/Views/MainMenu/Options/SoundView.cpp

${OBJECTDIR}/src/Lib/Network/TCPListenSocket.o: src/Lib/Network/TCPListenSocket.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/Network
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/Network/TCPListenSocket.o src/Lib/Network/TCPListenSocket.cpp

${OBJECTDIR}/src/Lib/Network/TCPSocket.o: src/Lib/Network/TCPSocket.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/Network
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/Network/TCPSocket.o src/Lib/Network/TCPSocket.cpp

${OBJECTDIR}/src/NetPanzer/Views/Game/GameTemplateView.o: src/NetPanzer/Views/Game/GameTemplateView.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Views/Game
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Views/Game/GameTemplateView.o src/NetPanzer/Views/Game/GameTemplateView.cpp

${OBJECTDIR}/src/NetPanzer/Interfaces/GameManager.o: src/NetPanzer/Interfaces/GameManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Interfaces
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Interfaces/GameManager.o src/NetPanzer/Interfaces/GameManager.cpp

${OBJECTDIR}/src/NetPanzer/Bot/BotTaskList.o: src/NetPanzer/Bot/BotTaskList.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Bot
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Bot/BotTaskList.o src/NetPanzer/Bot/BotTaskList.cpp

${OBJECTDIR}/src/Lib/Util/Timer.o: src/Lib/Util/Timer.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/Util
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/Util/Timer.o src/Lib/Util/Timer.cpp

${OBJECTDIR}/src/NetPanzer/Views/Game/CodeStatsView.o: src/NetPanzer/Views/Game/CodeStatsView.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Views/Game
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Views/Game/CodeStatsView.o src/NetPanzer/Views/Game/CodeStatsView.cpp

${OBJECTDIR}/src/NetPanzer/Network/ServerSocket.o: src/NetPanzer/Network/ServerSocket.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Network
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Network/ServerSocket.o src/NetPanzer/Network/ServerSocket.cpp

${OBJECTDIR}/src/Lib/physfs/platform/pocketpc.o: src/Lib/physfs/platform/pocketpc.c 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/physfs/platform
	$(COMPILE.c) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/physfs/platform/pocketpc.o src/Lib/physfs/platform/pocketpc.c

${OBJECTDIR}/src/Lib/lua/ltable.o: src/Lib/lua/ltable.c 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/lua
	$(COMPILE.c) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/lua/ltable.o src/Lib/lua/ltable.c

${OBJECTDIR}/src/NetPanzer/Particles/RadarPingParticle2D.o: src/NetPanzer/Particles/RadarPingParticle2D.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Particles
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Particles/RadarPingParticle2D.o src/NetPanzer/Particles/RadarPingParticle2D.cpp

${OBJECTDIR}/src/NetPanzer/Particles/Particle2D.o: src/NetPanzer/Particles/Particle2D.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Particles
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Particles/Particle2D.o src/NetPanzer/Particles/Particle2D.cpp

${OBJECTDIR}/src/NetPanzer/Interfaces/MapsManager.o: src/NetPanzer/Interfaces/MapsManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Interfaces
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Interfaces/MapsManager.o src/NetPanzer/Interfaces/MapsManager.cpp

${OBJECTDIR}/src/Lib/lua/lstring.o: src/Lib/lua/lstring.c 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/lua
	$(COMPILE.c) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/lua/lstring.o src/Lib/lua/lstring.c

${OBJECTDIR}/src/NetPanzer/System/SDLVideo.o: src/NetPanzer/System/SDLVideo.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/System
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/System/SDLVideo.o src/NetPanzer/System/SDLVideo.cpp

${OBJECTDIR}/src/Lib/lua/ldo.o: src/Lib/lua/ldo.c 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/lua
	$(COMPILE.c) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/lua/ldo.o src/Lib/lua/ldo.c

${OBJECTDIR}/src/NetPanzer/Views/MainMenu/OptionsTemplateView.o: src/NetPanzer/Views/MainMenu/OptionsTemplateView.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Views/MainMenu
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Views/MainMenu/OptionsTemplateView.o src/NetPanzer/Views/MainMenu/OptionsTemplateView.cpp

${OBJECTDIR}/src/Lib/optionmm/option.o: src/Lib/optionmm/option.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/optionmm
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/optionmm/option.o src/Lib/optionmm/option.cpp

${OBJECTDIR}/src/Lib/lua/lobject.o: src/Lib/lua/lobject.c 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/lua
	$(COMPILE.c) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/lua/lobject.o src/Lib/lua/lobject.c

${OBJECTDIR}/src/Lib/lua/llex.o: src/Lib/lua/llex.c 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/lua
	$(COMPILE.c) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/lua/llex.o src/Lib/lua/llex.c

${OBJECTDIR}/src/NetPanzer/Views/Game/ResignView.o: src/NetPanzer/Views/Game/ResignView.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Views/Game
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Views/Game/ResignView.o src/NetPanzer/Views/Game/ResignView.cpp

${OBJECTDIR}/src/Lib/Util/Log.o: src/Lib/Util/Log.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/Util
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/Util/Log.o src/Lib/Util/Log.cpp

${OBJECTDIR}/src/Lib/optionmm/command_line.o: src/Lib/optionmm/command_line.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/optionmm
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/optionmm/command_line.o src/Lib/optionmm/command_line.cpp

${OBJECTDIR}/src/NetPanzer/Views/MainMenu/MenuTemplateView.o: src/NetPanzer/Views/MainMenu/MenuTemplateView.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Views/MainMenu
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Views/MainMenu/MenuTemplateView.o src/NetPanzer/Views/MainMenu/MenuTemplateView.cpp

${OBJECTDIR}/src/NetPanzer/Views/Game/VehicleSelectionView.o: src/NetPanzer/Views/Game/VehicleSelectionView.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Views/Game
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Views/Game/VehicleSelectionView.o src/NetPanzer/Views/Game/VehicleSelectionView.cpp

${OBJECTDIR}/src/NetPanzer/Classes/UnitOpcodeDecoder.o: src/NetPanzer/Classes/UnitOpcodeDecoder.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/UnitOpcodeDecoder.o src/NetPanzer/Classes/UnitOpcodeDecoder.cpp

${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi/IPAddressView.o: src/NetPanzer/Views/MainMenu/Multi/IPAddressView.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi/IPAddressView.o src/NetPanzer/Views/MainMenu/Multi/IPAddressView.cpp

${OBJECTDIR}/src/Lib/physfs/zlib123/deflate.o: src/Lib/physfs/zlib123/deflate.c 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/physfs/zlib123
	$(COMPILE.c) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/physfs/zlib123/deflate.o src/Lib/physfs/zlib123/deflate.c

${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi/UnitSelectionView.o: src/NetPanzer/Views/MainMenu/Multi/UnitSelectionView.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Views/MainMenu/Multi/UnitSelectionView.o src/NetPanzer/Views/MainMenu/Multi/UnitSelectionView.cpp

${OBJECTDIR}/src/NetPanzer/Classes/Network/ClientConnectDaemon.o: src/NetPanzer/Classes/Network/ClientConnectDaemon.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes/Network
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/Network/ClientConnectDaemon.o src/NetPanzer/Classes/Network/ClientConnectDaemon.cpp

${OBJECTDIR}/src/Lib/lua/lbaselib.o: src/Lib/lua/lbaselib.c 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/lua
	$(COMPILE.c) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/lua/lbaselib.o src/Lib/lua/lbaselib.c

${OBJECTDIR}/src/NetPanzer/Views/Game/AreYouSureResignView.o: src/NetPanzer/Views/Game/AreYouSureResignView.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Views/Game
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Views/Game/AreYouSureResignView.o src/NetPanzer/Views/Game/AreYouSureResignView.cpp

${OBJECTDIR}/src/Lib/physfs/platform/macosx.o: src/Lib/physfs/platform/macosx.c 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/physfs/platform
	$(COMPILE.c) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/physfs/platform/macosx.o src/Lib/physfs/platform/macosx.c

${OBJECTDIR}/src/NetPanzer/Particles/FireParticle2D.o: src/NetPanzer/Particles/FireParticle2D.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Particles
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Particles/FireParticle2D.o src/NetPanzer/Particles/FireParticle2D.cpp

${OBJECTDIR}/src/NetPanzer/Classes/Network/NetPacketQueues.o: src/NetPanzer/Classes/Network/NetPacketQueues.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes/Network
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/Network/NetPacketQueues.o src/NetPanzer/Classes/Network/NetPacketQueues.cpp

${OBJECTDIR}/src/Lib/physfs/platform/os2.o: src/Lib/physfs/platform/os2.c 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/physfs/platform
	$(COMPILE.c) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/physfs/platform/os2.o src/Lib/physfs/platform/os2.c

${OBJECTDIR}/src/Lib/Network/SocketBase.o: src/Lib/Network/SocketBase.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/Network
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/Network/SocketBase.o src/Lib/Network/SocketBase.cpp

${OBJECTDIR}/src/NetPanzer/Interfaces/PathScheduler.o: src/NetPanzer/Interfaces/PathScheduler.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Interfaces
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Interfaces/PathScheduler.o src/NetPanzer/Interfaces/PathScheduler.cpp

${OBJECTDIR}/src/Lib/Network/UDPSocket.o: src/Lib/Network/UDPSocket.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/Network
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/Network/UDPSocket.o src/Lib/Network/UDPSocket.cpp

${OBJECTDIR}/src/NetPanzer/Classes/TipOfDay.o: src/NetPanzer/Classes/TipOfDay.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/TipOfDay.o src/NetPanzer/Classes/TipOfDay.cpp

${OBJECTDIR}/src/NetPanzer/Classes/Objective.o: src/NetPanzer/Classes/Objective.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/Objective.o src/NetPanzer/Classes/Objective.cpp

${OBJECTDIR}/src/NetPanzer/Particles/TemplateExplosionSystem.o: src/NetPanzer/Particles/TemplateExplosionSystem.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Particles
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Particles/TemplateExplosionSystem.o src/NetPanzer/Particles/TemplateExplosionSystem.cpp

${OBJECTDIR}/src/Lib/physfs/platform/unix.o: src/Lib/physfs/platform/unix.c 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/physfs/platform
	$(COMPILE.c) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/physfs/platform/unix.o src/Lib/physfs/platform/unix.c

${OBJECTDIR}/src/NetPanzer/Classes/SpawnList.o: src/NetPanzer/Classes/SpawnList.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/SpawnList.o src/NetPanzer/Classes/SpawnList.cpp

${OBJECTDIR}/src/Lib/lua/ldebug.o: src/Lib/lua/ldebug.c 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/lua
	$(COMPILE.c) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/lua/ldebug.o src/Lib/lua/ldebug.c

${OBJECTDIR}/src/Lib/Network/SocketManager.o: src/Lib/Network/SocketManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/Network
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/Network/SocketManager.o src/Lib/Network/SocketManager.cpp

${OBJECTDIR}/src/Lib/ArrayUtil/BitArray.o: src/Lib/ArrayUtil/BitArray.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/ArrayUtil
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/ArrayUtil/BitArray.o src/Lib/ArrayUtil/BitArray.cpp

${OBJECTDIR}/src/NetPanzer/Classes/AI/Astar.o: src/NetPanzer/Classes/AI/Astar.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes/AI
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/AI/Astar.o src/NetPanzer/Classes/AI/Astar.cpp

${OBJECTDIR}/src/Lib/INIParser/Section.o: src/Lib/INIParser/Section.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/INIParser
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/INIParser/Section.o src/Lib/INIParser/Section.cpp

${OBJECTDIR}/src/NetPanzer/Views/Components/View.o: src/NetPanzer/Views/Components/View.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Views/Components
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Views/Components/View.o src/NetPanzer/Views/Components/View.cpp

${OBJECTDIR}/src/NetPanzer/Classes/Weapons/Weapon.o: src/NetPanzer/Classes/Weapons/Weapon.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes/Weapons
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/Weapons/Weapon.o src/NetPanzer/Classes/Weapons/Weapon.cpp

${OBJECTDIR}/src/Lib/lua/loslib.o: src/Lib/lua/loslib.c 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/lua
	$(COMPILE.c) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/lua/loslib.o src/Lib/lua/loslib.c

${OBJECTDIR}/src/Lib/2D/RGBColor.o: src/Lib/2D/RGBColor.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/2D
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/2D/RGBColor.o src/Lib/2D/RGBColor.cpp

${OBJECTDIR}/src/NetPanzer/Classes/Network/ServerConnectDaemon.o: src/NetPanzer/Classes/Network/ServerConnectDaemon.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes/Network
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/Network/ServerConnectDaemon.o src/NetPanzer/Classes/Network/ServerConnectDaemon.cpp

${OBJECTDIR}/src/NetPanzer/Views/Game/GameView.o: src/NetPanzer/Views/Game/GameView.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Views/Game
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Views/Game/GameView.o src/NetPanzer/Views/Game/GameView.cpp

${OBJECTDIR}/src/NetPanzer/Views/Components/Choice.o: src/NetPanzer/Views/Components/Choice.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Views/Components
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Views/Components/Choice.o src/NetPanzer/Views/Components/Choice.cpp

${OBJECTDIR}/src/NetPanzer/Particles/ChunkTrajectoryParticle2D.o: src/NetPanzer/Particles/ChunkTrajectoryParticle2D.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Particles
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Particles/ChunkTrajectoryParticle2D.o src/NetPanzer/Particles/ChunkTrajectoryParticle2D.cpp

${OBJECTDIR}/src/NetPanzer/Classes/PlacementMatrix.o: src/NetPanzer/Classes/PlacementMatrix.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/PlacementMatrix.o src/NetPanzer/Classes/PlacementMatrix.cpp

${OBJECTDIR}/src/Lib/lua/ltablib.o: src/Lib/lua/ltablib.c 
	${MKDIR} -p ${OBJECTDIR}/src/Lib/lua
	$(COMPILE.c) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/Lib/lua/ltablib.o src/Lib/lua/ltablib.c

${OBJECTDIR}/src/NetPanzer/Views/GameViewGlobals.o: src/NetPanzer/Views/GameViewGlobals.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Views
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Views/GameViewGlobals.o src/NetPanzer/Views/GameViewGlobals.cpp

${OBJECTDIR}/src/NetPanzer/Classes/AI/PathingState.o: src/NetPanzer/Classes/AI/PathingState.cpp 
	${MKDIR} -p ${OBJECTDIR}/src/NetPanzer/Classes/AI
	$(COMPILE.cc) -g -Wall -DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -I. -Isrc/Lib -Isrc/NetPanzer -Isrc/Lib/physfs -Isrc/Lib/physfs/zlib123 -I/Library/Frameworks/SDL.framework/Versions/A/Headers -I/Library/Frameworks/SDL_mixer.framework/Versions/A/Headers -o ${OBJECTDIR}/src/NetPanzer/Classes/AI/PathingState.o src/NetPanzer/Classes/AI/PathingState.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/mac-debug
	${RM} ./netpanzer-debug

# Subprojects
.clean-subprojects:
