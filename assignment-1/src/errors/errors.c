#include <stdio.h>

#include "errors.h"

int err_code = LSH_OK;

char* lsh_strerr(int code)
{
	err_code = LSH_OK;

	if (code == LSH_OK)
		return "success";

	if (code == LSH_INV_ARGS)
		return "invalid arguments";

	if (code == LSH_INV_CLI_ARGS)
		return "invalid command line arguments";

	err_code = LSH_INV_ARGS;
	return NULL;
}

void lsh_perror(char* str)
{
	err_code = LSH_OK;

	if (str == NULL) str = "Error";
	printf("%s: %s", str, lsh_strerr(err_code));
}
