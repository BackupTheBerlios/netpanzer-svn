#! /bin/sh

gcc -o netpanzer ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Core/main.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Core/NetworkGlobals.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Port/blitters.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Port/Gdatstct.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/OpcodeDebugger.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/UnitPowerUp.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/ScreenSurface.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/Sprite.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/SelectionBoxSprite.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/SpriteSorter.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/UnitBlackBoard.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/WorldInputCmdProcessor.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/UnitState.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/SelectionList.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/TileSet.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/EnemyRadarPowerUp.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/Outpost.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/UnitMessage.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/PowerUp.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/SpawnList.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/PlayerUnitConfig.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/PlayerID.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/Objective.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/UnitBucketArray.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/PlayerState.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/Unit.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/PlacementMatrix.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/Projectile.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/BonusUnitPowerUp.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/UnitOpcodeEncoder.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/ViewCamera.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/TileEngine.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/WorldMap.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/AI/Astar.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/AI/PathingState.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/AI/PathList.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/Units/Lynx.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/Units/Leopard.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/Units/UnitGlobals.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/Units/M109.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/Units/Abrams.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/Units/Vehicle.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/Units/Archer.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/Units/Hammerhead.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/Units/Humvee.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/Units/SpahPanzer.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/Units/Scorpion.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/Units/Valentine.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/Weapons/Weapon.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/Weapons/BulletWeapon.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/Weapons/WeaponGlobals.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/Weapons/ShellWeapon.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/Weapons/MissleWeapon.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/Network/NetMessageEncoder.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/Network/ClientConnectDaemon.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/Network/NetworkInterface.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/Network/NetworkState.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/Network/ClientMessageRouter.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/Network/ServerConnectDaemon.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/Network/UnitSync.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/Network/NetPacketQueues.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/Network/ServerMessageRouter.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/Network/NetworkServer.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/Network/NetworkClient.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/Network/UnitNetMessage.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Classes/Network/NetPacketDebugger.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/IRCLobby/IRCLobby.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/IRCLobby/ProxyServer.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Network/ClientSocket.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Network/SocketClient.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Network/ClientList.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Network/SocketMessage.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Network/ServerSocket.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Interfaces/GameControlRulesDaemon.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Interfaces/ConsoleInterface.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Interfaces/MapsManager.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Interfaces/KeyboardInterface.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Interfaces/GameConfig.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Interfaces/ProjectileInterface.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Interfaces/MouseInterface.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Interfaces/TileInterface.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Interfaces/PlayerInterface.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Interfaces/MiniMapInterface.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Interfaces/ConfigVariable.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Interfaces/Stats.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Interfaces/PathScheduler.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Interfaces/Server.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Interfaces/PowerUpInterface.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Interfaces/Client.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Interfaces/ChatInterface.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Interfaces/UnitProfileInterface.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Interfaces/PlayerGameManager.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Interfaces/GameManager.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Interfaces/ObjectiveInterface.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Interfaces/DedicatedGameManager.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Interfaces/BotGameManager.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Interfaces/BaseGameManager.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Interfaces/UnitInterface.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Interfaces/MapInterface.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Interfaces/WorldViewInterface.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Interfaces/unix/NetworkServerUnix.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Interfaces/unix/NetworkClientUnix.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Bot/Bot.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Bot/BotTaskList.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Bot/BotPlayer.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/System/Sound.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/System/SDLVideo.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/System/DummySound.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/System/SDLSound.o ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Panels/TestPanel.o  ./out/i386-pc-linux-gnu/opt/src/Lib/Types/libnetpanzertypes.a ./out/i386-pc-linux-gnu/opt/src/Lib/Util/libnetpanzerutil.a ./out/i386-pc-linux-gnu/opt/src/Lib/Xml/libnetpanzerxml.a ./out/i386-pc-linux-gnu/opt/src/Lib/2D/libnetpanzer2d.a ./out/i386-pc-linux-gnu/opt/src/Lib/ArrayUtil/libnetpanzerarrayutil.a ./out/i386-pc-linux-gnu/opt/src/Lib/optionmm/liboptionmm.a ./out/i386-pc-linux-gnu/opt/src/Lib/UI/libnetpanzerui.a ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Particles/libnetpanzerparticles.a ./out/i386-pc-linux-gnu/opt/src/NetPanzer/Views/libnetpanzerviews.a  -static -Wl,-Bstatic -lresolv -Wl,-Bstatic -lphysfs -lz -Wl,-Bstatic -lxml2 -lglib -lz  -lSDL_net -Wl,-Bstatic -lSDL_mixer -lvorbisfile -lvorbis -logg -lsmpeg -Wl,-Bstatic -lSDL_ttf -lfreetype  -Wl,-Bstatic -lSDL -laudiofile -L/usr/X11R6/lib -lX11 -lXext -Wl,-Bstatic -ldl -Wl,-Bstatic -lstdc++ -lpthread -lpthread_nonshared -lc -lm

gcc -o netpanzerbrowser ./out/i386-pc-linux-gnu/opt/src/ServerBrowser/Main.o ./out/i386-pc-linux-gnu/opt/src/ServerBrowser/ServerList.o  ./out/i386-pc-linux-gnu/opt/src/Lib/Types/libnetpanzertypes.a ./out/i386-pc-linux-gnu/opt/src/Lib/Util/libnetpanzerutil.a ./out/i386-pc-linux-gnu/opt/src/Lib/IRC/libnetpanzerirc.a ./out/i386-pc-linux-gnu/opt/src/Lib/optionmm/liboptionmm.a  -static -Wl,-Bstatic -lphysfs -lz -static -Wl,-Bstatic -lphysfs -lz -lSDL_net -Wl,-Bstatic -lSDL -laudiofile -L/usr/X11R6/lib -lX11 -lXext -ldl -Wl,-Bstatic -lstdc++ -lpthread -lpthread_nonshared -lc -lm
