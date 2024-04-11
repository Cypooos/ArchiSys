
main: main.c
	gcc -o main main.c

run: main
	./main

debug:
	gcc -ggdb3 -o main_dbg main.c
	valgrind ./main_dbg ibm4q05.dat

clean:
	rm -f main