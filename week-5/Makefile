CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -O2 -Isrc
GUI_LIBS = -mwindows -lcomctl32 -lgdi32
SOURCES = src/main.c src/simulator.c src/algorithms.c src/parser.c
TARGET = PageReplacementSimulator.exe

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(TARGET) $(GUI_LIBS)

clean:
	del /Q $(TARGET) 2>NUL

.PHONY: all clean
