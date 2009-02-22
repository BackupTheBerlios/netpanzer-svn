#! /bin/bash

if [ -z "$1" ]; then
	echo "usage: $0 <netpanzer_version>"
	exit 1
fi

VERSION="$1"
NPDEST=/tmp/netpanzer-svnwin
ZIPNAME="netpanzer-windows-${VERSION}.zip"

./configure \
	--host=i686-mingw32 \
	--with-sdl-exec-prefix=/usr/local/gcc/mingw-3.4.5/usr

jam clean
jam 

i686-mingw32-strip netpanzer.exe

jam "-sDESTDIR=${NPDEST}" \
	-sbindir=. \
	-sappdatadir=. \
	-sappdocdir=. \
	-sapplicationsdir=. \
	-spixmapsdir=. \
	install

cp /usr/local/gcc/mingw-3.4.5/usr/bin/SDL.dll "${NPDEST}"
cp /usr/local/gcc/mingw-3.4.5/usr/lib/*.dll "${NPDEST}"

mkdir packages
pushd "${NPDEST}/.."
zip -9r "${ZIPNAME}" `basename "${NPDEST}"`
popd
mv "${NPDEST}/../${ZIPNAME}" packages

