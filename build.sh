#!/bin/sh
CWD=`pwd`
git submodule update -i
cp -a "$CWD/lss" "$CWD/breakpad/src/third_party/" || exit 1
cd "$CWD/breakpad";./configure && make -j8 || exit 1
cd "$CWD/minidump-stackwalk";make || exit 1
mv "$CWD/minidump-stackwalk/stackwalker" "$CWD/" || exit 1
