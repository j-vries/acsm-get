#!/bin/bash

# Abort on error.
set -e

# Copy files.
cp bin/acsm-get /usr/bin/
cp res/acsm-get.desktop /usr/share/applications/
cp res/acsm-get.png /usr/share/icons/hicolor/128x128/apps/
cp res/acsm-get.svg /usr/share/icons/hicolor/scalable/apps/
cp res/acsm-get-mimetypes.xml /usr/share/mime/packages/

# Update icon cache.
update-icon-caches /usr/share/icons/*
