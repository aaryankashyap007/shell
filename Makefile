all:
	gcc main.c print.c -o shell
	./shell

clean:
	rm -f shell
