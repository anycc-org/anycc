CFLAGS += -I./include
build:
	g++ src/anycc.cpp src/Lex/*.cpp -o main $(CFLAGS)
run:build
	./main
