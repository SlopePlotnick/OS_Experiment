CC=gcc
CFLAGS=-O
OBJS=main.o cmd.o parse.o
TARGET=sh

$(TARGET): $(OBJS)
	$(CC) -o $(TARGET) $(OBJS)
main.o: main.c cmd.h parse.h
	$(CC) $(CFLAGS) -c main.c
cmd.o: cmd.c cmd.h
	$(CC) $(CFLAGS) -c cmd.c
parse.o: parse.c parse.h
	$(CC) $(CFLAGS) -c parse.c
clean:
	rm -f $(TARGET) $(OBJS)