CFLAGS =-Wall -Wextra -Werror -Wpedantic -std=c99 -g
CC=clang $(CFLAGS)
OBJECTS = hatterspeak.o hash.o speck.o ll.o parser.o bf.o bv.o hs.o
EXECBIN = hatterspeak

.PHONY: all
all: $(EXECBIN)

$(EXECBIN):$(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXECBIN)

hatterspeak.o: hatterspeak.c bf.h hash.h hs.h parser.h
	$(CC) -c hatterspeak.c

hash.o: hash.c hash.h ll.h speck.h
	$(CC) -c hash.c

ll.o: ll.c ll.h hs.h
	$(CC) -c ll.c

speck.o: speck.c speck.h
	$(CC) -c speck.c

parser.o: parser.c parser.h
	$(CC) -c parser.c

bf.o: bf.c bf.h bv.h speck.h
	$(CC) -c bf.c

bv.o: bv.c bv.h
	$(CC) -c bv.c

hs.o: hs.c hs.h
	$(CC) -c hs.c

.PHONY: clean
clean:
	rm -f $(EXECBIN) *.o

.PHONY: valgrind
valgrind:
	make clean; make; valgrind ./$(EXECBIN) -s < hybrid_text.txt

.PHONY: infer
infer:
	make clean; infer-capture -- make; infer-analyze -- make
