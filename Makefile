CFLAGS=-std=c11 -g -static

docker:
	docker run --rm -it -v $$HOME/Desktop/9cc:/home/user/9cc compilerbook

9cc: 9cc.c

test: 9cc
	chmod a+x test.sh
	./test.sh

clean:
	rm -f 9cc *.o *~ tmp*

.PHONY: test clean