all:
	gcc main.c print.c input.c function.c hop.c -o shell
	./shell

clean:
	rm -f shell
