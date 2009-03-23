import os
import glob

opts = Options()
opts.AddOptions(
    ('mode','set compile mode to debug or release','release'),
    BoolOption('crossmingw', 'Set to 1 for cross-compilation with mingw', 0)
)

env = Environment(options = opts)
Help(opts.GenerateHelpText(env))

if not (env['mode'] in ['debug', 'release']):
    print "Error: mode can only be 'debug' or 'release', found: " + env['mode']
    Exit(1)

if env['crossmingw']:
    finalplatform = 'mingw'
else:
    finalplatform = env['PLATFORM']

env['FINALBUILDDIR'] = 'build/' + finalplatform + '/' + env['mode'] + '/'
env['FINALLIBSDIR'] = env['FINALBUILDDIR'] + 'libs/'

def GlobBuildFiles(localenv, pattern):
    sources = map(lambda x: localenv['FINALBUILDDIR'] + x, glob.glob(pattern))
    return sources

# Modified copy from BosWars
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

env['CPPPATH'] = [ '.', 'src/Lib', 'src/NetPanzer' ]
env.Append(CPPPATH = ['src/Lib/physfs'])
#env.Append(CPPPATH = ['/usr/local/include/SDL'])

# BUILDS 2D
MakeStaticLib(env, 'np2d', '2D', '*.cpp')

# BUILDS NETWORK
MakeStaticLib(env, 'npnetwork', 'Network', '*.cpp')

# BUILDS LUA
MakeStaticLib(env, 'nplua', 'lua', '*.c')

# BUILDS REST OF LIBRARIES
MakeStaticLib(env, 'nplibs', 'ArrayUtil INIParser Types Util optionmm','*.cpp')

# BUILDS PHYSFS
physfsenv = env.Clone()
physfsenv['CFLAGS'] = '-DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1'
MakeStaticLib(physfsenv, 'npphysfs', 'physfs physfs/platform physfs/archivers physfs/zlib123', '*.c')

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

netpsources = globSources(env, 'src/NetPanzer', npdirs, "*.cpp")
env.AppendUnique(FRAMEWORKS=Split('SDL SDL_mixer Cocoa IOKit'))
env.Program(    'netpanzer',
                netpsources,
                LIBS=['nplua','npphysfs','np2d','npnetwork','nplibs', 'SDLmain'],
                LIBPATH=env['FINALLIBSDIR']
            )