# Compiler and flags
CC = gcc
CFLAGS = -O3 -march=native -funroll-loops -flto -Wall -Wextra -Wshadow -pedantic
LDFLAGS = -flto -W

# Target executable
TARGET = m.exe

# Source files
SRCS = main.c
OBJS = $(SRCS:.c=.o)

# Default rule
all: $(TARGET)

# Compile object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) $(LDFLAGS) -o $(TARGET)

# Clean build files
clean:
	rm -f $(OBJS) $(TARGET)
