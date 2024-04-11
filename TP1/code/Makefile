objs = mmap.o alloc.o
out = test

$(out): $(objs)
	gcc -g -o $(out) $(out).c $(objs)

*.o:*.c
	gcc -g -c $@

run: $(out)
	./$(out)

clean:
	rm -f $(out) $(objs)