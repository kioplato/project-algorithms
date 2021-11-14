#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "../errors/errors.h"
#include "../fparser/converts.h"
#include "args.h"

struct args* args_init()
{
	struct args* args = malloc(sizeof(struct args));
	if (args == NULL) { perror("malloc"); exit(1); }

	err_code = LSH_OK;

	args->inputf = NULL;
	args->queryf = NULL;
	args->outputf = NULL;
	args->k = 4;
	args->L = 5;
	args->N = 1;
	args->R = 10000;

	return args;
}

static void print_usage()
{
	printf("./lsh [options]\n");
	printf("Available options:\n");
	printf("\t-i <input-file>\t: path to input file.\n");
	printf("\t-q <query-file>\t: path to query file.\n");
	printf("\t-o <output-file>: path to output file.\n");
	printf("\t-k <int>\t: number of LSH h fn for defining g fn.\n");
	printf("\t-L <int>\t: number of hashtables.\n");
	printf("\t-N <int>\t: number of nearest neighbors.\n");
	printf("\t-R <int>\t: number of radius.\n");
}

void args_tokenize(struct args* args, int argc, char* argv[])
{
	if (!args || !argc || !argv) { print_usage(); exit(1); }

	err_code = LSH_OK;

	for (unsigned c_arg = 1; c_arg < argc - 1; ++c_arg) {
		if (!strcmp("-i", argv[c_arg])) {
			args->inputf = malloc(strlen(argv[++c_arg]) + 1);
			strcpy(args->inputf, argv[c_arg]);
		}
		else if (!strcmp("-q", argv[c_arg])) {
			args->queryf = malloc(strlen(argv[++c_arg]) + 1);
			strcpy(args->queryf, argv[c_arg]);
		}
		else if (!strcmp("-o", argv[c_arg])) {
			args->outputf = malloc(strlen(argv[++c_arg]) + 1);
			strcpy(args->outputf, argv[c_arg]);
		}
		else if (!strcmp("-k", argv[c_arg]))
			args.k = atoi(argv[++c_arg]);
		else if (!strcmp("-L", argv[c_arg]))
			args.L = atoi(argv[++c_arg]);
		else if (!strcmp("-N", argv[c_arg]))
			args.N = atoi(argv[++c_arg]);
		else if (!strcmp("-R", argv[c_arg]))
			args.R = atoi(argv[++c_arg]);
		else {
			printf("Unknown argument \"%s\".\n", argv[c_arg]);
			print_usage();
			exit(1);
		}
	}
}

static char* read_reg_path(const char* str)
{
	while (1) {
		void* nldelim = nldelim();
		printf("%s", str);
		void* mem = fparse(stdin, convert_to_string, nldelim);
		char* path = *(mem + sizeof(unsigned));
		free(mem); free(nldelim);

		struct stat st;
		if (stat(path, &st) < 0)
		{ perror("Invalid path"); free(path); }
		else if (!S_ISREG(st.st_mode))
		{ printf("Invalid path: not a regular file"); free(path); }
		else return path;
	}
}

static char* read_unique_path(const char* str)
{
	while (1) {
		void* nldelim = nldelim();
		printf("%s", str);
		void* mem = fparse(stdin, convert_to_string, nldelim);
		char* path = *(mem + sizeof(unsigned));
		free(mem); free(nldelim);

		struct stat st;
		if (stat(path, &st) == 0)
		{ printf("Invalid path: file already exists") free(path); }
		else return path;
	}
}

void args_prompt(struct args* args)
{
	if (args == NULL) { err_code = LSH_INV_ARGS; return NULL; }

	err_code = LSH_OK;

	if (args->inputf == NULL)
		args->inputf = read_regular_path("Provide input file path: ");
	if (args->queryf == NULL)
		args->queryf = read_regular_path("Provide query file path: ");
	if (args->outputf == NULL)
		args->outputf = read_unique_path("Provide output file path: ");
}

void args_print(struct args* args)
{
	if (args == NULL) { err_code = LSH_INV_ARGS; return; }

	err_code = LSH_OK;

	if (args->inputf) printf("inputf = %s\n", args->inputf);
	if (args->queryf) printf("queryf = %s\n", args->queryf);
	if (args->outputf) printf("outputf = %s\n", args->outputf);
	printf("k = %u\n", args->k);
	printf("L = %u\n", args->L);
	printf("N = %u\n", args->N);
	printf("R = %u\n", args->R);
}

void args_free(struct args* args)
{
	err_code = LSH_OK;

	free(args->inputf);
	free(args->queryf);
	free(args->outputf);

	free(args);
}
