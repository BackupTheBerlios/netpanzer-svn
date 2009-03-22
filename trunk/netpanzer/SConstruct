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

finalbuildir = 'build/' + finalplatform + '/' + env['mode'] + '/'
finallibsdir = finalbuildir + 'libs/'
finalbindir = finalbuildir + 'bin/'

def GlobBuildFiles(builddir, pattern):
    sources = map(lambda x: builddir + '/' + x, glob.glob(pattern))
    return sources

env.VariantDir(finalbuildir,'.',duplicate=0)

env['CPPPATH'] = [ '.', 'src/Lib', 'src/NetPanzer' ]
env.Append(CPPPATH = ['src/Lib/physfs'])
env.Append(CPPPATH = ['/usr/local/include/SDL'])

# BUILDS 2D
env.StaticLibrary(finallibsdir + 'np2d',GlobBuildFiles(finalbuildir, 'src/Lib/2D/*.cpp'))

# BUILDS NETWORK
env.StaticLibrary(finallibsdir + 'npnetwork',GlobBuildFiles(finalbuildir, 'src/Lib/Network/*.cpp'))

# BUILDS LUA
env.StaticLibrary(finallibsdir + 'nplua',GlobBuildFiles(finalbuildir, 'src/Lib/lua/*.c'))

# BUILDS REST OF LIBRARIES
env.StaticLibrary(finallibsdir + 'nplibs',
                    GlobBuildFiles(finalbuildir, 'src/Lib/ArrayUtil/*.cpp')
                    + GlobBuildFiles(finalbuildir, 'src/Lib/INIParser/*.cpp')
                    + GlobBuildFiles(finalbuildir, 'src/Lib/Types/*.cpp')
                    + GlobBuildFiles(finalbuildir, 'src/Lib/Util/*.cpp')
                    + GlobBuildFiles(finalbuildir, 'src/Lib/optionmm/*.cpp')
                )

# BUILDS PHYSFS
physfsenv = env.Clone()
physfsenv['CFLAGS'] = '-DPHYSFS_SUPPORTS_ZIP=1 -DZ_PREFIX=1'
physfsenv.StaticLibrary(finallibsdir + 'npphysfs',
                    GlobBuildFiles(finalbuildir, 'src/Lib/physfs/*.c')
                    + GlobBuildFiles(finalbuildir, 'src/Lib/physfs/platform/*.c')
                    + GlobBuildFiles(finalbuildir, 'src/Lib/physfs/archivers/*.c')
                    + GlobBuildFiles(finalbuildir, 'src/Lib/physfs/zlib123/*.c')
                )

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

# Modified copy from BosWars
def globSources(sourcePrefix, sourceDirs, builddir, pattern):
    sources = []
    sourceDirs = Split(sourceDirs)
    for d in sourceDirs:
        sources.append(glob.glob( sourcePrefix + '/' + d + '/' + pattern))
    sources = Flatten(sources)
    targetsources = []
    for s in sources:
        targetsources.append(builddir + s)

    return targetsources

netpsources = globSources('src/NetPanzer', npdirs, finalbuildir, "*.cpp")
env.AppendUnique(FRAMEWORKS=Split('SDL SDL_mixer Cocoa IOKit'))
env.Program(    finalbindir + 'netpanzer',
                netpsources,
                LIBS=['nplua','npphysfs','np2d','npnetwork','nplibs', 'SDLmain'],
                LIBPATH=finallibsdir
            )