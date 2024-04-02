CC = gcc
COMPILE_FLAGS = -O3 -std=c2x

.PHONY: all create_build \
	 	build_bsp build_components \
		build_communication build_algorithms \
		build_application

all: create_build build_bsp build_components \
	 build_communication build_algorithms \
	 build_application build/main

create_build:
	if ! [ -d build ]; then mkdir build; fi;

INCLUDE_FLAGS = -I bsp/
LINK_LIBS = -lwiringPi


build_bsp: bsp/*
	cd bsp && make -f bsp.mak bsp
build_components: components/*
	cd components && make -f components.mak components
build_communication: communication/*
	cd communication && make -f communication.mak communication
build_algorithms: algorithms/*
	cd algorithms && make -f algorithms.mak algorithms
build_application: application/*
	cd application && make -f application.make application

build/main: bsp/build/*.o components/build/*.o communication/build/*.o algorithms/build/*.o application/build/*.o
	$(CC) $^ $(COMPILE_FLAGS) -o $@ $(LINK_LIBS)
