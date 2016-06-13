#!/bin/sh
CWD=`pwd`
git submodule update -i --recursive
cd "$CWD/breakpad";./configure && make -j8 || exit 1
cd "$CWD/minidump-stackwalk";make || exit 1
