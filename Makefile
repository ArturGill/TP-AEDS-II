CC=gcc
CFLAGS=-Wall -Wextra -g
SRC=$(wildcard *.c)
OBJ=$(SRC:.c=.o)
TARGET=app.exe

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del /Q $(OBJ) $(TARGET)