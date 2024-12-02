# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -O2 -g

# Executable name
TARGET = myprogram

# Source files
SRCS = buf.c test.c

# Object files (derived from SRCS)
OBJS = $(SRCS:.c=.o)

# Default rule
all: $(TARGET)

# Linking the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

# Compiling source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	rm -f $(OBJS) $(TARGET)

# Phony targets
.PHONY: all clean
