#!/bin/sh
SW="../minidump-stackwalk/stackwalker"
SYM="symbols"
DUMPS="39eb2108-ca98-7946-09d36d98-51c56b32 6537E627-678E-44B3-9116-36323C0BA4CC f97fd058-fb43-4098-b770-7fce7a3fb757"
for i in $DUMPS;do
  #echo "Testing $i"
  #rm -f $i.tmp || true
  $SW --pretty --symbols-url http://fxarena.net/natron/symbols --symbols-cache /tmp $i.dmp #> $i.tmp
  #DIFF=`diff $i.json $i.tmp`
  #if [ ! -z "$DIFF" ]; then
  #  echo "$i: FAILED"
  #  echo "$DIFF"
  #  exit 1
  #else
  #  echo "$i: PASSED"
  #fi
  #rm -f $i.tmp || true
done
#echo "Tests done"
#exit 0
