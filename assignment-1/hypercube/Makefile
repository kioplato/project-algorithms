#!/usr/bin/make -f

SHELL := /bin/sh

# Directory where object files live.
OBJDIR := build
# Directory where executable files live.
BINDIR := bin
# Directory where source files live.
SRCDIR := src

# Compiler to use.
CC := gcc
# Flags for compiling.
CFLAGS := -Wall -Wextra -g -O2
# Flags for linking.
LDFLAGS := -g

# Paths to source files.
SRC := $(shell find $(SRCDIR) -name "*.c")
# Paths to object files.
OBJ := $(SRC:.c=.o)

# Name of executable.
BIN := cube

.PHONY: all clean

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(LDFLAGS) $(addprefix $(OBJDIR)/, $(notdir $(OBJ))) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $(OBJDIR)/$(notdir $@)

clean:
	rm -f $(OBJDIR)/* $(BINDIR)/*
