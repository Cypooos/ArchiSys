objs = main.o util.o
out = main

$(out): $(objs)
	gcc -o $(out) $(objs)

*.o:*.c
	gcc -c $@

run: $(out)
	./$(out)

clean:
	rm -f $(out) $(objs)