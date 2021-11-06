#include <stdio.h>
#include <stdlib.h>

#include "args/args.h"
#include "fparse/converts.h"

int main(int argc, char* argv[])
{
	// Read the command line arguments and missing mandatory ones.
	struct args* args = args_init();
	args_tokenize(args, argc, argv);
	args_prompt(args);

	// Read the input dataset file.
	FILE* fp = fopen(args->inputf, "r");
	void* nldelim = nldelim();
	void* mem = fparse(fp, convert_to_vector, nldelim);
	free(nldelim);
	fclose(fp);

	struct vector* vectors[] = mem + sizeof(unsigned);
	unsigned n_vectors = *(unsigned*)mem;

	// LSH.
}
