#
# Network client library
#

AR	:= ar
CC	:= gcc
CFLAGS	:= -Wall -Wextra -O2 -Wno-unknown-pragmas -I ../include -I ../../include

SRCDIR	:= src
OBJDIR	:= obj
BINDIR	:= bin

TARGETS	:= $(BINDIR)/libirbis.a

SOURCES	:= src/connect.c src/dll.c src/ean.c src/error.c src/field.c src/format.c \
src/guard.c src/ini.c src/isbn.c iso2709.c src/menu.c src/query.c src/record.c \
src/response.c src/search.c src/spec.c src/subfield.c src/tree.c src/upc.c \
src/version.c src/xrf.c

OBJ	:= obj/connect.o obj/dll.o obj/ean.o obj/error.o obj/field.o obj/format.o \
obj/guard.o obj/ini.o obj/isbn.o obj/iso2709.o obj/menu.o obj/query.o obj/record.o \
obj/response.o obj/search.o obj/spec.o obj/subfield.o obj/tree.o obj/upc.o \
obj/version.o obj/xrf.o

.PHONY: all clean

all: $(TARGETS)

clean:
	rm -rf $(TARGETS) $(OBJDIR)

$(OBJ): $(OBJDIR)/%.o : $(SRCDIR)/%.c
	-mkdir $(OBJDIR)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BINDIR)/libirbis.a: $(OBJ)
	-mkdir $(BINDIR)
	$(AR) rcs $@ $(OBJ)
