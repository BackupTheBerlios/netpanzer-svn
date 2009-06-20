import os
import glob
import sys
import string
import subprocess

################################################################
# Fix compiling with long lines in windows
################################################################
class ourSpawn:
    def ourspawn(self, sh, escape, cmd, args, env):
        newargs = string.join(args[1:], ' ')
        cmdline = cmd + " " + newargs
        startupinfo = subprocess.STARTUPINFO()
        startupinfo.dwFlags |= subprocess.STARTF_USESHOWWINDOW
        proc = subprocess.Popen(cmdline, stdin=subprocess.PIPE, stdout=subprocess.PIPE,
            stderr=subprocess.PIPE, startupinfo=startupinfo, shell = False, env = env)
        data, err = proc.communicate()
        rv = proc.wait()
        if rv:
            print "====="
            print err
            print "====="
        return rv

def SetupSpawn( env ):
    if sys.platform == 'win32':
        buf = ourSpawn()
        buf.ourenv = env
        env['SPAWN'] = buf.ourspawn

################################################################
# Modified copy from BosWars of globSources
################################################################

def globSources(localenv, sourcePrefix, sourceDirs, pattern):
    sources = []
    sourceDirs = Split(sourceDirs)
    for d in sourceDirs:
        sources.append(glob.glob( sourcePrefix + '/' + d + '/' + pattern))
    sources = Flatten(sources)
    targetsources = []
    for s in sources:
        targetsources.append(localenv['FINALBUILDDIR'] + s)

    return targetsources

################################################################
# Make static libraries
################################################################

def MakeStaticLib(localenv, libname, libdirs, pattern):
    sources = globSources(localenv, 'src/Lib', libdirs, pattern)
    localenv.StaticLibrary( localenv['FINALLIBSDIR'] + libname, sources)

################################################################
# Add options
################################################################

opts = Options()
opts.AddOptions(
    ('mode','set compile mode to debug or release','release'),
    ('sdlconfig','sets the sdl-config full path', 'sdl-config'),
    ('crosslinuxcompilerprefix', 'sets the prefix for the cross linux compiler, example: i686-pc-linux-gnu-', ''),
    ('crossmingwcompilerprefix', 'sets the prefix for the cross mingw compiler, example: i686-mingw32-', ''),
    ('crosslinuxsdlconfig','sets the sdl-config full path for linux cross compiler', 'sdl-config'),
    ('crossmingwsdlconfig','sets the sdl-config full path for mingw cross compiler', 'sdl-config')
)

env = Environment(ENV = os.environ, options = opts)
Help(opts.GenerateHelpText(env))

################################################################
# Create Environments
################################################################

crossmingwenv = env.Clone();
crosslinuxenv = env.Clone();

if 'crossmingw' in COMMAND_LINE_TARGETS:
    if env['crossmingwcompilerprefix'] == '':
        print 'You need to set a compiler prefix for mingw cross compilation'
        Exit(1)

    if env['crossmingwsdlconfig'] == 'sdl-config':
        print 'You need to set the full path of sdl-config for mingw cross compilation'
        Exit(1)

    #finalplatform = 'mingw'
    crossmingwenv = Environment(ENV = {'PATH' : os.environ['PATH']}, tools = ['mingw'], options=opts )
    print crossmingwenv
    crossmingwenv.Replace( CXX = env['crossmingwcompilerprefix'] + env['CXX'] )
    crossmingwenv.Replace( CC = env['crossmingwcompilerprefix'] + env['CC'] )
    crossmingwenv.Replace( AR = env['crossmingwcompilerprefix'] + env['AR'] )
    crossmingwenv.Replace( RANLIB = env['crossmingwcompilerprefix'] + env['RANLIB'] )
    crossmingwenv.Append( CCFLAGS = [ '-Dsocklen_t=int' ] )
    crossmingwenv.Append( LDFLAGS = [ '-mwindows' ] )
    crossmingwenv.Append( LIBS = [ 'ws2_32', 'mingw32' ] )
    crossmingwenv['PROGSUFFIX'] = '.exe'

if 'crosslinux' in COMMAND_LINE_TARGETS:
    if env['crosslinuxcompilerprefix'] == '':
        print 'You need to set a compiler prefix for linux cross compilation'
        Exit(1)

    if env['crosslinuxsdlconfig'] == 'sdl-config':
        print 'You need to set the full path of sdl-config for linux cross compilation'
        Exit(1)

    #finalplatform = 'linux'
    crosslinuxenv= Environment(ENV = {'PATH' : os.environ['PATH']},tools = ['gcc','g++','ar','gnulink'], options=opts )
    crosslinuxenv.Replace( CXX = env['crosslinuxcompilerprefix'] + env['CXX'] )
    crosslinuxenv.Replace( CC = env['crosslinuxcompilerprefix'] + env['CC'] )
    crosslinuxenv.Replace( AR = env['crosslinuxcompilerprefix'] + env['AR'] )
    crosslinuxenv.Replace( RANLIB = env['crosslinuxcompilerprefix'] + env['RANLIB'] )
    crosslinuxenv.Command('libstdc++.a', None, Action('ln -sf `%s -print-file-name=libstdc++.a` build/linux/' % crosslinuxenv['CXX'] ) )
    crosslinuxenv.Append( LIBS = [ 'libstdc++.a' ] )


if not (env['mode'] in ['debug', 'release']):
    print "Error: mode can only be 'debug' or 'release', found: " + env['mode']
    Exit(1)

env['FINALBUILDDIR'] = 'build/' + env['mode'] + '/'
env['FINALLIBSDIR'] = env['FINALBUILDDIR'] + 'libs/'
crossmingwenv['FINALBUILDDIR'] = 'build/crossmingw/' + env['mode'] + '/'
crossmingwenv['FINALLIBSDIR'] = crossmingwenv['FINALBUILDDIR'] + 'libs/'
crosslinuxenv['FINALBUILDDIR'] = 'build/crosslinux/' + env['mode'] + '/'
crosslinuxenv['FINALLIBSDIR'] = crosslinuxenv['FINALBUILDDIR'] + 'libs/'

# HANDLE 'mode' flags
if env['mode'] == 'debug':
    env.Append(CCFLAGS = ['-g', '-O0'])
    env['FINALEXENAME'] = 'netpanzer-debug'
    crossmingwenv.Append(CCFLAGS = ['-g', '-O0'])
    crossmingwenv['FINALEXENAME'] = crossmingwenv['FINALBUILDDIR'] + 'netpanzer-debug'
    crosslinuxenv.Append(CCFLAGS = ['-g', '-O0'])
    crosslinuxenv['FINALEXENAME'] = crosslinuxenv['FINALBUILDDIR'] + 'netpanzer-debug'
else:
    env.Append(CCFLAGS = ['-O2','-s'])
    env['FINALEXENAME'] = 'netpanzer'
    crossmingwenv.Append(CCFLAGS = ['-O2','-s'])
    crossmingwenv['FINALEXENAME'] = crossmingwenv['FINALBUILDDIR'] + 'netpanzer'
    crosslinuxenv.Append(CCFLAGS = ['-O2','-s'])
    crosslinuxenv['FINALEXENAME'] = crosslinuxenv['FINALBUILDDIR'] + 'netpanzer'


env.BuildDir(env['FINALBUILDDIR'],'.',duplicate=0)
crossmingwenv.BuildDir(crossmingwenv['FINALBUILDDIR'],'.',duplicate=0)
crosslinuxenv.BuildDir(crosslinuxenv['FINALBUILDDIR'],'.',duplicate=0)

luaenv = env.Clone()
physfsenv = env.Clone()
networkenv = env.Clone()

crossmingwluaenv = crossmingwenv.Clone()
crossmingwphysfsenv = crossmingwenv.Clone()
crossmingwnetworkenv = crossmingwenv.Clone()

crosslinuxluaenv = crosslinuxenv.Clone()
crosslinuxphysfsenv = crosslinuxenv.Clone()
crosslinuxnetworkenv = crosslinuxenv.Clone()

################################################################
# Set NetPanzer Version
################################################################

NPVERSION = 'svn'
SVERSION = ''
try:
    SVERSION = os.popen('svnversion').read()[:-1]
    SVERSION = SVERSION.split(':')[-1]
except e:
    pass

print "SVERSION = " + SVERSION

env.Append( CCFLAGS = [ '-DPACKAGE_VERSION=\\"' + NPVERSION + '\\"' ] )
crossmingwenv.Append( CCFLAGS = [ '-DPACKAGE_VERSION=\\"' + NPVERSION + '\\"' ] )
crosslinuxenv.Append( CCFLAGS = [ '-DPACKAGE_VERSION=\\"' + NPVERSION + '\\"' ] )

thisplatform = sys.platform;
print 'Building version ' + NPVERSION + ' in ' + sys.platform

################################################################
# Configure Environments
################################################################

env.Append( CPPPATH = [ '.', 'src/Lib', 'src/NetPanzer' , 'src/lib/physfs'] )
crossmingwenv.Append( CPPPATH = [ '.', 'src/Lib', 'src/NetPanzer' , 'src/lib/physfs'] )
crosslinuxenv.Append( CPPPATH = [ '.', 'src/Lib', 'src/NetPanzer' , 'src/lib/physfs'] )

# for this platform
if thisplatform == 'darwin':
    env.Append(CPPPATH = ['/Library/Frameworks/SDL.framework/Headers',
                          '/Library/Frameworks/SDL_mixer.framework/Headers' ] )
    env.AppendUnique(FRAMEWORKS=Split('SDL SDL_mixer Cocoa IOKit'))
    env.Append( NPSOURCES =  ['support/macosx/SDLMain.m'] )
elif thisplatform == 'win32':
    env.Append( CPPPATH = [ 'C:/mingw/include/SDL' ] )
    env.Append( LIBS = [ 'ws2_32', 'mingw32', 'SDLMain', 'SDL' ] )
    env.Append( CCFLAGS = [ '-Dsocklen_t=int' ] )
    networkenv.Append( CCFLAGS = [ '-Dsocklen_t=int' ] )
    env.Append( _LIBFLAGS = [ '-mwindows' ] )
    env.Prepend( _LIBFLAGS = [ 'c:/mingw/lib/SDL_mixer.lib' ] )
    SetupSpawn(env)
else:
    env.ParseConfig(env['sdlconfig'] + ' --cflags --libs')
    env.Append( NPLIBS = [ 'SDL_mixer' ] )

# for crossmingw platform
crossmingwenv.ParseConfig(env['crossmingwsdlconfig'] + ' --cflags --libs')
crossmingwenv.Append( NPLIBS = [ 'SDL_mixer' ] )

# for crosslinux platform
crosslinuxenv.ParseConfig(env['crosslinuxsdlconfig'] + ' --cflags --libs')
crosslinuxenv.Append( NPLIBS = [ 'SDL_mixer' ] )

################################################################
# Makes libs
################################################################

# BUILDS NETWORK
MakeStaticLib(networkenv, 'npnetwork', 'Network', '*.cpp')
MakeStaticLib(crossmingwnetworkenv, 'npnetwork', 'Network', '*.cpp')
MakeStaticLib(crosslinuxnetworkenv, 'npnetwork', 'Network', '*.cpp')

# BUILDS LUA
MakeStaticLib(luaenv, 'nplua', 'lua', '*.c')
MakeStaticLib(crossmingwluaenv, 'nplua', 'lua', '*.c')
MakeStaticLib(crosslinuxluaenv, 'nplua', 'lua', '*.c')

# BUILDS PHYSFS
physfsenv.Append( CFLAGS = '-DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -DPHYSFS_NO_CDROM_SUPPORT=1' )
physfsenv.Append( CPPPATH = [ 'src/Lib/physfs', 'src/Lib/physfs/zlib123' ] )
MakeStaticLib(physfsenv, 'npphysfs', 'physfs physfs/platform physfs/archivers physfs/zlib123', '*.c')

crossmingwphysfsenv.Append( CFLAGS = '-DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -DPHYSFS_NO_CDROM_SUPPORT=1' )
crossmingwphysfsenv.Append( CPPPATH = [ 'src/Lib/physfs', 'src/Lib/physfs/zlib123' ] )
MakeStaticLib(crossmingwphysfsenv, 'npphysfs', 'physfs physfs/platform physfs/archivers physfs/zlib123', '*.c')

crosslinuxphysfsenv.Append( CFLAGS = '-DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -DPHYSFS_NO_CDROM_SUPPORT=1' )
crosslinuxphysfsenv.Append( CPPPATH = [ 'src/Lib/physfs', 'src/Lib/physfs/zlib123' ] )
MakeStaticLib(crosslinuxphysfsenv, 'npphysfs', 'physfs physfs/platform physfs/archivers physfs/zlib123', '*.c')

# BUILDS 2D
MakeStaticLib(env, 'np2d', '2D', '*.cpp')
MakeStaticLib(crossmingwenv, 'np2d', '2D', '*.cpp')
MakeStaticLib(crosslinuxenv, 'np2d', '2D', '*.cpp')

# BUILDS REST OF LIBRARIES
MakeStaticLib(env, 'nplibs', 'ArrayUtil INIParser Types Util optionmm','*.cpp')
MakeStaticLib(crossmingwenv, 'nplibs', 'ArrayUtil INIParser Types Util optionmm','*.cpp')
MakeStaticLib(crosslinuxenv, 'nplibs', 'ArrayUtil INIParser Types Util optionmm','*.cpp')

################################################################
# NetPanzer source dirs
################################################################

npdirs = """
    Bot
    Classes
    Classes/AI
    Classes/Network
    Core
    Interfaces
    Network
    Objectives
    Particles
    PowerUps
    Resources
    Scripts
    System
    Units
    Weapons
    Views
    Views/Components
    Views/Game
    Views/MainMenu
    Views/MainMenu/Multi
    Views/MainMenu/Multi/MasterServer
    Views/MainMenu/Options
"""

env.Append( NPSOURCES = globSources(env, 'src/NetPanzer', npdirs, "*.cpp") )
crossmingwenv.Append( NPSOURCES = globSources(crossmingwenv, 'src/NetPanzer', npdirs, "*.cpp") )
crosslinuxenv.Append( NPSOURCES = globSources(crosslinuxenv, 'src/NetPanzer', npdirs, "*.cpp") )

env.Append( NPLIBS = ['nplua','np2d','npnetwork','nplibs','npphysfs'] )
crossmingwenv.Append( NPLIBS = ['nplua','np2d','npnetwork','nplibs','npphysfs'] )
crosslinuxenv.Append( NPLIBS = ['nplua','np2d','npnetwork','nplibs','npphysfs'] )

if env.has_key('LIBS'):
    env.Append( NPLIBS = env['LIBS'] )

if crossmingwenv.has_key('LIBS'):
    crossmingwenv.Append( NPLIBS = crossmingwenv['LIBS'] )

if crosslinuxenv.has_key('LIBS'):
    crosslinuxenv.Append( NPLIBS = crosslinuxenv['LIBS'] )

env.Append( NPLIBPATH = env['FINALLIBSDIR'] )
crossmingwenv.Append( NPLIBPATH = crossmingwenv['FINALLIBSDIR'] )
crosslinuxenv.Append( NPLIBPATH = crosslinuxenv['FINALLIBSDIR'] )

netpanzer = env.Program( env['FINALEXENAME'], env['NPSOURCES'],
             LIBS=env['NPLIBS'], LIBPATH=env['NPLIBPATH'] )

crossmingw = crossmingwenv.Program( crossmingwenv['FINALEXENAME'], crossmingwenv['NPSOURCES'],
             LIBS=crossmingwenv['NPLIBS'], LIBPATH=crossmingwenv['NPLIBPATH'] )

Alias('crossmingw', crossmingw);

crosslinux = crosslinuxenv.Program( crosslinuxenv['FINALEXENAME'], crosslinuxenv['NPSOURCES'],
             LIBS=crosslinuxenv['NPLIBS'], LIBPATH=crosslinuxenv['NPLIBPATH'] )

Alias('crosslinux', crosslinux);

Default(netpanzer)
