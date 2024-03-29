ads7830_test: ads7830_test.c ../bsp/ads7830.c ../bsp/ads7830.h
	gcc -O3 $^ -I ../bsp/ -lwiringPi -o $@

board_test: board_test.c ../bsp/*
	gcc -O3 $^ -I ../bsp/ -lwiringPi -o $@