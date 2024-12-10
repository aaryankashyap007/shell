all:
	gcc main.c print.c input.c function.c -o shell
	./shell

clean:
	rm -f shell
