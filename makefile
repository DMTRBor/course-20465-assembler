# Compiler and flags
CC = gcc
CFLAGS = -Wall -ansi -pedantic -g

# Directories
DATA_STRUCTS_SRC = data_structs/src
DATA_STRUCTS_HDR = data_structs/hdr
ENCODING_SRC = encoding/src
ENCODING_HDR = encoding/hdr
MACHINE_SRC = machine/src
MACHINE_HDR = machine/hdr
PROCESSING_SRC = processing/src
PROCESSING_HDR = processing/hdr
UTILS_SRC = utils/src
UTILS_HDR = utils/hdr

# Include directories
INCLUDES = -I$(DATA_STRUCTS_HDR) -I$(ENCODING_HDR) -I$(MACHINE_HDR) -I$(PROCESSING_HDR) -I$(UTILS_HDR)

# Source files
SOURCES = assembler.c \
          $(DATA_STRUCTS_SRC)/memory_table.c \
          $(DATA_STRUCTS_SRC)/macro_list.c \
          $(DATA_STRUCTS_SRC)/lines_list.c \
          $(DATA_STRUCTS_SRC)/labels_table.c \
          $(ENCODING_SRC)/util.c \
          $(ENCODING_SRC)/data_section.c \
          $(ENCODING_SRC)/code_section.c \
          $(MACHINE_SRC)/machine.c \
          $(PROCESSING_SRC)/pre_assembler.c \
          $(PROCESSING_SRC)/first_pass.c \
          $(PROCESSING_SRC)/second_pass.c \
          $(UTILS_SRC)/utils.c \
          $(UTILS_SRC)/errors_handling.c \
          $(UTILS_SRC)/build_output.c

# Object files
OBJECTS = $(SOURCES:.c=.o)

# Target executable
TARGET = assembler

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ -lm

# Compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJECTS) $(TARGET) *.am *.ob *.ent *.ext

# Phony targets
.PHONY: all clean

# Dependencies
assembler.o: assembler.c $(PROCESSING_HDR)/pre_assembler.h $(PROCESSING_HDR)/first_pass.h $(PROCESSING_HDR)/second_pass.h

$(DATA_STRUCTS_SRC)/memory_table.o: $(DATA_STRUCTS_SRC)/memory_table.c $(DATA_STRUCTS_HDR)/mem_unit.h
$(DATA_STRUCTS_SRC)/macro_list.o: $(DATA_STRUCTS_SRC)/macro_list.c $(DATA_STRUCTS_HDR)/macro.h
$(DATA_STRUCTS_SRC)/lines_list.o: $(DATA_STRUCTS_SRC)/lines_list.c $(DATA_STRUCTS_HDR)/line.h
$(DATA_STRUCTS_SRC)/labels_table.o: $(DATA_STRUCTS_SRC)/labels_table.c $(DATA_STRUCTS_HDR)/label.h

$(ENCODING_SRC)/util.o: $(ENCODING_SRC)/util.c $(ENCODING_HDR)/util.h
$(ENCODING_SRC)/data_section.o: $(ENCODING_SRC)/data_section.c $(ENCODING_HDR)/data_section.h
$(ENCODING_SRC)/code_section.o: $(ENCODING_SRC)/code_section.c $(ENCODING_HDR)/code_section.h

$(MACHINE_SRC)/machine.o: $(MACHINE_SRC)/machine.c $(MACHINE_HDR)/machine.h

$(PROCESSING_SRC)/pre_assembler.o: $(PROCESSING_SRC)/pre_assembler.c $(PROCESSING_HDR)/pre_assembler.h
$(PROCESSING_SRC)/first_pass.o: $(PROCESSING_SRC)/first_pass.c $(PROCESSING_HDR)/first_pass.h
$(PROCESSING_SRC)/second_pass.o: $(PROCESSING_SRC)/second_pass.c $(PROCESSING_HDR)/second_pass.h

$(UTILS_SRC)/utils.o: $(UTILS_SRC)/utils.c $(UTILS_HDR)/utils.h
$(UTILS_SRC)/errors_handling.o: $(UTILS_SRC)/errors_handling.c $(UTILS_HDR)/errors_handling.h
$(UTILS_SRC)/build_output.o: $(UTILS_SRC)/build_output.c $(UTILS_HDR)/build_output.h
