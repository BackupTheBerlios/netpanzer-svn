#!/bin/sh
# a little script for making a netpanzer release

VERSION=`autoconf -t AC_INIT | sed -e 's/[^:]*:[^:]*:[^:]*:[^:]*:\([^:]*\):.*/\1/g'`

SOURCERELEASE=release/netpanzer-$VERSION
DATARELEASE=release/netpanzer-data-$VERSION

# first clean the directories up
echo "*** Cleaning up"
if test ! -f configure; then
    ./autogen.sh
fi
if test ! -f Jamconfig; then
    ./configure
fi
rm -rf out
rm -rf release
jam distclean

echo "*** Scanning for files"
AUTOFILES="autogen.sh configure.ac configure config.h.in `find mk/autoconf/ -name "*.m4" -o -name "config.*"` mk/autoconf/install-sh"
JAMFILES="Jamrules Jamconfig.in `find mk/jam -name "*.jam"`"
ICONS="*.png *.xpm"
TEXTS="COPYING README TODO ChangeLog"
SOURCES="`find src -name "*.cpp" -o -name "*.hpp" -o -name "Jamfile"`"
DOCS="docs/*.[1-9] docs/Jamfile"

echo "*** Creating Sourcepackage"
mkdir -p $SOURCERELEASE
# Create a new Jamfile
cat > $SOURCERELEASE/Jamfile << '__END__'
SubDir TOP ;

SubInclude TOP src ;
SubInclude TOP docs ;

appicondir ?= [ ConcatDirs $(datadir) pixmaps $(PACKAGE_NAME) ] ;

##  InstallIcon files [ : subdir ]
##    Installs an icon
rule InstallIcon
{
    LOCATE on $(<:G=installicon) = $(SUBDIR) ;
    Depends install_data : [ DoInstall $(<:G=installicon) : $(appicondir) $(2) ] ;
}

InstallIcon netpanzer.png netpanzer.xpm ;
__END__

cp -p --parents $AUTOFILES $SOURCERELEASE
cp -p --parents $JAMFILES $SOURCERELEASE
cp -p --parents $TEXTS $SOURCERELEASE
cp -p --parents $SOURCES $SOURCERELEASE
cp -p --parents $ICONS $SOURCERELEASE
cp -p --parents $DOCS $SOURCERELEASE

echo "*** Packing source"
cd release
tar -c --bzip2 -f netpanzer-$VERSION.tar.bz2 netpanzer-$VERSION
cd -

echo "*** Creating data release"
mkdir -p $DATARELEASE
# Create a new Jamfile
cat > $DATARELEASE/Jamfile << __END__
SubDir TOP ;

SubInclude TOP pics ;
SubInclude TOP maps ;
SubInclude TOP sound ;
SubInclude TOP powerups ;
SubInclude TOP units ;
SubInclude TOP wads ;
SubInclude TOP fonts ;
__END__

find pics \( -name "*.bmp" -o -name "*.raw" -o -name "*.til" -o -name "*.pak" -o  -name "Jamfile" \) -exec cp -p --parents {} $DATARELEASE ';'
find wads \( -name "*.act" -o -name "*.tls" -o -name "Jamfile" \) -exec cp -p --parents {} $DATARELEASE ';'
find powerups \( -name "*.pak" -o -name "Jamfile" \) -exec cp -p --parents {} $DATARELEASE ';'
find units \( -name "*.pfl" -o -name "*.pak" -o -name "Jamfile" \) -exec cp -p --parents {} $DATARELEASE ';'
find sound \( -name "*.wav" -o -name "Jamfile" \) -exec cp -p --parents {} $DATARELEASE ';'
find maps \( -name "*.npm" -o -name "*.opt" -o -name "*.spn" -o -name "Jamfile" \) -exec cp -p --parents {} $DATARELEASE ';'
find fonts \( -name "*.pcf" \) -exec cp -p --parents {} $DATARELEASE ';'
cp -p --parents $AUTOFILES $DATARELEASE
cp -p --parents $JAMFILES $DATARELEASE
cp -p --parents $TEXTS $DATARELEASE

echo "*** Packing Data"
cd release
tar -c --bzip2 -f netpanzer-data-$VERSION.tar.bz2 netpanzer-data-$VERSION
cd -

