How to run:
    $ make
    $ gcc main.c cache.c ram.c set.c -o main -lm

Flags:
    Flags orders' are not important.
    -r ram file (default RAM.dat)
    And all flags that in the pdf

Example Run:
    $ ./main -L1s 4 -L1E 1 -L1b 4 -L2s 8 -L2E 2 -L2b 4 -t test_large.trace -r RAM.dat