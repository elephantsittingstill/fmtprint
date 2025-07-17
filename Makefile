CC = gcc
CFLAGS = -Wall -Wextra -std=c89 -g #-fsanitize=address
OBJ = main.o lexer.o string.o specifier.o
DEPS = lexer.h string.h

printf: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $<

test: 
	$(CC) $(CFLAGS) test.c -o test

clean:
	rm -f *.o printf
