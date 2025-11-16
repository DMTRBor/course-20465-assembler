# 20465 System Programming Laboratory Final Project (sem. 2025-B) - Assembler

### Overview
- For detailed description, see [Assignment 14](doc/assignment_14.pdf)


### Requirements
C Standard
- ANSI C(89/90)

Linux
- Ubuntu >= 16.04
- Install gcc and make
```
sudo apt update
sudo apt install build-essential
```

Windows
- Windows 10/11
- MSVC 2022


### Build Instructions
Linux
```
make clean
make
```

Windows
```
# Activate the Visual Studio build environment for x64 architecture.
call "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" x64

# Compile all C sources into assembler.exe
cl /nologo /W4 /EHsc /std:c89 /Fe:assembler.exe ^
    assembler.c ^
    data_structs/src/memory_table.c ^
    data_structs/src/macro_list.c ^
    data_structs/src/lines_list.c ^
    data_structs/src/labels_table.c ^
    encoding/src/util.c ^
    encoding/src/data_section.c ^
    encoding/src/code_section.c ^
    machine/src/machine.c ^
    processing/src/pre_assembler.c ^
    processing/src/first_pass.c ^
    processing/src/second_pass.c ^
    utils/src/utils.c ^
    utils/src/errors_handling.c ^
    utils/src/build_output.c
```


### Usage
- The assembler source file names passed without extension

- Use command line:

Linux
```
./assembler <path/to/>file1 <path/to/>file2 <path/to/>file3
```

Windows
```
assembler.exe <path/to/>file1 <path/to/>file2 <path/to/>file3
```