all:
	gcc main.c print.c input.c function.c hop.c reveal.c log.c -o shell
	./shell

clean:
	rm -f shell
