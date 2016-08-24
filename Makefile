all: kdata
kdata: main.c
	g++ -l pthread -l freenect main.c -o kdata
debug: main.c
	g++ -g -l pthread -l freenect main.c -o kdata
