# Stackwalker [![Build Status](https://travis-ci.org/NatronGitHub/stackwalker.svg)](https://travis-ci.org/NatronGitHub/stackwalker)


Stackwalker is used to dump symbols and process crash reports for MINGW, Linux and macOS.

*This is a fork of Google Breakpad and Mozilla Socorro with additional features/changes maintained for Natron.*

## Requirements

* pkg-config
* libcurl
* libzip
* jsoncpp

## Build

```
git submodule update -i --recursive
cd breakpad
./configure && make
cd ..
make -C minidump-stackwalk
```

This will create two binaries:

* ``breakpad/src/tools/<OS>/dump_syms``
* ``minidump-stackwalk/stackwalker``

## Usage

``dump_syms`` is used to create a symbol file for a given (not stripped) binary. Example:

```
dump_syms Natron-bin > Natron-bin.sym
```

``stackwalker`` is used to parse crash report dumps against symbols generated from ``dump_syms``. Example:

```
stackwalker CRASH.dmp <LOCAL_OR_REMOTE_SYMBOLS_DIR> > CRASH.json
```

## Symbols Storage

Symbols are stored using the following directory structure:

* Filename *(example: Natron-bin)*
  * ID *(example: 69CDA01A0F236F7C71CD19E5A20A21AC0)*
    * Filename.sym.zip *(example: Natron-bin.sym.zip)*

Filename and ID must match line 1 in the symbol file.

**Note that Stackwalker only supports zipped symbols (filename.sym.zip)**
