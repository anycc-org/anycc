CFLAGS += -I./include


build:
	g++ src/main.cpp -o main $(CFLAGS)


run:build
	./main

