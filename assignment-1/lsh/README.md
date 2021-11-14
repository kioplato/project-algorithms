# Project Algorithms - Assignment 1 - Question LSH

Locality Sensitive Hashing.  
Provided a dataset of vectors, index it and perform queries on it.

## Directory structure

- bin/: where binary is stored.
- build/: where object files are stored.
- makefile: config file for `make` utility.
- README.md: file containing information subquestion LSH.
- src/: contains the source code.
    - args/: API for reading cli arguments.
    - avl-tree/: AVL tree implementation.
    - errors/: LSH related errors.
    - fparser/: API for reading streams.
    - hashtable/: Hashtable implementation.
    - linked-list/: Linked list implementation.
    - lsh/: lsh implementation.
    - main.c: main function.
    - vector/: API for vectors.
- tests/: contains tests.

## Compilation

`make`: create binary in `bin/` and object files in `build/`.  
`make clean`: removes binary from `bin/` and object files from `build/`.

## Usage

`./lsh` will give default values to program parameters and prompt for others.  
`./lsh -i <input-file> -q <query-file> -k <int> -L <int> -o <output-file> -N
<number-of-nearest> -R <radius>`  
You can specify any number of parameters through command line arguments. User
will be prompted for the missing ones. Incorrect parameters or unrecognized ones
raise an error and program's usage is printed to `stdout`.

## Implementation details

- Hashtables use separate chaining using AVL trees for collision handling.
- Calculate mean distance between vectors using subset of log^2(vectors).
