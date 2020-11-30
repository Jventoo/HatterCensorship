# Assignment 6 - Jack Vento
## The Program
Hatterspeak is a program that checks if supplied input (IO redirection supported) is part of a set of "forbidden oldspeak words" (the list of words in oldspeak.txt) or if it is forbidden, but has a translation into hatterspeak (the list of word pairs in hatterspeak.txt). The program will dynamically check these files at runtime, allowing the lists to be customized to whatever the user wants.

It features a bloom filter (BF) for quickly checking against massive amounts of data, a hash table (HT) for rapidly finding translations, linked list (LL) chaining for hash table values, and regex parsing.

These are five possible arguments:
* -s: Suppresses the censor letter and instead prints statistics for the following:
    * Seeks: number of seeks (lookups) performed.
    * Average seek length: LL nodes searched / total seeks.
    * Average LL length: average length of non-zero LLs in the Hash Table.
    * Hash Table load: percentage of loading of the HT.
    * Boom Filter load: percentage of loading of the BF.
* -h size: Specifies the length of the hash table to be _size_ entries (default: 10000).
* -f size: Specifies the length of the bloom filter to be _size_ entries (default: 2<sup>20</sup> = 1,048,576).
* -m: Moves LL nodes to the front of the LL whenever they are searched for (_moveToFront_ = true)
* -b: Does not do the above (_moveToFront_ = false).

Run the program using `./hatterspeak <arguments>` after compiling. Only -m and -b are mutually exclusive; the rest of the arguments may be specified in any order.

## Makefile
* `make` and `make all` will compile the program to an executable named "*hatterspeak*".
* `make clean` removes all compiler-generated files.
* `make valgrind` checks for memory management errors.
* `make infer` checks for common runtime errors using Infer, though it is not always correct. With this program, infer generates no errors.
