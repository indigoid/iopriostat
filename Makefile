CFLAGS=-Wall -Wextra -std=c99

iopriostat: iopriostat.c ioprio.h
clean:
	$(RM) iopriostat iopriostat.o
