# Project EOL, see https://github.com/rodlie/breakdown for future development

# Stackwalker [![Build Status](https://travis-ci.org/NatronGitHub/stackwalker.svg)](https://travis-ci.org/NatronGitHub/stackwalker)

The stackwalker project includes applications used to process crash reports from Breakpad.

## Requirements

* pkg-config
* libcurl
* libzip
* jsoncpp
* qtbase (Breakdown)
* cmake
* php (server-side)

### MSYS2

Add the required dependencies:
```
pacman -S tar git make mingw-w64-x86_64-{toolchain,cmake,curl,libzip,jsoncpp,qt5}
```

### MXE

Add the required dependencies:
```
make cc cmake jsoncpp curl libzip qtbase
```

## Applications

The stackwalker project contains the following applications:

### Breakdown

GUI and commandline application that can parse local and remote minidumps.

### Stackwalker

Commandline application that parses mindumps against symbols and outputs a JSON.

### dump_syms

WIN32 TODO. Need to port from Natron google-breakpad fork.

## Build

```
mkdir build && cd build
cmake ..
make
```

Use ``cmake -G"MSYS Makefiles" .. `` on MSYS2.

Tested on Linux and MXE/MSYS2 (MinGW).

## Usage

``dump_syms`` is used to create a symbol file for a given (not stripped) binary.

Example:

```
dump_syms Natron-bin > Natron-bin.sym
```

``stackwalker`` or ``breakdown`` is used to parse crash report dumps against symbols generated from ``dump_syms``.

Example:

```
stackwalker CRASH.dmp LOCAL_OR_REMOTE_PATH_TO_SYMBOLS_DIR > CRASH.json
```

```
breakdown CRASH.dmp > CRASH.json
```

Or open ``breakdown`` and view the report in a GUI.

## Symbols Storage

Symbols are stored using the following directory structure:

* Filename *(example: Natron-bin)*
  * ID *(example: 69CDA01A0F236F7C71CD19E5A20A21AC0)*
    * Filename.sym.zip *(example: Natron-bin.sym.zip)*

Filename and ID must match line 1 in the symbol file.

**Note that Stackwalker/Breakdown only supports zipped symbols (filename.sym.zip)**
