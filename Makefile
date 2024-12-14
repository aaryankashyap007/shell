all:
	gcc main.c print.c input.c function.c hop.c reveal.c log.c sys_commands.c proclore.c -o shell
	./shell

clean:
	rm -f shell
	rm -f log_commands.txt
