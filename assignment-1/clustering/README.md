# Project Algorithms - Assignment 1 - Question Clustering

Clustering using k-means, Lloyd and reverse assignment using LSH and hypercube.  
Provided a dataset of vectors, cluster them and evaluate.

## Directory structure

- bin/: where binary is stored.
- build/: where object files are stored.
- include/: where compiled library headers are stored.
- lib/: where compiled libraries are stored.
- makefile: config file for `make` utility.
- README.md: file containing information subquestion clustering.
- src/: contains the source code.
    - args/: API for reading cli arguments.
    - avl-tree/: AVL tree implementation.
    - cluster/: API for clustering.
    - errors/: clustering related errors.
    - fparser/: API for reading streams.
    - hashtable/: Hashtable implementation.
    - linked-list/: Linked list implementation.
    - lloyd/: Lloyd implementation.
    - main.c: main function.
    - vector/: API for vectors.
- tests/: contains tests.

## Compilation

`make`: create binary in `bin/` and object files in `build/`.  
`make clean`: removes binary from `bin/` and object files from `build/`.

## Usage

`./cluster` will give default values to program parameters and prompt for others.  
`./cluster -i <input-file> -c <config-file> -o <out-file> -complete <optional> -m <method>`  
You can specify any number of parameters through command line arguments. User
will be prompted for the missing ones. Incorrect parameters or unrecognized ones
raise an error and program's usage is printed to `stdout`.

## Implementation details

- Hashtables use separate chaining using AVL trees for collision handling.
- Calculate mean distance between vectors using subset of log^2(vectors).
- Use LSH and hypercube random projection as shared objects.
