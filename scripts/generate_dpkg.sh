#!/bin/bash

# Ensure all the expected files exist.
ensureFileExists()
{
	if [ ! -f "$1" ]; then
		echo "$1 does not exist."
		exit 1
	fi
}

ensureFileExists bin/acsm-get
ensureFileExists res/acsm-get.desktop
ensureFileExists res/acsm-get.png
ensureFileExists res/acsm-get.svg
ensureFileExists res/acsm-get-mimetypes.xml

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
PACKAGE_NAME="acsm-get_${ACSMGET_VERSION}_1_amd64"
rm -rf "${PACKAGE_NAME}"
mkdir -p "${PACKAGE_NAME}"
mkdir -p "${PACKAGE_NAME}/DEBIAN"
mkdir -p "${PACKAGE_NAME}/debian"
mkdir -p "${PACKAGE_NAME}/usr/bin"
mkdir -p "${PACKAGE_NAME}/usr/share/applications"
mkdir -p "${PACKAGE_NAME}/usr/share/icons/hicolor/128x128/apps"
mkdir -p "${PACKAGE_NAME}/usr/share/icons/hicolor/scalable/apps"
mkdir -p "${PACKAGE_NAME}/usr/share/mime/packages"

# Copy files.
cp acsm-get "${PACKAGE_NAME}/usr/bin/"
cp ../res/acsm-get.desktop "${PACKAGE_NAME}/usr/share/applications/"
cp ../res/acsm-get.png "${PACKAGE_NAME}/usr/share/icons/hicolor/128x128/apps/"
cp ../res/acsm-get.svg "${PACKAGE_NAME}/usr/share/icons/hicolor/scalable/apps/"
cp ../res/acsm-get-mimetypes.xml "${PACKAGE_NAME}/usr/share/mime/packages/"

# Determine dependencies.
touch "${PACKAGE_NAME}/debian/control"
cd $PACKAGE_NAME
DEPENDS=$(dpkg-shlibdeps -O usr/bin/acsm-get)
DEPENDS=$(echo $DEPENDS | sed -e 's/shlibs:Depends=//g')
cd ..
rm -rf "${PACKAGE_NAME}/debian"

# Create control file.
DEBIAN_CONTROL="${PACKAGE_NAME}/DEBIAN/control"

echo "Package: acsm-get"                                                                >  $DEBIAN_CONTROL
echo "Version: ${ACSMGET_VERSION}"                                                      >> $DEBIAN_CONTROL
echo "Architecture: amd64"                                                              >> $DEBIAN_CONTROL
echo "Maintainer: Jan de Vries <128360649+j-vries@users.noreply.github.com>"            >> $DEBIAN_CONTROL
echo "Description: acsm-get is an open source tool to download e-books with Adobe DRM." >> $DEBIAN_CONTROL
echo "Depends: ${DEPENDS}"                                                              >> $DEBIAN_CONTROL

# Create the package. This command will give a warning "binaries to analyze should already be installed in their package's directory", but that is not a problem.
dpkg-deb --build --root-owner-group ${PACKAGE_NAME}
rm -rf ${PACKAGE_NAME}
