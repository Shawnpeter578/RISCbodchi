# Makefile for RISC-bodchi OS Simulator
CC      := gcc
CFLAGS  := -Wall -Wextra -O2
LDLIBS  := -lrt
NCLIBS  := -lncurses

BUILD   := build

CORE_SRC   := src/core.c
UI_SRC     := src/ui.c
LOGGER_SRC := src/logger.c
MAIN_SRC   := src/main.c

CORE_BIN   := $(BUILD)/core
UI_BIN     := $(BUILD)/ui
LOGGER_BIN := $(BUILD)/logger
MAIN_BIN   := riscbodchi_sim

.PHONY: all clean

all: $(BUILD) $(CORE_BIN) $(UI_BIN) $(LOGGER_BIN) $(MAIN_BIN)

$(BUILD):
	mkdir -p $(BUILD)

$(CORE_BIN): $(CORE_SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS)

$(UI_BIN): $(UI_SRC)
	$(CC) $(CFLAGS) -o $@ $< $(NCLIBS) $(LDLIBS)

$(LOGGER_BIN): $(LOGGER_SRC)
	$(CC) $(CFLAGS) -o $@ $< $(LDLIBS)

$(MAIN_BIN): $(MAIN_SRC)
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -rf $(BUILD) $(MAIN_BIN) simulator.log
