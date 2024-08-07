FUSE := -D_FILE_OFFSET_BITS=64 -I/usr/include/fuse -lfuse -pthread
MNT  := mnt

all:
	gcc -Wall -o hello hello.c $(FUSE)

clean:
	rm hello

run:
	./hello $(MNT)

umnt:
	fusermount -u $(MNT)
