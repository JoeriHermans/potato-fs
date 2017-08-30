CC = gcc
LD = g++

SHELL = bash
MODULES = ai application building controller d
LDFLAGS = -lm -lz -lpthread -lc -o potato
CCFLAGS = -I"src/" -c -Wall -std=c11 -D_FILE_OFFSET_BITS=64

SOURCEDIR = src/
BUILDDIR = build/

SOURCES = $(shell cd $(SOURCEDIR) && find potato/ -name "*.c" && cd ..)
OBJECTS = $(addprefix build/, $(SOURCES:.c=.out))

NO_COLOR = \x1b[0m
YELLOW_COLOR = \x1b[33;01m
GREEN_COLOR = \x1b[32;01m
BOLD = \033[1m
NO_BOLD = \033[0m
OK_STRING = $(BOLD)$(GREEN_COLOR)[OK]$(NO_COLOR)$(NO_BOLD)
ARROW = $(YELLOW_COLOR)=>$(NO_COLOR)

.DEFAULT: all

all: potato

potato: $(OBJECTS)
	@mkdir -p build
	@echo -e "$(ARROW) Linking code."
	@$(LD) $(LDFLAGS) -o $@ $^
	@mkdir -p bin/
	@rm -f bin/potato
	@mv potato bin/potato
	@echo -e "$(BOLD)DONE$(NO_BOLD)"

build/%.out: src/%.c
	@mkdir -p $(dir $(OBJECTS))
	@echo -en "$(ARROW) $(BOLD)Compiling:$(NO_BOLD) "$(word 2,$@ $<)
	@$(CC) $(CCFLAGS) -o $@ $<
	@echo -e " $(OK_STRING)"

clean:
	@rm -f -r build
	@rm -f -r bin

update_binary:
	@cp bin/potato /usr/bin/potato
install:
	@make -s update_binary
