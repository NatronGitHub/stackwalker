#!/bin/sh
SW="../minidump-stackwalk/stackwalker"
DUMPS="2f5d2700-d191-9876-430fd7b8-3825119e"
for i in $DUMPS;do
  echo "Testing $i"
  rm -f $i.tmp || true
  $SW --pretty $i.dmp symbols > $i.tmp
  DIFF=`diff $i.json $i.tmp`
  if [ ! -z "$DIFF" ]; then
    echo "$i: FAILED"
    echo "$DIFF"
    exit 1
  else
    echo "$i: PASSED"
  fi
  rm -f $i.tmp || true
done
