all:
	gcc src/main.c -o prog -lpthread
run:
	./prog