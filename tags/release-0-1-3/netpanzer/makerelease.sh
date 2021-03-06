#!/bin/sh
# a little script for making a netpanzer release

VERSION=`autoconf -t AC_INIT | sed -e 's/[^:]*:[^:]*:[^:]*:[^:]*:\([^:]*\):.*/\1/g'`

SOURCERELEASE=release/netpanzer-$VERSION
DATARELEASE=release/netpanzerdata-$VERSION

# first clean the directories up
echo "*** Cleaning up"
./autogen.sh
./configure
rm -rf out
rm -rf release
jam distclean

echo "*** Scanning for files"
AUTOFILES="autogen.sh configure.ac configure config.h.in `find mk/autoconf/ -name "*.m4" -o -name "config.*"` mk/autoconf/install-sh"
JAMFILES="Jamrules Jamconfig.in `find mk/jam -name "*.jam"`"
TEXTS="COPYING README TODO ChangeLog"

SOURCES="`find src -name "*.cpp" -o -name "*.hpp" -o -name "Jamfile"`"
PICS="`find pics -name "*.bmp" -o -name "*.raw" -o -name "*.til" -o -name "*.pak" -o -name "Jamfile"`"
WADS="`find wads -name "*.act" -o -name "*.tls" -o -name "Jamfile"`"
POWERUPS="`find powerups -name "*.pak" -o -name "Jamfile"`"
UNITS="`find units -name "*.pfl" -o -name "*.pak" -o -name "Jamfile"`"
SOUND="`find sound -name "*.wav" -o -name "Jamfile"`"
MAPS="`find maps -name "*.npm" -o -name "*.opt" -o -name "*.spn" -o -name "Jamfile"`"

echo "*** Creating Sourcepackage"
mkdir -p $SOURCERELEASE
# Create a new Jamfile
cat > $SOURCERELEASE/Jamfile << __END__
SubDir TOP ;

SubInclude TOP src ;
__END__

cp -p --parents $AUTOFILES $SOURCERELEASE
cp -p --parents $JAMFILES $SOURCERELEASE
cp -p --parents $TEXTS $SOURCERELEASE
cp -p --parents $SOURCES $SOURCERELEASE

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
__END__

find pics \( -name "*.bmp" -o -name "*.raw" -o -name "*.til" -o -name "*.pak" -o  -name "Jamfile" \) -exec cp -p --parents {} $DATARELEASE ';'
find wads \( -name "*.act" -o -name "*.tls" -o -name "Jamfile" \) -exec cp -p --parents {} $DATARELEASE ';'
find powerups \( -name "*.pak" -o -name "Jamfile" \) -exec cp -p --parents {} $DATARELEASE ';'
find units \( -name "*.pfl" -o -name "*.pak" -o -name "Jamfile" \) -exec cp -p --parents {} $DATARELEASE ';'
find sound \( -name "*.wav" -o -name "Jamfile" \) -exec cp -p --parents {} $DATARELEASE ';'
find maps \( -name "*.npm" -o -name "*.opt" -o -name "*.spn" -o -name "Jamfile" \) -exec cp -p --parents {} $DATARELEASE ';'
cp -p --parents $AUTOFILES $DATARELEASE
cp -p --parents $JAMFILES $DATARELEASE
cp -p --parents $TEXTS $DATARELEASE

echo "*** Packing Data"
cd release
tar -c --bzip2 -f netpanzerdata-$VERSION.tar.bz2 netpanzerdata-$VERSION
cd -

