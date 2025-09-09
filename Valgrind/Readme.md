# Valgrind Demo: Memcheck, Massif, Helgrind

This repository contains practical examples using three main tools of [Valgrind](https://valgrind.org/):

- **Memcheck**: Detects dynamic memory errors such as leaks, invalid access, invalid free, use of uninitialized data, etc.
- **Massif**: Analyzes and measures heap usage (heap profiler).
- **Helgrind**: Detects race conditions in multithreaded programs.

## Folder Structure

```
Memcheck/
    accessInvalidMemory/
    alignmentError/
    IllegalFree/
    InvalidMemoryArgument/
    memoryLeak/
    mismatchMemoryManage/
    overlapMemoryArgument/
    useUninitializedData/
Massif/
    main.c
    massif.out.*
Helgrind/
    main.c
    valgrind.log
```

## Usage Guide

### 1. Memcheck

The examples in the `Memcheck` folder illustrate common memory management errors:

- **accessInvalidMemory**: Invalid memory access (out-of-bounds, use-after-free).
- **alignmentError**: Memory alignment errors during allocation.
- **IllegalFree**: Invalid memory free (double free, freeing non-original pointer).
- **InvalidMemoryArgument**: Invalid arguments for memory allocation functions.
- **memoryLeak**: Various types of memory leaks (definitely, indirectly, possibly lost, still reachable).
- **mismatchMemoryManage**: Mismatched memory management (new/delete, malloc/free).
- **overlapMemoryArgument**: Overlapping memory arguments (overlapping memcpy).
- **useUninitializedData**: Use of uninitialized data.

**Run Memcheck:**
```sh
valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes --log-file=valgrind.log ./your_program
```
- `--leak-check=full`: Detailed memory leak check.
- `--show-leak-kinds=all`: Show all types of memory leaks.
- `--track-origins=yes`: Track the origin of uninitialized values.
- `--log-file=valgrind.log`: Save the output to a log file.

Check the `valgrind.log` file in each folder for the results.

### 2. Massif

The example in [Massif/main.c](Massif/main.c) is used to analyze heap usage.

**Run Massif:**
```sh
valgrind --tool=massif ./your_program
ms_print massif.out.<pid>
```
See [massif.out.108784](Massif/massif.out.108784) for sample output.

### 3. Helgrind

The example in [Helgrind/main.c](Helgrind/main.c) demonstrates a race condition with a global variable.

**Run Helgrind:**
```sh
valgrind --tool=helgrind ./your_program
```
See [valgrind.log](Helgrind/valgrind.log) for the results.

## Requirements

- GCC to compile C/C++ programs
- Valgrind installed on your system

## References

- [Valgrind Documentation](https://valgrind.org/docs/manual/manual.html)
- [Valgrind Quick Start](https://valgrind.org/docs/manual/quick-start.html)

---

