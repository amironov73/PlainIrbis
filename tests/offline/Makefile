#########################
# Common support library
#########################

.PHONY: all clean

include ../../_make/common

APPLICATION_NAME := offline
INCLUDE_DIR1     := $(PWD)/include
INCLUDE_DIR2     := $(PWD)/../../include
TARGET           := $(BINDIR)/$(APPLICATION_NAME)

CFLAGS := $(CFLAGS) -I $(INCLUDE_DIR1) -I $(INCLUDE_DIR2)

INCLUDE_FILES1 := $(shell ls $(INCLUDE_DIR1)/*.h)
INCLUDE_FILES2 := $(shell ls $(INCLUDE_DIR2)/magna/*.h)

all: $(TARGET)

clean:
	rm -rf $(TARGET) $(OBJECT_FILES)

$(TARGET): $(OBJECT_FILES) $(LIBRARIES)
	@mkdir -p $(BINDIR)
	$(CC) -o $@ $^

$(OBJECT_FILES): $(OBJDIR)/%.o : $(SRCDIR)/%.c $(INCLUDE_FILES1) $(INCLUDE_FILES2)
	@mkdir -p $(OBJDIR)
	$(CC) -c -o $@ $< $(CFLAGS)
