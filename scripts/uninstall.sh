#!/bin/bash

# Remove files.
rm /usr/bin/acsm-get
rm /usr/share/applications/acsm-get.desktop
rm /usr/share/icons/hicolor/128x128/apps/acsm-get.png
rm /usr/share/icons/hicolor/scalable/apps/acsm-get.svg
rm /usr/share/mime/packages/acsm-get-mimetypes.xml

# Update icon cache.
update-icon-caches /usr/share/icons/*
