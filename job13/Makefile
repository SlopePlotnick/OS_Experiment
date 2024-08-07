TOP_DIR      := $(shell pwd)
TOP_DIR		 := .

CC           := gcc
LD           := gcc
AS           := gcc

CFLAGS       := -c -I$(TOP_DIR) -D__UXFS__
CFLAGS       += -Wall -Wundef -Wno-unused -Wno-unused-result -Wno-parentheses
CFLAGS       += -Wno-pointer-arith -Wno-pointer-sign -Wno-int-to-pointer-cast
CFLAGS       += -Wfatal-errors -Werror -Wno-format
CFLAGS       += -fno-common
AFLAGS       := -c

CFLAGS       += -D_FILE_OFFSET_BITS=64 -I/usr/include/fuse
LFLAGS       := -lfuse -lpthread

DEBUG        := 1
ifeq ($(DEBUG), 1)
CFLAGS 	     += -g
CFLAGS       += -fsanitize=address
LFLAGS       += -fsanitize=address
else
CFLAGS       += -O -DNDEBUG -fno-omit-frame-pointer
endif

SRCS		:= $(wildcard *.c utils/*.c)
OBJS		:= $(subst .c,.o,$(SRCS))

TARGET		:= uxfs

all: $(OBJS)
	@echo "LD -o $(TARGET)"
	@$(LD) -o $(TARGET) $(OBJS) $(LFLAGS) 

%.o: %.c
	@echo "CC $<"
	@$(CC) $(CFLAGS) -o $@ $<

dep:
	@(for SRC in $(SRCS); do						\
		OBJ=`echo $$SRC | sed -e 's/\.[cS]/\.o/'`;	\
		cpp -I$(TOP_DIR) -MM -MT $$OBJ $$SRC;		\
	done) >Makefile.dep

clean:
	@rm -f $(OBJS) $(TARGET) tags core Makefile.dep *.log disk.bin

count:
	@wc -l */*.[chS]

MOUNT_DIR    := mnt

run:
	./uxfs -log disk.bin $(MOUNT_DIR)

umt:
	fusermount -u $(MOUNT_DIR)

ctags:
	ctags */*.[chS]

ifeq (Makefile.dep, $(wildcard Makefile.dep))
include Makefile.dep
endif
