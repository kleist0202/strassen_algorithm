CC = gcc
SRCDIR = src
BINDIR = bin
PROG_NAME = strassen

SOURCES := $(wildcard $(SRCDIR)/*.c)

$(PROG_NAME):
	$(CC) -o $@ $(SOURCES)

all: $(PROG_NAME)
	mv $(PROG_NAME) $(BINDIR)

run:
	./$(BINDIR)/$(PROG_NAME)

plot:
	python3 $(SRCDIR)/strassen_plot.py

.PHONY: all run plot
