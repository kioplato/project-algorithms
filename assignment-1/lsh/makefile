#!/usr/bin/make -f

SHELL := /bin/sh

# Directory where submodules live.
SUBDIR := submodules/database

# Paths to Makefiles.
SUB := $(shell find $(SUBDIR) -name "Makefile")

.PHONY: $(SUB) clean

all: $(SUB)

$(SUB):
	echo $(SUB)
	$(MAKE) -C $(@D)
	cp $(@D)/bin/* ./bin/

#
##!/usr/bin/make -f
#
#SHELL := /bin/sh
#
## Directory where object files live.
#OBJDIR := build
## Directory where executable files live.
#BINDIR := bin
## Directory where source files live.
#SRCDIR := src
#
## Compiler to use.
#CC := gcc
## Flags for compiling.
#CFLAGS := -Wall -Wextra -g -O2
## Flags for linking.
#LDFLAGS := -g
#
## Paths to source files.
#SRC := $(shell find $(SRCDIR) -name "*.c")
## Paths to object files.
#OBJ := $(SRC:.c=.o)
#
## Name of executable.
#BIN := diseaseMonitor
#
#.PHONY: all clean
#
#all: $(BIN)
#
#$(BIN): $(OBJ)
#	$(CC) $(LDFLAGS) $(addprefix $(OBJDIR)/, $(notdir $(OBJ))) -o $@ -lcrypto
#
#%.o: %.c
#	$(CC) $(CFLAGS) -c $< -o $(OBJDIR)/$(notdir $@)
#
#clean:
#	rm -f $(OBJDIR)/* $(BINDIR)/*
## Paths to source files.
#SRC := $(shell find $(SRCDIR) -name "*.c")
## Paths to object files.
#OBJ := $(SRC:.c=.o)
#
## Name of executable.
#BIN := diseaseMonitor
#
#.PHONY: all clean
#
#all: $(BIN)
#
#$(BIN): $(OBJ)
#	$(CC) $(LDFLAGS) $(addprefix $(OBJDIR)/, $(notdir $(OBJ))) -o $@ -lcrypto
#
#%.o: %.c
#	$(CC) $(CFLAGS) -c $< -o $(OBJDIR)/$(notdir $@)
#
#clean:
#	rm -f $(OBJDIR)/* $(BINDIR)/*
	@git pull -f origin > /dev/null 2>&1
	make
