CC=	clang
CFLAGS=	-std=c99 -O2 -Wall -Wextra -Werror

all: mkmodule

mkmodule: mkmodule.c
	$(CC) $(CFLAGS) -o mkmodule mkmodule.c

clean:
	-rm mkmodule


