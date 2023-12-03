CFLAGS += -I./include
CFLAGS += -std=c++17

build:
	g++ src/anycc.cpp src/Lex/*.cpp -o main $(CFLAGS) 
run:build
	./main
