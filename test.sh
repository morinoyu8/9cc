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

<< COMMENTOUT

assert 0 '0;'
assert 42 '42;'
assert 21 "5+20-4;"
assert 41 " 12 + 34 - 5 ;"
assert 47 '5+6*7;'
assert 15 '5*(9-6);'
assert 4 '(3+5)/2;'
assert 17 '4*5-(3+7)*2/4+(3+5)-2*3;'
assert 10 '-10+20;'
assert 60 '+45-(-3)*5;'

assert 0 '0==1;'
assert 1 '42==42;'
assert 1 '0!=1;'
assert 0 '42!=42;'

assert 1 '0<1;'
assert 0 '1<1;'
assert 0 '2<1;'
assert 1 '0<=1;'
assert 1 '1<=1;'
assert 0 '2<=1;'

assert 1 '1>0;'
assert 0 '1>1;'
assert 0 '1>2;'
assert 1 '1>=0;'
assert 1 '1>=1;'
assert 0 '1>=2;'


計算結果は255以下となるように

COMMENTOUT

assert 10 'a=5;a=a+5;'
assert 6 'foo = 1; bar = 2 + 3; foo = foo + bar;'
assert 6 'foo = 1; bar = 2 + 3; return foo + bar;'
assert 6 'foo = 1; if (foo < 2) foo = foo + 5; return foo;'
assert 6 'foo = 6; if (foo < 2) foo = foo + 5; return foo;'
assert 4 'foo = 6; if (foo < 2) foo = foo + 5; else foo = foo - 2; return foo;'
assert 4 'foo = 1; while (foo < 4) foo = foo + 1; return foo;'
assert 4 'foo = 1; bar = 0; while (foo < 4) if (bar == 0) foo = foo + 1; else foo = foo + 2; return foo;'
assert 5 'foo = 1; bar = 1; while (foo < 4) if (bar == 0) foo = foo + 1; else foo = foo + 2; return foo;'
assert 10 'foo = 0; for (i = 0; i < 5; i = i + 1) foo = foo + i; return foo;'
assert 4 'foo = 1; bar = 0; while (foo < 4) { if (bar == 0) { foo = foo + 1; } else { foo = foo + 2; } } return foo;'
assert 4 'foo = 1; bar = 0; while (foo < 4) { if (bar == 0) { foo = foo + 1; } else { foo = foo + 1; foo = foo + 1; } } return foo;'
assert 110 'foo = 0; bar = 0; baz = 0; for (i = 0; i < 10; i = i + 1) { foo = foo + 1; bar = bar + 1; baz = baz + foo + bar; } return baz;'

echo OK