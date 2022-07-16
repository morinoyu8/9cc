CFLAGS=-std=c11 -g -static
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)
UNAME=$(shell uname -m)
DIR=$(dir $(abspath $(lastword $(MAKEFILE_LIST))))

9cc: $(OBJS)
	$(CC) -o 9cc $(OBJS) $(LDFLAGS)

$(OBJS): 9cc.h

test: 9cc
	chmod a+x test.sh
	./test.sh

clean:
	rm -f 9cc *.o *~ tmp*

docker:
ifeq ($(UNAME), arm64)
	docker run --rm -it -v $(DIR):/home/user/9cc compilerbook_x86_64
else
	docker run --rm -it -v $(DIR):/home/user/9cc compilerbook
endif

.PHONY: test clean