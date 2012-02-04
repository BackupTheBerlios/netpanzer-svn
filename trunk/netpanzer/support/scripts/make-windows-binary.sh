#! /bin/bash

if [ ! -f RELEASE_VERSION ]; then
    echo "ERROR: RELEASE_VERSION file missing, cannot create"
    exit 1
fi

RELEASEDIR="releases"
VERSION="`<RELEASE_VERSION`"
NPDEST=/tmp/netpanzer
ZIPNAME="netpanzer-windows-${VERSION}.zip"
EXENAME="build/mingw/release/netpanzer.exe"

echo "Making netPanzer windows version ${VERSION}"

scons cross=mingw \
      sdlconfig=~/program/tools/crossmingw/bin/sdl-config

# with newer gcc seems stripping works as it should, doesn't need to strip again
# i586-mingw32msvc-strip "${EXENAME}"

[ -d "${NPDEST}" ] && rm -rf "${NPDEST}"

mkdir -p "${NPDEST}"
cp "${EXENAME}" "${NPDEST}"

support/scripts/copy_datafiles.sh "${NPDEST}"
support/scripts/copy_docfiles.sh "${NPDEST}"

cp ~/program/tools/crossmingw/dll/SDL/* "${NPDEST}"
cp ~/program/tools/crossmingw/dll/SDL_mixer/* "${NPDEST}"

[ ! -d "${RELEASEDIR}" ] && mkdir "${RELEASEDIR}"

pushd "${NPDEST}/.."
zip -9r "${ZIPNAME}" `basename "${NPDEST}"`
popd

[ -f "${RELEASEDIR}/${ZIPNAME}" ] && rm -f "${RELEASEDIR}/${ZIPNAME}"

mv "${NPDEST}/../${ZIPNAME}" "${RELEASEDIR}"
rm -rf "${NPDEST}"

