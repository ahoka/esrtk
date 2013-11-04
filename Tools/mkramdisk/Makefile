CC=	clang
CFLAGS=	-std=c99 -O2 -Wall -Wextra -Werror

all: mkramdisk

mkramdisk: mkramdisk.c
	$(CC) $(CFLAGS) -o mkramdisk mkramdisk.c

clean:
	-rm mkramdisk


