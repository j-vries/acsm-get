#!/bin/bash

# Abort on error.
set -e

# Ensure version can be determined.
ensureVersionSuccessfullyDetermined()
{
	if [ ! -z "${1}" ]; then
		echo "$2 version: $1"
	else
		echo "Unable to determine version of $2."
		exit 1
	fi
}

# Get version from src/main.h, #define ACSMGET_VERSION.
ACSMGET_VERSION=$(cat src/main.h | grep ACSMGET_VERSION | cut -d '"' -f2)
ensureVersionSuccessfullyDetermined "$ACSMGET_VERSION" "acsm-get"

# Set up folders.
cd bin
DPKG_NAME="acsm-get_${ACSMGET_VERSION}_amd64"
rm -rf "${DPKG_NAME}"
mkdir -p "${DPKG_NAME}"
mkdir -p "${DPKG_NAME}/DEBIAN"
mkdir -p "${DPKG_NAME}/debian"
mkdir -p "${DPKG_NAME}/usr/bin"
mkdir -p "${DPKG_NAME}/usr/share/applications"
mkdir -p "${DPKG_NAME}/usr/share/icons/hicolor/128x128/apps"
mkdir -p "${DPKG_NAME}/usr/share/icons/hicolor/scalable/apps"
mkdir -p "${DPKG_NAME}/usr/share/mime/packages"

# Copy files.
cp acsm-get "${DPKG_NAME}/usr/bin/"
cp ../res/acsm-get.desktop "${DPKG_NAME}/usr/share/applications/"
cp ../res/acsm-get.png "${DPKG_NAME}/usr/share/icons/hicolor/128x128/apps/"
cp ../res/acsm-get.svg "${DPKG_NAME}/usr/share/icons/hicolor/scalable/apps/"
cp ../res/acsm-get-mimetypes.xml "${DPKG_NAME}/usr/share/mime/packages/"

# Determine dependencies.
touch "${DPKG_NAME}/debian/control"
cd $DPKG_NAME
DEPENDS=$(dpkg-shlibdeps -O usr/bin/acsm-get)
DEPENDS=$(echo $DEPENDS | sed -e 's/shlibs:Depends=//g')
cd ..
rm -rf "${DPKG_NAME}/debian"

# Create control file.
DEBIAN_CONTROL="${DPKG_NAME}/DEBIAN/control"

echo "Package: acsm-get"                                                                >  $DEBIAN_CONTROL
echo "Version: ${ACSMGET_VERSION}"                                                      >> $DEBIAN_CONTROL
echo "Architecture: amd64"                                                              >> $DEBIAN_CONTROL
echo "Maintainer: Jan de Vries <128360649+j-vries@users.noreply.github.com>"            >> $DEBIAN_CONTROL
echo "Description: acsm-get is an open source tool to download e-books with Adobe DRM." >> $DEBIAN_CONTROL
echo "Depends: ${DEPENDS}"                                                              >> $DEBIAN_CONTROL

# Create the package. This command will give a warning "binaries to analyze should already be installed in their package's directory", but that is not a problem.
dpkg-deb --build --root-owner-group ${DPKG_NAME}
rm -rf ${DPKG_NAME}
