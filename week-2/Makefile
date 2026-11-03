CC = gcc
CFLAGS = -Iinclude -std=c11 -Wall -Wextra -Wpedantic -O2 -D_POSIX_C_SOURCE=200809L -D_DEFAULT_SOURCE
LDFLAGS = -pthread -lrt
SRC = main.c algorithms/algorithms.c processes/core.c processes/logger.c ipc/ipc.c \
      ipc/posix_mq.c ipc/pipes.c ipc/shared_memory.c ipc/sockets.c ipc/fifo.c
OBJ = $(SRC:.c=.o)

all: simulator

simulator: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $(OBJ) $(LDFLAGS)

clean:
	rm -f $(OBJ) simulator
	rm -f /tmp/w4sim_fifo_*
	rm -f /dev/mqueue/w4sim_*

.PHONY: all clean
