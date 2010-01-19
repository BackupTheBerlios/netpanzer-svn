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
    ('datadir','define the extra directory where the netpanzer will look for data files, usefull for linux distributions, defaults to no extra directory',''),
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

if not (env['mode'] in ['debug', 'release']):
    print "Error: mode can only be 'debug' or 'release', found: " + env['mode']
    Exit(1)

crossmingwenv = env.Clone();
crosslinuxenv = env.Clone();

if 'crossmingw' in COMMAND_LINE_TARGETS:
    if env['crossmingwcompilerprefix'] == '':
        print 'You need to set a compiler prefix for mingw cross compilation'
        Exit(1)

    if env['crossmingwsdlconfig'] == 'sdl-config':
        print 'You need to set the full path of sdl-config for mingw cross compilation'
        Exit(1)

    crossmingwenv = Environment( ENV = {'PATH' : os.environ['PATH']}, tools = ['mingw'], options=opts )
    crossmingwenv.Replace( CXX = env['crossmingwcompilerprefix'] + crossmingwenv['CXX'] )
    crossmingwenv.Replace( CC = env['crossmingwcompilerprefix'] + crossmingwenv['CC'] )
    crossmingwenv.Replace( AR = env['crossmingwcompilerprefix'] + crossmingwenv['AR'] )
    crossmingwenv.Replace( RANLIB = env['crossmingwcompilerprefix'] + crossmingwenv['RANLIB'] )
    crossmingwenv.Replace( RC = env['crossmingwcompilerprefix'] + crossmingwenv['RC'] )
    crossmingwenv.Append( CCFLAGS = [ '-Dsocklen_t=int' ] )
    crossmingwenv.Append( LDFLAGS = [ '-mwindows' ] )
    crossmingwenv.Append( LIBS = [ 'ws2_32', 'mingw32' ] )
    crossmingwenv['WINICON'] = crossmingwenv.RES( 'support/icon/npicon.rc' )
    crossmingwenv['PROGSUFFIX'] = '.exe'

if 'crosslinux' in COMMAND_LINE_TARGETS:
    if env['crosslinuxcompilerprefix'] == '':
        print 'You need to set a compiler prefix for linux cross compilation'
        Exit(1)

    if env['crosslinuxsdlconfig'] == 'sdl-config':
        print 'You need to set the full path of sdl-config for linux cross compilation'
        Exit(1)

    crosslinuxenv= Environment(ENV = {'PATH' : os.environ['PATH']},tools = ['gcc','g++','ar','gnulink'], options=opts )
    crosslinuxenv.Replace( CXX = env['crosslinuxcompilerprefix'] + env['CXX'] )
    crosslinuxenv.Replace( CC = env['crosslinuxcompilerprefix'] + env['CC'] )
    crosslinuxenv.Replace( AR = env['crosslinuxcompilerprefix'] + env['AR'] )
    crosslinuxenv.Replace( RANLIB = env['crosslinuxcompilerprefix'] + env['RANLIB'] )
    crosslinuxenv.Append( LINKFLAGS = [ '-static-libgcc' ] )
    #crosslinuxenv.Prepend( _LIBFLAGS = [ '-static-libgcc' ] )
    #crosslinuxenv.Prepend( _LIBFLAGS = [ '-lstdc++' ] )
    #crosslinuxenv.Append( _LIBFLAGS = [ '`' + crosslinuxenv['CXX'] + ' -print-file-name=libstdc++.a`' ] )
    #crosslinuxenv.Prepend( _LIBFLAGS = [ '/usr/local/gcc/i686-linux/lib/gcc/i686-linux/4.2.4/../../../../i686-linux/lib/libstdc++.a' ] )
    

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

env.Append(CCFLAGS = ['-Wall'])

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

NPVERSION = ''
SVERSION = ''

try:
    FILE = open('RELEASE_VERSION', 'r')
    NPVERSION = FILE.readline().strip(' \n\r')
    FILE.close()
except:
    pass

try:
    SVERSION = os.popen('svnversion').read()[:-1]
    SVERSION = SVERSION.split(':')[-1]
except:
    pass

print "NPVERSION = " + NPVERSION
print "SVERSION = " + SVERSION
if NPVERSION == '' and SVERSION != '':
    NPVERSION = 'svn-' + SVERSION;

env.Append( CCFLAGS = [ '-DPACKAGE_VERSION=\\"' + NPVERSION + '\\"' ] )
if env['datadir'] != '':
    env.Append( CCFLAGS = [ '-DNP_DATADIR=\\"' +  env['datadir'] + '\\"' ])
    
crossmingwenv.Append( CCFLAGS = [ '-DPACKAGE_VERSION=\\"' + NPVERSION + '\\"' ] )
crosslinuxenv.Append( CCFLAGS = [ '-DPACKAGE_VERSION=\\"' + NPVERSION + '\\"' ] )

thisplatform = sys.platform;
print 'Building version ' + NPVERSION + ' in ' + sys.platform

################################################################
# Configure Environments
################################################################

env.Append(           CPPPATH = [ '.', 'src/Lib', 'src/NetPanzer', 'src/lib/physfs', 'src/Lib/lua', 'src/Lib/toluapp'] )
crossmingwenv.Append( CPPPATH = [ '.', 'src/Lib', 'src/NetPanzer', 'src/lib/physfs', 'src/Lib/lua', 'src/Lib/toluapp'] )
crosslinuxenv.Append( CPPPATH = [ '.', 'src/Lib', 'src/NetPanzer', 'src/lib/physfs', 'src/Lib/lua', 'src/Lib/toluapp'] )

# for this platform
if thisplatform == 'darwin':
    env.Append( CPPPATH = ['/Library/Frameworks/SDL.framework/Headers',
                           '/Library/Frameworks/SDL_mixer.framework/Headers' ] )
#    env.Append( CCFLAGS = [ '-isysroot', '/Developer/SDKs/MacOSX10.4u.sdk', '-arch', 'ppc', '-arch', 'i386' ] )
#    luaenv.Append( CCFLAGS = [ '-isysroot', '/Developer/SDKs/MacOSX10.4u.sdk', '-arch', 'ppc', '-arch', 'i386' ] )
#    physfsenv.Append( CCFLAGS = [ '-isysroot', '/Developer/SDKs/MacOSX10.4u.sdk', '-arch', 'ppc', '-arch', 'i386' ] )
#    networkenv.Append( CCFLAGS = [ '-isysroot', '/Developer/SDKs/MacOSX10.4u.sdk', '-arch', 'ppc', '-arch', 'i386' ] )
#    env.Append( LINKFLAGS = [ '-mmacosx-version-min=10.4', '-Wl,-syslibroot,/Developer/SDKs/MacOSX10.4u.sdk', '-arch', 'ppc', '-arch', 'i386' ] )
    env.AppendUnique(FRAMEWORKS=Split('SDL SDL_mixer Cocoa IOKit'))
    env.Append( NPSOURCES =  ['support/macosx/SDLMain.m'] )
elif thisplatform == 'win32':
    env.Append( CPPPATH = [ 'C:/mingw/include/SDL' ] )
    env.Append( LIBS = [ 'ws2_32', 'mingw32', 'SDLMain', 'SDL' ] )
    env.Append( CCFLAGS = [ '-Dsocklen_t=int' ] )
    networkenv.Append( CCFLAGS = [ '-Dsocklen_t=int' ] )
    env.Append( _LIBFLAGS = [ '-mwindows' ] )
    env.Prepend( _LIBFLAGS = [ 'c:/mingw/lib/SDL_mixer.lib' ] )
    env['WINICON'] = env.RES( 'support/icon/npicon.rc' )
    SetupSpawn(env)
else:
    env.ParseConfig(env['sdlconfig'] + ' --cflags --libs')
    env.Append( NPLIBS = [ 'SDL_mixer' ] )

# for crossmingw platform
if 'crossmingw' in COMMAND_LINE_TARGETS:
    crossmingwenv.ParseConfig(env['crossmingwsdlconfig'] + ' --cflags --libs')
    crossmingwenv.Append( NPLIBS = [ 'SDL_mixer' ] )

# for crosslinux platform
if 'crosslinux' in COMMAND_LINE_TARGETS:
    crosslinuxenv.ParseConfig(env['crosslinuxsdlconfig'] + ' --cflags --libs')
    crosslinuxenv.Append( NPLIBS = [ 'SDL_mixer' ] )

################################################################
# Makes libs
################################################################

# BUILDS NETWORK
MakeStaticLib(          networkenv, 'npnetwork', 'Network', '*.cpp')
MakeStaticLib(crossmingwnetworkenv, 'npnetwork', 'Network', '*.cpp')
MakeStaticLib(crosslinuxnetworkenv, 'npnetwork', 'Network', '*.cpp')

# BUILDS LUA
luaenv.Append(           CPPPATH = [ 'src/Lib/lua', 'src/Lib/toluapp'] )
crossmingwluaenv.Append( CPPPATH = [ 'src/Lib/lua', 'src/Lib/toluapp'] )
crosslinuxluaenv.Append( CPPPATH = [ 'src/Lib/lua', 'src/Lib/toluapp'] )
MakeStaticLib(          luaenv, 'nplua', 'lua toluapp', '*.c')
MakeStaticLib(crossmingwluaenv, 'nplua', 'lua toluapp', '*.c')
MakeStaticLib(crosslinuxluaenv, 'nplua', 'lua toluapp', '*.c')

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
env.Append( CFLAGS = '-DZ_PREFIX=1' )
env.Append( CPPPATH = 'src/Lib/physfs/zlib123' )
MakeStaticLib(env, 'np2d', '2D 2D/libpng', '*.c*')
crossmingwenv.Append( CFLAGS = '-DZ_PREFIX=1' )
crossmingwenv.Append( CPPPATH = 'src/Lib/physfs/zlib123' )
MakeStaticLib(crossmingwenv, 'np2d', '2D 2D/libpng', '*.c*')
crosslinuxenv.Append( CFLAGS = '-DZ_PREFIX=1' )
crosslinuxenv.Append( CPPPATH = 'src/Lib/physfs/zlib123' )
MakeStaticLib(crosslinuxenv, 'np2d', '2D 2D/libpng', '*.c*')

# BUILDS REST OF LIBRARIES
MakeStaticLib(env, 'nplibs', 'ArrayUtil INIParser Types Util optionmm','*.cpp')
MakeStaticLib(crossmingwenv, 'nplibs', 'ArrayUtil INIParser Types Util optionmm','*.cpp')
MakeStaticLib(crosslinuxenv, 'nplibs', 'ArrayUtil INIParser Types Util optionmm','*.cpp')

################################################################
# NetPanzer source dirs
################################################################

npdirs = """
    Bot Classes Classes/AI Classes/Network Core Interfaces Network
    Objectives Particles PowerUps Resources Scripts System Units Weapons
    Views Views/Components Views/Game Views/MainMenu Views/MainMenu/Multi
    Views/MainMenu/Multi/MasterServer Views/MainMenu/Options
"""

env.Append( NPSOURCES = globSources(env, 'src/NetPanzer', npdirs, "*.cpp") )
if env.has_key('WINICON'):
    env.Append( NPSOURCES = env['WINICON'] )

crossmingwenv.Append( NPSOURCES = globSources(crossmingwenv, 'src/NetPanzer', npdirs, "*.cpp") )
if crossmingwenv.has_key('WINICON'):
    crossmingwenv.Append( NPSOURCES = crossmingwenv['WINICON'] )
    
crosslinuxenv.Append( NPSOURCES = globSources(crosslinuxenv, 'src/NetPanzer', npdirs, "*.cpp") )

env.Append(           NPLIBS = ['nplua','np2d','npnetwork','nplibs','npphysfs'] )
crossmingwenv.Append( NPLIBS = ['nplua','np2d','npnetwork','nplibs','npphysfs'] )
crosslinuxenv.Append( NPLIBS = ['nplua','np2d','npnetwork','nplibs','npphysfs'] )

if env.has_key('LIBS'):
    env.Append( NPLIBS = env['LIBS'] )

if crossmingwenv.has_key('LIBS'):
    crossmingwenv.Append( NPLIBS = crossmingwenv['LIBS'] )

if crosslinuxenv.has_key('LIBS'):
    crosslinuxenv.Append( NPLIBS = crosslinuxenv['LIBS'] )

env.Append(           NPLIBPATH =           env['FINALLIBSDIR'] )
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
