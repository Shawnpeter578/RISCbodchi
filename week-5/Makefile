# ==========================================================================
# Makefile — Page Replacement Simulator
# ==========================================================================
CC      ?= gcc
CFLAGS  ?= -std=c11 -Wall -Wextra -Wpedantic -Wshadow -Wconversion -O2 -g
CPPFLAGS?=
LDFLAGS ?=
LDLIBS  ?=

SRC_DIR       := src
INC_DIR       := include
TEST_DIR      := test
OBJ_DIR       := build/obj
TEST_BIN_DIR  := build/bin

BACKEND_LIB   := $(SRC_DIR)/backend.c
BACKEND_MAIN  := $(SRC_DIR)/backend_main.c
CONSOLE_MAIN  := $(SRC_DIR)/main.c
ENTRY_SRCS    := $(BACKEND_MAIN) $(CONSOLE_MAIN)

EXPECTED_CORE := \
    $(SRC_DIR)/orchestrator.c \
    $(SRC_DIR)/fifo.c \
    $(SRC_DIR)/lru.c \
    $(SRC_DIR)/optimal.c \
    $(SRC_DIR)/display.c

CORE_FOUND   := $(wildcard $(EXPECTED_CORE))
CORE_MISSING := $(filter-out $(CORE_FOUND),$(EXPECTED_CORE))

ALL_SRCS      := $(wildcard $(SRC_DIR)/*.c)
UNIT_SRCS     := $(filter-out $(ENTRY_SRCS),$(ALL_SRCS))
UNIT_NAMES    := $(patsubst $(SRC_DIR)/%.c,%,$(UNIT_SRCS))

RUNNABLE_NAMES := $(foreach n,$(UNIT_NAMES),$(if $(wildcard $(TEST_DIR)/test_$(n).c),$(n),))
MISSING_TESTS  := $(filter-out $(RUNNABLE_NAMES),$(UNIT_NAMES))

BACK_LIB_OBJ  := $(OBJ_DIR)/src_backend.o
BACK_MAIN_OBJ := $(OBJ_DIR)/backend_main.o
CON_OBJ       := $(OBJ_DIR)/console_main.o
LIB_OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/src_%.o,$(UNIT_SRCS))

.PHONY: all backend full run status clean help check-core test test-strict test-backend

all: check-core backend
	@if [ -f "$(CONSOLE_MAIN)" ] && [ -z "$(CORE_MISSING)" ]; then \
	    $(MAKE) --no-print-directory full; \
	fi
	@$(MAKE) --no-print-directory test

backend: $(OBJ_DIR) $(BACK_MAIN_OBJ) $(BACK_LIB_OBJ) $(filter-out $(BACK_LIB_OBJ),$(LIB_OBJS))
	@$(CC) $(CFLAGS) $(CPPFLAGS) $(BACKEND_ONLY_DEFINE) \
	    $(BACK_MAIN_OBJ) $(LIB_OBJS) \
	    $(LDFLAGS) $(LDLIBS) -o page_sim_backend

full: $(OBJ_DIR) $(CON_OBJ) $(LIB_OBJS)
	@$(CC) $(CFLAGS) $(CPPFLAGS) $(CON_OBJ) $(LIB_OBJS) $(LDFLAGS) $(LDLIBS) -o page_sim

run: backend
	@./page_sim_backend

test: $(TEST_BIN_DIR)
	@for n in $(RUNNABLE_NAMES); do \
	    t="$(TEST_BIN_DIR)/test_$$n"; \
	    $(MAKE) --no-print-directory "$$t" >/dev/null 2>&1; \
	    "$$t"; \
	done

$(OBJ_DIR) $(TEST_BIN_DIR):
	@mkdir -p $@

$(BACK_LIB_OBJ): $(BACKEND_LIB) | $(OBJ_DIR)
	@$(CC) $(CFLAGS) $(CPPFLAGS) -I$(INC_DIR) -I. -c $< -o $@

$(OBJ_DIR)/src_%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@$(CC) $(CFLAGS) $(CPPFLAGS) -I$(INC_DIR) -I. -Dmain=main_$* -c $< -o $@

$(BACK_MAIN_OBJ): $(BACKEND_MAIN) | $(OBJ_DIR)
	@$(CC) $(CFLAGS) $(CPPFLAGS) -I$(INC_DIR) -I. -c $< -o $@

$(CON_OBJ): $(CONSOLE_MAIN) | $(OBJ_DIR)
	@$(CC) $(CFLAGS) $(CPPFLAGS) -I$(INC_DIR) -I. -c $< -o $@

$(OBJ_DIR)/test_%.o: $(TEST_DIR)/test_%.c | $(OBJ_DIR)
	@$(CC) $(CFLAGS) $(CPPFLAGS) -I$(INC_DIR) -I$(TEST_DIR) -c $< -o $@

$(TEST_BIN_DIR)/test_%: $(OBJ_DIR)/test_%.o $(OBJ_DIR)/src_%.o | $(TEST_BIN_DIR)
	@$(CC) $(CFLAGS) $(CPPFLAGS) $^ $(LDFLAGS) $(LDLIBS) -o $@

clean:
	@rm -rf build page_sim_backend page_sim ./page_sim.sock
