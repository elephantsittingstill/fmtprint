# `fmtprint` - A Less Feature-Full `printf`-Style Formatting Function in C 

---

### Overview
`fmtprint` is my first attempt at writing my own `printf` function in C.
It supports custom format strings with flags, width, precision, and 
most of the main specifiers. Its design is based, but does diverge from,
how the original `printf` is written.

---

### Features
- Supports common format specifiers:
    - `%d`
    - `%x`
    - `%o`
    - `%u`
    - `%p`
    - `%s`
    - `%c`
- Handles flags:
    - `-`
    - `0`
    - `#`
- Width and precision control (including `.*` for precision)

---

### Building
To build manually:
```bash
make lib        # builds libfmtprint.a
```
To use in your own project, copy include/fmtprint.h and libfmtprint.a 
into your project and compile accordingly:
```bash
# depending on your project structure, something like this
gcc -Iinclude -o prog main.c lib/libfmtprint.a
```

---

### Example Usage
```c
#include "fmtprint.h"
int main() {
    int value = 42;
    fmtprint("%d\n", value);
    fmtprint("Hex: %#x, Octal: %#o\n", value, value);
    return 0;
}
```

---

### API
`void fmtprint(const char *fmt, ...)`
Prints formatted output to `stdout`. Supports standard format
specifiers and flags.

---

### Format Specifier Support
| Specifier  | Meaning        | Supported |
| ---------- | -------------- | --------- |
| `%d`       | Signed decimal | ✅         |
| `%u`       | Unsigned int   | ✅         |
| `%x`     | Hexadecimal    | ✅         |
| `%o`       | Octal          | ✅         |
| `%f`       | Floating point (double)          | ✅         |
| `%p`       | Pointer        | ✅         |
| `%s`       | String         | ✅         |
| `%c`       | Char           | ✅         |
| `%%`       | Literal `%`    | ✅         |

For flags, width, precision:
- Flags: `-`, `#`, `0`
- Width: supported.
- Precision: supported (only numeric).
- Length modifiers: not supported.

---

### Limitations
- No wide character support.
- No locale or multithreading awareness.

---

### Directory Structure
```bash
fmtprint/
├── build/
│   ├── test_objects/
│   └── test executables...
├── include/
│   └── fmtprint.h
├── src/
│   ├── fmtprint.c
│   ├── fmtprint_lexer.c
│   ├── fmtprint_lexer.h
│   ├── fmtprint_specifier.c
│   ├── fmtprint_string.c
│   ├── fmtprint_string.h
│   └── fmtprint.h
├── test/
│   ├── run_tests.sh
│   ├── test_01_basic.c
│   ├── test_01_basic.out
│   ├── test_02_hex.c
│   ├── test_02_hex.out
│   └── a few other various tests...
├── Makefile
└── README.md
```

---

### Future Plans
- Add build environment with `Makefile`.
- Support aribitrary streams or buffers.
- Add more test cases.
- Potentially add compile-time checks for format string issues.
- Rewrite state machines using direct threading (computed gotos) 
for `gcc` and `clang` compilers.

---

### License
> MIT License
> Copyright (c) 2025 Eli Knutson
> Permission is hereby granted, free of charge, to any person
> obtaining a copy to use, destroy, make fun of, or make better
> this software.

---

### Author
[Eli Knutson](https://github.com/elephantsittingstill)
Made for learning and fun.
