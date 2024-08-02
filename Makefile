CXX          ?= $(CROSS)g++
CXXFLAGS     ?= -Wall -O3

SOURCES      =  src/main.cpp
SOURCES      += src/application.cpp
SOURCES      += src/core/adeptclient.cpp
SOURCES      += src/core/downloadinfo.cpp
SOURCES      += src/core/drmprocessorclient.cpp
SOURCES      += src/gen/resources.c
SOURCES      += src/ui/aboutdialog.cpp
SOURCES      += src/ui/acsmfilechooserdialog.cpp
SOURCES      += src/ui/authorizationwindow.cpp
SOURCES      += src/ui/mainwindow.cpp
SOURCES      += src/ui/messagedialogs.cpp
SOURCES      += src/utils/filesystemutils.cpp

INCLUDE_DIRS =  -Ilib/libgourou/include/
INCLUDE_DIRS += -Ilib/libgourou/utils/

STATIC_LIBS  =  lib/libgourou/libgourou.a
STATIC_LIBS  += lib/libgourou/utils/utils.a

SHARED_LIBS  =  -lcurl
SHARED_LIBS  += -lcrypto
SHARED_LIBS  += -lpugixml
SHARED_LIBS  += -lz
SHARED_LIBS  += -lzip

GTK_FLAGS    = `pkg-config gtkmm-3.0 --cflags --libs`

all: clean updfparser libgourou acsm-get dpkg

clean:
	cd lib/updfparser && $(MAKE) clean
	cd lib/libgourou && $(MAKE) clean
	rm -rf lib/libgourou/lib
	rm -rf bin
	rm -rf obj
	rm -rf res/gen/*
	rm -rf src/gen/*

updfparser:
	cd lib/updfparser && make BUILD_STATIC=1 BUILD_SHARED=0

libgourou:
	mkdir -p lib/libgourou/lib
	cd lib/libgourou/lib && rm -f updfparser && ln -s ../../../lib/updfparser/ updfparser
	cd lib/libgourou && $(MAKE) BUILD_STATIC=1 BUILD_SHARED=0 all

generate_licenses:
	mkdir -p res/gen
	scripts/generate_licenses_txt.sh

generate_resources:
	mkdir -p res/gen
	mkdir -p src/gen
	glib-compile-resources --target=src/gen/resources.c --generate-source res/acsmget.gresource.xml

acsm-get: generate_licenses generate_resources
	mkdir -p bin
	$(CXX) -o bin/acsm-get $(SOURCES) $(INCLUDE_DIRS) $(STATIC_LIBS) $(SHARED_LIBS) $(CXXFLAGS) $(GTK_FLAGS)

dpkg:
	scripts/generate_dpkg.sh

install:
	scripts/install.sh

uninstall:
	scripts/uninstall.sh
