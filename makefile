# compiler and flags
CC = gcc
CFLAGS = -Wall -ansi -pedantic -g

# include headers
INCLUDES = -Idata_structs/hdr -Iencoding/hdr -Imachine/hdr -Iprocessing/hdr -Iutils/hdr

# source files
SOURCES = assembler.c \
          data_structs/src/memory_table.c \
          data_structs/src/macro_list.c \
          data_structs/src/lines_list.c \
          data_structs/src/labels_table.c \
          encoding/src/util.c \
          encoding/src/data_section.c \
          encoding/src/code_section.c \
          machine/src/machine.c \
          processing/src/pre_assembler.c \
          processing/src/first_pass.c \
          processing/src/second_pass.c \
          utils/src/utils.c \
          utils/src/errors_handling.c \
          utils/src/build_output.c

# object files
OBJECTS = $(SOURCES:.c=.o)

# executable
EXEC = assembler

# build executable
$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ -lm

# compile source files to object files
%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# remove files on rebuild
clean:
	rm -rf $(OBJECTS) $(EXEC) *.am *.ob *.ent *.ext

# add dependencies
assembler.o: assembler.c processing/hdr/pre_assembler.h processing/hdr/first_pass.h processing/hdr/second_pass.h

data_structs/src/memory_table.o: data_structs/src/memory_table.c data_structs/hdr/mem_unit.h
data_structs/src/macro_list.o: data_structs/src/macro_list.c data_structs/hdr/macro.h
data_structs/src/lines_list.o: data_structs/src/lines_list.c data_structs/hdr/line.h
data_structs/src/labels_table.o: data_structs/src/labels_table.c data_structs/hdr/label.h

encoding/src/util.o: encoding/src/util.c encoding/hdr/util.h
encoding/src/data_section.o: encoding/src/data_section.c encoding/hdr/data_section.h
encoding/src/code_section.o: encoding/src/code_section.c encoding/hdr/code_section.h

machine/src/machine.o: machine/src/machine.c machine/hdr/machine.h

processing/src/pre_assembler.o: processing/src/pre_assembler.c processing/hdr/pre_assembler.h
processing/src/first_pass.o: processing/src/first_pass.c processing/hdr/first_pass.h processing/hdr/passes.h
processing/src/second_pass.o: processing/src/second_pass.c processing/hdr/second_pass.h processing/hdr/passes.h

utils/src/utils.o: utils/src/utils.c utils/hdr/utils.h
utils/src/errors_handling.o: utils/src/errors_handling.c utils/hdr/errors_handling.h
utils/src/build_output.o: utils/src/build_output.c utils/hdr/build_output.h
