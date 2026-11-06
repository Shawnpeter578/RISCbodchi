CC = gcc
CFLAGS = -Wall -fPIC -shared
TARGET = libdeadlock.so
SRCS = state.c bankers.c deadlock.c resource.c
OBJS = $(SRCS:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c common.h
	$(CC) -Wall -fPIC -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)
