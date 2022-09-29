#!/bin/bash
assert() {
  expected="$1"
  input="$2"

  ./9cc "$input" > tmp.s
  cc -o tmp tmp.s
  ./tmp
  actual="$?"

  if [ "$actual" = "$expected" ]; then
    echo "$input => $actual"
  else
    echo "$input => $expected expected, but got $actual"
    exit 1
  fi
}

assert 0 'test/step1/test1-1.c'
assert 42 'test/step1/test1-2.c'

assert 21 'test/step2/test2-1.c'

assert 41 'test/step3/test3-1.c'

assert 47 'test/step5/test5-1.c'
assert 15 'test/step5/test5-2.c'
assert 4 'test/step5/test5-3.c'
assert 17 'test/step5/test5-4.c'

assert 10 'test/step6/test6-1.c'
assert 60 'test/step6/test6-2.c'

assert 0 'test/step7/test7-1.c'
assert 1 'test/step7/test7-2.c'
assert 1 'test/step7/test7-3.c'
assert 0 'test/step7/test7-4.c'
assert 1 'test/step7/test7-5.c'
assert 0 'test/step7/test7-6.c'
assert 0 'test/step7/test7-7.c'
assert 1 'test/step7/test7-8.c'
assert 1 'test/step7/test7-9.c'
assert 0 'test/step7/test7-10.c'
assert 1 'test/step7/test7-11.c'
assert 0 'test/step7/test7-12.c'
assert 0 'test/step7/test7-13.c'
assert 1 'test/step7/test7-14.c'
assert 1 'test/step7/test7-15.c'
assert 0 'test/step7/test7-16.c'

assert 10 'test/step9/test9-1.c'
assert 20 'test/step9/test9-2.c'

assert 6 'test/step10/test10-1.c'

assert 6 'test/step11/test11-1.c'

assert 6 'test/step12/test12-1.c'
assert 6 'test/step12/test12-2.c'
assert 4 'test/step12/test12-3.c'
assert 4 'test/step12/test12-4.c'
assert 4 'test/step12/test12-5.c'
assert 5 'test/step12/test12-6.c'
assert 10 'test/step12/test12-7.c'

assert 4 'test/step13/test13-1.c'
assert 4 'test/step13/test13-2.c'
assert 110 'test/step13/test13-3.c'

echo OK