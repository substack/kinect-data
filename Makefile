all: kdata
kdata: main.c
	g++ -l freenect main.c -o kdata
