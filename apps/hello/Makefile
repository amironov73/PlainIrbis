#########################
# Common support library
#########################

.PHONY: all clean

APPLICATION_NAME := hello
PWD              := $(shell pwd)
SRCDIR           := $(PWD)/src
OBJDIR           := $(PWD)/obj
BINDIR           := $(PWD)/bin
INCLUDE_DIR1     := $(PWD)/include
INCLUDE_DIR2     := $(PWD)/../../include
LIBRARIES        := $(PWD)/../../libs/magna/bin/libmagna.a $(PWD)/../../libs/irbis/bin/libirbis.a
TARGET           := $(BINDIR)/$(APPLICATION_NAME)

AR     := ar
CC     := gcc
CFLAGS := -Wall -Wextra -O2 -Wno-unknown-pragmas -I $(INCLUDE_DIR1) -I $(INCLUDE_DIR2)

INCLUDE_FILES1 := $(shell ls $(INCLUDE_DIR1)/*.h)
INCLUDE_FILES2 := $(shell ls $(INCLUDE_DIR2)/magna/*.h)
SOURCE_FILES   := $(shell ls $(SRCDIR)/*.c)
OBJECT_FILES   := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SOURCE_FILES))

all: $(TARGET)

clean:
	rm -rf $(TARGET) $(OBJECT_FILES)

$(TARGET): $(OBJECT_FILES) $(LIBRARIES)
	mkdir -p $(BINDIR)
	$(CC) -o $@ $^

$(OBJECT_FILES): $(OBJDIR)/%.o : $(SRCDIR)/%.c $(INCLUDE_FILES1) $(INCLUDE_FILES2)
	mkdir -p $(OBJDIR)
	$(CC) -c -o $@ $< $(CFLAGS)
