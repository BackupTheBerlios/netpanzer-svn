import os
import glob

opts = Options()
opts.AddOptions(
    ('mode','set compile mode to debug or release','release'),
    ('sdlconfig','sets the sdl-config full path', 'sdl-config'),
    BoolOption('crossmingw', 'Set to 1 for cross-compilation with mingw', 0)
)

env = Environment(options = opts)
Help(opts.GenerateHelpText(env))

if not (env['mode'] in ['debug', 'release']):
    print "Error: mode can only be 'debug' or 'release', found: " + env['mode']
    Exit(1)

# HANDLE 'mode' flags
if env['mode'] == 'debug':
    env.Append(CCFLAGS = ['-g', '-O0'])
    env['FINALEXENAME'] = 'netpanzer-debug'
else:
    env.Append(CCFLAGS = ['-O2','-s'])
    env['FINALEXENAME'] = 'netpanzer'

if env['crossmingw']:
    finalplatform = 'mingw'
else:
    finalplatform = env['PLATFORM']

env['FINALBUILDDIR'] = 'build/' + finalplatform + '/' + env['mode'] + '/'
env['FINALLIBSDIR'] = env['FINALBUILDDIR'] + 'libs/'

# Modified copy from BosWars of globSources
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

def MakeStaticLib(localenv, libname, libdirs, pattern):
    sources = globSources(localenv, 'src/Lib', libdirs, pattern)
    localenv.StaticLibrary( localenv['FINALLIBSDIR'] + libname, sources)

env.VariantDir(env['FINALBUILDDIR'],'.',duplicate=0)

luaenv = env.Clone()
physfsenv = env.Clone()
networkenv = env.Clone()

env.Append( CPPPATH = [ '.', 'src/Lib', 'src/NetPanzer' ] )
env.Append( CPPPATH = [ 'src/Lib/physfs' ] )

if finalplatform == 'darwin':
    env.Append(CPPPATH = ['/Library/Frameworks/SDL.framework/Headers',
                          '/Library/Frameworks/SDL_mixer.framework/Headers' ] )
    env.AppendUnique(FRAMEWORKS=Split('SDL SDL_mixer Cocoa IOKit'))
    env.Append( NPSOURCES =  ['support/macosx/SDLMain.m'] )
elif finalplatform == 'win32':
    print 'win32 not done'
    Exit(1)
else:
    env.ParseConfig(env['sdlconfig'] + ' --cflags --libs')
    env.Append( NPLIBS = [ 'SDL_mixer' ] )

# BUILDS NETWORK
MakeStaticLib(networkenv, 'npnetwork', 'Network', '*.cpp')

# BUILDS LUA
MakeStaticLib(luaenv, 'nplua', 'lua', '*.c')

# BUILDS PHYSFS
physfsenv.Append( CFLAGS = '-DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1 -DPHYSFS_NO_CDROM_SUPPORT=1' )
physfsenv.Append( CPPPATH = 'src/Lib/physfs' )
MakeStaticLib(physfsenv, 'npphysfs', 'physfs physfs/platform physfs/archivers physfs/zlib123', '*.c')

# BUILDS 2D
MakeStaticLib(env, 'np2d', '2D', '*.cpp')

# BUILDS REST OF LIBRARIES
MakeStaticLib(env, 'nplibs', 'ArrayUtil INIParser Types Util optionmm','*.cpp')


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
env.Append( NPLIBS = ['nplua','np2d','npnetwork','nplibs','npphysfs'] )
env.Append( NPLIBPATH = env['FINALLIBSDIR'] )

env.Program( env['FINALEXENAME'], env['NPSOURCES'],
             LIBS=env['NPLIBS'], LIBPATH=env['NPLIBPATH'] )
