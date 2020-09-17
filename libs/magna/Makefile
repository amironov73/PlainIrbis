#
# Common support library
#

AR	:= ar
CC	:= gcc
CFLAGS	:= -Wall -Wextra -O2 -Wno-unknown-pragmas -I ../include -I ../../include

SRCDIR	:= src
OBJDIR	:= obj
BINDIR	:= bin

TARGETS	:= $(BINDIR)/libmagna.a

SOURCES	:= src/array.c src/beep.c src/buffer.c src/chunked.c src/cp1251.c src/cp866.c src/date.c src/delphi.c src/dump.c src/encoding.c src/enumertr.c \
src/error.c src/fastbuf.c src/file.c src/handle.c src/intarray.c src/io.c src/keyboard.c src/koi8r.c src/magna.c src/memory.c src/navigatr.c src/number.c \
src/path.c src/retry.c src/screen.c src/sleep.c src/span.c src/spanarry.c src/stream.c src/string.c src/tcp4.c src/thread.c src/utf8.c src/utils.c

OBJ	:= obj/array.o obj/beep.o obj/buffer.o obj/chunked.o obj/cp1251.o obj/cp866.o obj/date.o obj/delphi.o obj/dump.o obj/encoding.o obj/enumertr.o \
obj/error.o obj/fastbuf.o obj/file.o obj/handle.o obj/intarray.o obj/io.o obj/keyboard.o obj/koi8r.o obj/magna.o obj/memory.o obj/navigatr.o obj/number.o \
obj/path.o obj/retry.o obj/screen.o obj/sleep.o obj/span.o obj/spanarry.o obj/stream.o obj/string.o obj/tcp4.o obj/thread.o obj/utf8.o obj/utils.o

.PHONY: all clean

all: $(TARGETS)

clean:
	rm -rf $(TARGETS) $(OBJDIR)

$(OBJ): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	-mkdir $(OBJDIR)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BINDIR)/libmagna.a: $(OBJ)
	-mkdir $(BINDIR)
	$(AR) rcs $@ $(OBJ)
