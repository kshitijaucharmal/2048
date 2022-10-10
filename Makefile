CC = g++
CFLAGS = -Wall -g -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

all:
	${CC} -o main ${CFLAGS} main.cpp;
	chmod +x main.cpp;
	./main;

windows:
	${CC} -o main.exe ${CFLAGS} main.cpp;
	./main.exe;

clean:
	rm -f main
