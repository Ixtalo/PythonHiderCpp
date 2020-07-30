# PythonHiderCpp

Hides Python code in a C++ binary.
The Python code is encrypted using [AES (Advanced Encryption Standard)](https://en.wikipedia.org/wiki/Advanced_Encryption_Standard).
It offers some form of reverse engineering protection.



## Motivation
Sometimes you want to hide your precious Python code when distributing your program. 
But the interpreted Python language does not allow to "hide" your code.

Compiled Python files (.pyc/.pyo) files do show parts of the Python code in plain text and can be easily decompiled.


## Requirements

- Python 3.4+
- Python Development headers (`python3-dev` package)
- CMake 3.14+
- C++ compiler such as GNU G++
- C++ AES implementation from https://github.com/SergeyBel/AES
  - Tested with tag ["v1.0.0"](https://github.com/SergeyBel/AES/releases/tag/v.1.0.0), revision [3c750a1](https://github.com/SergeyBel/AES/commit/3c750a190e2fea44fed702744b17acc8eb1e4557)


## How-To Run

1. initialize git submodule:
   `git submodule init && git submodule update --recursive`
2. cmake initialization
   `mkdir build && pushd build && cmake ..`
3. compile:
   `make clean all `
4. encrypt Python code:
   `./gendatah KEY PYTHONFILE.py`
    * KEY must have a length of 16 characters!
5. rebuild:
   `make clean all`
6. run:
   `./runner`

After each change to your `PYTHONFILE.py` you have to do:
1. `./gendatah KEY PYTHONFILE.py`
   generates new `data.h`
2. `make clean all`
   recompiles `runner` binary; make sure to also do a `make clean`!



## Technical Overview
This PythonHiderCpp consists of the two command line interface (CLI) C++ programs `gendatah.cpp` and `runner.cpp` which have to compiled to binaries.

- `gendatah.cpp` CLI program
  - AES encryptes Python file with given key  (using AES EBC mode)
  - generates the `data.h` header which includes
    - the key (as a C++ macro)
    - the encrypted data

- `runner.cpp` CLI program
  - The encrypted data (by `gendatah` is compiled into this binary.
  - **Attention: this private key is also stored in the binary!** It is somewhat hidden, not visible as plain text, but can be retrieved, e.g., memory analysis.
  - It uses the [Python/C API](https://docs.python.org/3/c-api/index.html) and its [high level layer](https://docs.python.org/3/c-api/veryhigh.html) to run the Python code. Specifically it uses [PyRun_SimpleString](https://docs.python.org/3/c-api/veryhigh.html#c.PyRun_SimpleString) to run the Python code from an in-memory string.



## Limitations - Private Key Problem

The private key is also stored inside the compiled binary. This is not safe because the key could be found!
PythonHiderCpp does make some efforts to hide the key, but nonetheless it can be found.



## Used Libraries

- C++ AES implementation by Sergey Bel, https://github.com/SergeyBel/AES, MIT license



## Other Solutions

There are otehr solutions which do compile Python code to binaries, but most of them first extract the code to a temporary folder where it is visible again.

- py2app
- cx_freeze
- pyinstaller


## License
Copyright (C) 2020 by Ixtalo.
AGPL3 license.
