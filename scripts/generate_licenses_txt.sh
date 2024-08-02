#!/bin/bash

LICENSES_TXT="res/gen/licenses.txt"

# Abort on error.
set -e

# Ensure all versions can be determined.
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

# Get version from lib/libgourou/include/libgourou.h, #define LIBGOUROU_VERSION.
LIBGOUROU_VERSION=$(cat lib/libgourou/include/libgourou.h | grep LIBGOUROU_VERSION | cut -d '"' -f2)
ensureVersionSuccessfullyDetermined "$LIBGOUROU_VERSION" "libgourou"

# Use the git commit hash as version, since there is no explicitly defined version.
UPDFPARSER_VERSION="'$(git --git-dir lib/updfparser/.git log -1 --pretty=format:' %h' | cut -d " " -f2)'"
ensureVersionSuccessfullyDetermined "$UPDFPARSER_VERSION" "uPDFParser"

# Generate licenses.txt
echo "=== License for acsm-get $ACSMGET_VERSION ==="                           >  $LICENSES_TXT
echo ""                                                                        >> $LICENSES_TXT
cat COPYING                                                                    >> $LICENSES_TXT
echo ""                                                                        >> $LICENSES_TXT

echo "=== License for library libgourou $LIBGOUROU_VERSION ==="                >> $LICENSES_TXT
echo ""                                                                        >> $LICENSES_TXT
cat lib/libgourou/LICENSE                                                      >> $LICENSES_TXT
echo ""                                                                        >> $LICENSES_TXT

echo "=== License for library libgourou utils $LIBGOUROU_VERSION ==="          >> $LICENSES_TXT
echo ""                                                                        >> $LICENSES_TXT
cat lib/libgourou/utils/LICENSE                                                >> $LICENSES_TXT
echo ""                                                                        >> $LICENSES_TXT

echo "=== License for library uPDFParser $UPDFPARSER_VERSION ==="              >> $LICENSES_TXT
echo ""                                                                        >> $LICENSES_TXT
cat lib/updfparser/LICENSE                                                     >> $LICENSES_TXT
echo ""                                                                        >> $LICENSES_TXT
