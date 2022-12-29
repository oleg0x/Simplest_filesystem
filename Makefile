CC1 = gcc
CFLAGS = -std=c17 -Wall -Wextra

TARGET = zzz
OBJ = main.o fs.o

.PHONY: all clean

all: $(TARGET)

clean:
	rm -rf $(TARGET) *.o

re: clean all

fs.o: fs.h fs.c
	$(CC) $(CFLAGS) -c fs.c

main.o: fs.h main.c
	$(CC) $(CFLAGS) -c main.c

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)
