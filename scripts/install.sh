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

# Copy files.
cp bin/acsm-get /usr/bin/
cp res/acsm-get.desktop /usr/share/applications/
cp res/acsm-get.png /usr/share/icons/hicolor/128x128/apps/
cp res/acsm-get.svg /usr/share/icons/hicolor/scalable/apps/
cp res/acsm-get-mimetypes.xml /usr/share/mime/packages/

# Update icon cache.
update-icon-caches /usr/share/icons/*
