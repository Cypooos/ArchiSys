all: test

test: test.c vmap.o mmap.o linked_list.o
	gcc -o test test.c vmap.o mmap.o linked_list.o -w

mmap.o: mmap.c mmap.h linked_list.c linked_list.h
	gcc -c mmap.c

linked_list.o: linked_list.c linked_list.h
	gcc -c linked_list.c

clean:
	rm -f test *.o
