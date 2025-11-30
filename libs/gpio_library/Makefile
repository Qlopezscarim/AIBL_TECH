# Makefile for BeagleBone GPIO API

CC = gcc
CFLAGS = -Wall -O2
LDFLAGS = 

# Library
LIB = libbbgpio.a
LIB_SRC = bb_gpio.c
LIB_OBJ = bb_gpio.o

# Example program
EXAMPLE = gpio_example
EXAMPLE_SRC = example_usage.c
EXAMPLE_OBJ = example_usage.o

.PHONY: all clean lib example

all: lib example

# Build static library
lib: $(LIB)

$(LIB): $(LIB_OBJ)
	ar rcs $(LIB) $(LIB_OBJ)
	@echo "Library created: $(LIB)"

$(LIB_OBJ): $(LIB_SRC) bb_gpio.h
	$(CC) $(CFLAGS) -c $(LIB_SRC) -o $(LIB_OBJ)

# Build example program
example: $(EXAMPLE)

$(EXAMPLE): $(EXAMPLE_OBJ) $(LIB)
	$(CC) $(EXAMPLE_OBJ) -L. -lbbgpio -o $(EXAMPLE)
	@echo "Example program created: $(EXAMPLE)"
	@echo "Run with: sudo ./$(EXAMPLE)"

$(EXAMPLE_OBJ): $(EXAMPLE_SRC) bb_gpio.h
	$(CC) $(CFLAGS) -c $(EXAMPLE_SRC) -o $(EXAMPLE_OBJ)

clean:
	rm -f $(LIB_OBJ) $(EXAMPLE_OBJ) $(LIB) $(EXAMPLE)
	@echo "Cleaned build files"

install: lib
	@echo "Installing library and header..."
	@echo "Note: This is a basic install. Adjust paths as needed."
	# cp $(LIB) /usr/local/lib/
	# cp bb_gpio.h /usr/local/include/
	# ldconfig
