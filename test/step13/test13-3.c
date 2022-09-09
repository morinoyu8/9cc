foo = 0; 
bar = 0; 
baz = 0; 
for (i = 0; i < 10; i = i + 1) { 
    foo = foo + 1; 
    bar = bar + 1; 
    baz = baz + foo + bar; 
} 
return baz;