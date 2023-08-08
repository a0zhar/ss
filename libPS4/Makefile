# Compiler (we use GCC)
COMPILER := gcc
# A utility for creating, modifying and extracting from archives
AR	     := ar
# Output Directory containing the object files (.o)
ODIR     := build
# Directory containing the source files (.c)
SOURCE   := source
# Directory containing the header files (.h)
HEADER   := include
# The flags which we will pass to our compiler
CFLAGS   := -I$(HEADER) -Os -std=c11 -ffunction-sections -fdata-sections -fno-builtin -nostartfiles -nostdlib -Wall \
			-Wextra -masm=intel -march=btver2 -mtune=btver2 -m64 -mabi=sysv -mcmodel=small -fpie -fPIC
LDFLAGS  := -Wl,--gc-sections
CFILES   := $(wildcard $(SOURCE)/*.c)
SFILES   := $(wildcard $(SOURCE)/*.s)
OBJS     := $(patsubst $(SOURCE)/%.c, $(ODIR)/%.o, $(CFILES)) $(patsubst $(SOURCE)/%.s, $(ODIR)/%.o, $(SFILES))

TARGET = libPS4.a

$(TARGET): $(ODIR) $(OBJS)
	@echo "Creating $(TARGET)..."
	@$(AR) rcs $@ $(OBJS)
	@echo "$(TARGET) created successfully!"

$(ODIR)/%.o: $(SOURCE)/%.c
	@echo "Compiling $<..."
	@$(COMPILER) -c -o $@ $< $(CFLAGS)

$(ODIR)/%.o: $(SOURCE)/%.s
	@echo "Compiling $<..."
	@$(COMPILER) -c -o $@ $< $(CFLAGS)

$(ODIR):
	@mkdir -p $@
	@echo "Created directory: $(ODIR)"

.PHONY: clean

clean:
	@echo "Cleaning..."
	@rm -rf $(TARGET) $(ODIR)
	@echo "Clean completed."
