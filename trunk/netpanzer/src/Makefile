include config.mak

DIRS = . Lib Lib/2D Lib/Environment Lib/Interfaces Lib/Particles \
	   Lib/PObject/Classes Lib/PObject/Templates Lib/Types Lib/View \
	   Lib/optionmm Lib/Xml \
	   NetPanzer/Classes NetPanzer/Classes/Weapons \
	   NetPanzer/Classes/Network \
	   NetPanzer/Views/MainMenu NetPanzer/Views/MainMenu/Options \
	   NetPanzer/Views/MainMenu/Multi \
	   NetPanzer/Views NetPanzer/Views/Game NetPanzer/Port \
	   NetPanzer/Classes/AI NetPanzer/Classes/Units \
	   NetPanzer/Interfaces NetPanzer/Structs \
	   NetPanzer/Core NetPanzer/Resources \
	   NetPanzer/Interfaces/unix UILib/SDL UILib/Network \
	   UILib

LINKFLAGS += -g3
INCLUDEDIRS=$(DIRS) NetPanzer/Classes/Network
CFLAGS += -Wall -g3 -O2 $(foreach dir, $(INCLUDEDIRS), -I$(dir))

GOAL = ../netpanzer
LINKFLAGS += -g3 `sdl-config --libs` -lSDL_net -lSDL_mixer -lSDL_image \
	-lphysfs `xml2-config --libs`
CFLAGS += `sdl-config --cflags` `xml2-config --cflags` -D${SYSTEM}

ifeq ($(PROFILING),yes)
CFLAGS += -pg
LINKFLAGS += -pg
endif
	   
SOURCES = $(foreach dir, $(DIRS), $(wildcard $(dir)/*.cpp))
OBJECTS = $(SOURCES:.cpp=.o)

$(GOAL): $(OBJECTS)
		$(CXX) -o $(GOAL) $(OBJECTS) $(LINKFLAGS)

%.o: %.cpp
		$(CXX) $(CFLAGS) -c -o $@ $<

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

# DO NOT DELETE
