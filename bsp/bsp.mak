CC = gcc
COMPILE_FLAGS = -O3 -std=c2x

.PHONY: bsp create_build clean

bsp: create_build build/ads7830.o build/pca9685.o build/pisystem.o build/board.o

create_build:
	if ! [ -d build ]; then mkdir build; fi;

build/ads7830.o: ads7830.c ads7830.h
	$(CC) -c $(COMPILE_FLAGS) $< $(INCLUDE_FLAGS) -o $@
build/pca9685.o: pca9685.c pca9685.h
	$(CC) -c $(COMPILE_FLAGS) $< $(INCLUDE_FLAGS) -o $@
build/pisystem.o: pisystem.c pisystem.h
	$(CC) -c $(COMPILE_FLAGS) $< $(INCLUDE_FLAGS) -o $@
build/board.o: board.c board.h
	$(CC) -c $(COMPILE_FLAGS) $< $(INCLUDE_FLAGS)  -o $@

clean:
	rm -r build;