ads7830_test: ads7830_test.c ../bsp/ads7830.c ../bsp/ads7830.h
	gcc -O3 $^ -I ../bsp/ -lwiringPi -o $@

board_test: board_test.c ../bsp/*
	gcc -O3 $^ -I ../bsp/ -lwiringPi -o $@

pisystem_test: pisystem_test.c ../bsp/pisystem.c ../bsp/pisystem.h
	gcc -O3 $^ -I ../bsp/ -o $@

communication_test: communication_test.c ../communication/* ../bsp/*
	gcc -g -Og $^ -I ../communication/ -I ../bsp/ -lwiringPi -o $@

engine_test: engine_test.c ../bsp/*
	gcc -O3 $^ -I ../bsp/ -lwiringPi -o $@

bmp280_test: bmp280_test.c ../bsp/* ../support/* ../components/bmp280.*
	gcc $^ -I ../bsp/ -I ../components/ -I ../support/ -lwiringPi -o $@

ms4525do_test: ms4525do_test.c ../bsp/* ../support/* ../components/ms4525do.*
	gcc $^ -I ../bsp/ -I ../components/ -I ../support/ -lwiringPi -lm -o $@