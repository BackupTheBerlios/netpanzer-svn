bin_PROGRAMS = masterserver
masterserver_SOURCES = Config.hpp Config.cpp Log.cpp Log.hpp main.cpp   \
                MasterServer.cpp MasterServer.hpp                       \
                RequestThread.cpp RequestThread.hpp ServerInfo.hpp      \
                SocketStream.hpp Tokenizer.hpp
masterserver_CXXFLAGS = -DLOCALSTATEDIR=\"${localstatedir}\"
masterserver_LDFLAGS = -pthread
