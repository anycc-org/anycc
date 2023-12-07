CFLAGS += -I./include
CFLAGS += -std=c++11

build:
	g++ src/anycc.cpp src/Lex/*.cpp -o main $(CFLAGS) 
run:build
	./main
