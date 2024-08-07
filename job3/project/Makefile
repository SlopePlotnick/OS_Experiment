CC=gcc
CFLAGS=-O
OBJS=main.o max.o min.o
TARGET=app

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)
main.o: main.c max.h min.h
	$(CC) $(CFLAGS) -c main.c
max.o: max.c max.h
	$(CC) $(CFLAGS) -c max.c
min.o: min.c min.h
	$(CC) $(CFLAGS) -c min.c
clean:
	rm -f $(TARGET) $(OBJS)