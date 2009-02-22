#! /bin/bash

if [ -z "$1" ]; then
	echo "usage: $0 <netpanzer_version>"
	exit 1
fi

VERSION="$1"
NPDEST=/tmp/netpanzer-svn
ZIPNAME="netpanzer-linux-${VERSION}.tar.gz"
PATH=$PATH:/usr/local/gcc/i686-linux/bin/

./configure \
	--host=i686-linux \
	--with-sdl-exec-prefix=/usr/local/gcc/i686-linux/usr \
	--enable-linuxbin

jam clean
jam 

i686-linux-strip netpanzer

jam "-sDESTDIR=${NPDEST}" \
	-sbindir=. \
	-sappdatadir=. \
	-sappdocdir=. \
	-sapplicationsdir=. \
	-spixmapsdir=. \
	install

mkdir packages
pushd "${NPDEST}/.."
tar zcvf "${ZIPNAME}" `basename "${NPDEST}"`
popd
mv "${NPDEST}/../${ZIPNAME}" packages

