LIBPS4  := libPS4
CC      := gcc
AS      := gcc
OBJCOPY := objcopy
ODIR    := build
SDIR    := source
IDIRS   := -I$(LIBPS4)/include -I. -Iinclude
LDIRS   := -L$(LIBPS4) -L. -Llib
CFLAGS  := $(IDIRS) -O2 -std=c11 -ffunction-sections -fdata-sections -fno-builtin -nostartfiles -nostdlib \
		   -Wall -masm=intel -march=btver2 -mtune=btver2 -m64 -mabi=sysv -mcmodel=small  -fpie
SFLAGS  := -nostartfiles -nostdlib -march=btver2 -mtune=btver2 -m64 -mabi=sysv -mcmodel=large
LFLAGS  := $(LDIRS) -Xlinker -T $(LIBPS4)/linker.x -Wl,--build-id=none
CFILES  := $(wildcard $(SDIR)/*.c)
SFILES  := $(wildcard $(SDIR)/*.s)
OBJS    := $(patsubst $(SDIR)/%.c, $(ODIR)/%.o, $(CFILES)) \
 		   $(patsubst $(SDIR)/%.s, $(ODIR)/%.o, $(SFILES))

LIBS    := -lPS4

TARGET = $(shell basename $(CURDIR)).bin

all: $(TARGET)

$(TARGET): makeGtaPayload requiredClean $(ODIR) $(OBJS)
	@echo "Linking $(TARGET)..."
	@$(CC) $(LIBPS4)/crt0.s $(ODIR)/*.o -o temp.t $(CFLAGS) $(LFLAGS) $(LIBS)
	@$(OBJCOPY) -O binary temp.t $(TARGET)
	@rm -f temp.t
	@echo "$(TARGET) successfully created!"

$(ODIR)/%.o: $(SDIR)/%.c
	@echo "Compiling $<..."
	@$(CC) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: $(SDIR)/%.s
	@echo "Compiling $<..."
	@$(AS) -c -o $@ $< $(SFLAGS)

$(ODIR):
	@mkdir -p $@
	@echo "Created build directory: $(ODIR)"

.PHONY: makeGtaPayload requiredClean

makeGtaPayload:
	@$(MAKE) -C gtaPayload $?

requiredClean:
	@rm -f $(TARGET) $(ODIR)/embed.o

clean:
	@echo "Cleaning..."
	@rm -f $(TARGET) $(ODIR)/*.o
	@echo "Clean completed."
