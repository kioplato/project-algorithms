#ifndef ERRORS_H
#define ERRORS_H

enum {
	LSH_OK,  // Successful execution.

	LSH_INV_ARGS,      // Invalid function arguments.
	LSH_INV_CLI_ARGS,  // Invalid command line arguments.
};

extern int err_code;

/*
 * Returns the string describing the error code passed.
 *
 * Return value:
 * Success: String describing the error code passed, `err_code` is set.
 * Failure: NULL is returned and `err_code` is set accordingly.
 */
char* lsh_strerr(int err_code);

/*
 * Prints to `stdout` the string describing current `err_code`.
 * `str` is printed before the description string along with a colon character.
 *
 * Return value:
 * Always succeeds, sets `err_code`.
 */
void lsh_perror(char* str);

#endif  // #ifndef ERRORS_H
