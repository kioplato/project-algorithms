#ifndef ARGS_H
#define ARGS_H

struct args {
	char* inputf;
	char* queryf;
	char* outputf;
	unsigned k;
	unsigned L;
	unsigned N;
	unsigned R;
};

/*
 * Initialize the `struct args` structure.
 * Set the defaults of command line arguments.
 * Memory is allocated on heap. Call `args_free()` to free memory.
 *
 * Return value:
 * Populated structure `struct args` with the defaults.
 */
struct args* args_init();

/*
 * Iterate through a string array `argv` and store it's tokens to `args` struct.
 * Does not use the same memory used in `argv` for the tokens.
 * Instead it's heap allocated. Use `args_free()` to release it.
 *
 * args: Initialized `struct args`.
 * argc: Number of arguments in `argv` array.
 * argv: Arguments as strings passed to the program by the command line.
 *
 * Return value:
 * The structure `struct args` with it's members filled with the provided args.
 * If the argv or arguments are invalid usage is printed and the program exits.
 */
struct args* args_tokenize(struct args* args, int argc, char* argv[]);

/*
 * Prompt user for the missing arguments.
 * Dataset path, query file path and output file path might be missing.
 *
 * Return value:
 * Structure `struct args` populated with missing arguments from the user.
 */
struct args* args_prompt(struct args* args);

/*
 * Print the `struct args` structure.
 *
 * Return value:
 * Prints `args` members to stdout, on success.
 * Returns immediately and sets `err_code`, on failure.
 */
void args_print(struct args* args);

/*
 * Free the memory allocated by `struct args` structure.
 *
 * Return value:
 * Always succeeds.
 */
void args_free(struct args* args);

#endif // #ifndef ARGS_H
