SYSTEM=WIN32
#SYSTEM=UNIX

DIRS = . Lib Lib/2D Lib/Environment Lib/Interfaces Lib/Particles \
	   Lib/PObject/Classes Lib/PObject/Templates Lib/Types Lib/View \
	   NetPanzer/Classes NetPanzer/Classes/Weapons \
	   NetPanzer/Classes/Network \
	   NetPanzer/Views/MainMenu NetPanzer/Views/MainMenu/Options \
	   NetPanzer/Views/MainMenu/Multi \
	   NetPanzer/Views NetPanzer/Views/Game NetPanzer/Port \
	   NetPanzer/Classes/AI NetPanzer/Classes/Units \
	   NetPanzer/Interfaces NetPanzer/Structs \
	   NetPanzer/Core NetPanzer/Resources \
	   UILib

LINKFLAGS += -g3
INCLUDEDIRS=$(DIRS) NetPanzer/Classes/Network
CFLAGS += -Wall -g3 -O2 $(foreach dir, $(INCLUDEDIRS), -I$(dir))

ifeq ($(SYSTEM),UNIX)
DIRS += NetPanzer/Interfaces/unix Stub Win32/unix UILib/SDL \
	    NetPanzer/Core/unix
GOAL = ../netpanzer
LINKFLAGS += -g3 `sdl-config --libs` -lSDL_net
CFLAGS += `sdl-config --cflags` -DUNIX
endif
ifeq ($(SYSTEM),WIN32)
DIRS += NetPanzer/Interfaces/win32 DirectX/DDraw DirectX/DirectInput \
        DirectX/DirectPlay DirectX/DirectSound \
        Win32 Win32/Winsock
GOAL = ../netpanzer.exe
CFLAGS += -DWIN32 -Iw32api -D_DEBUG -Wno-unknown-pragmas
LINKFLAGS += -lgdi32 -lshell32 -lddraw -ldinput -ldplayx -ldsound -lws2_32 -ldsetup -lwinmm -ldxguid -lole32
endif
	   
SOURCES = $(foreach dir, $(DIRS), $(wildcard $(dir)/*.cpp))
OBJECTS = $(SOURCES:.cpp=.o)
# eventually disable if you don't have makedep installed (but you don't have
# dependency checking then)
#MAKEDEPEND=makedepend

$(GOAL): $(OBJECTS)
		g++ -o $(GOAL) $(OBJECTS) $(LINKFLAGS)

%.o: %.cpp
		g++ $(CFLAGS) -c -o $@ $<

ifneq (,$(MAKEDEPEND))
.depend: $(SOURCES)
		echo "" > $@
		$(MAKEDEPEND) -f $@ -- $(CFLAGS) -- $(SOURCES) >& /dev/null
-include .depend
endif

clean:
		rm -f $(OBJECTS)
		rm -f $(GOAL)
		rm -f .depend

