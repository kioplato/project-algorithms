#ifndef FPARSER_H
#define FPARSER_H

enum {
	FPARSE_OK,  // Successful execution.

	FPARSE_INV_PARAMS,  // Invalid parameters provided (i.e. NULL).

	FPARSE_FEOF,        // File stream reached EOF before reading a delimiter.
	FPARSE_FERROR,      // File stream error occured (i.e. deleted file).
};

extern int f_errno;

/*
 * Read a token from specified stream 'stream' that ends with a delimiter.
 *
 * The delimiters are specified in an array where the first `sizeof(unsigned)`
 * bytes describe the length of the array. The remaining bytes are
 * `sizeof(unsigned)` each of which specify one delimiter.
 *
 * Return value:
 * On success, the read bytes are returned, along with their number.
 * The same format as the delimiters array.
 *
 * On failure, NULL is returned and f_errno is set accordingly.
 * If invalid non-NULL stream is passed behaviour is undefined.
 * Possible f_errno errors:
 * FPARSE_INV_PARAMS, FPARSE_FEOF, FPARSE_FERROR, FPARSE_OK.
 */
void* ftoken(FILE* stream, void* delims);

/*
 * Tokenizes stream delimitered by 'delims' and passes them, one by one as they
 * are read from stream, to `convert()`. To tokenize stream `ftoken()` is used.
 * Function `convert()` can be NULL. In such case each stream token is pointed
 * by a pointer stored in returned array. Return value of `convert()` is stored
 * in a pointer which is stored in the returned array by `fparse()`.
 *
 * stream: Stream to read from.
 *
 * convert: Takes two arguments. First one is the memory `fparse()` is going
 * to return. This way `convert()` may know what's been converted. The second
 * is the token as returned by `ftoken()`.
 *
 * delims: Array passed into `ftoken()` in the format described in `ftoken()`.
 *
 * Return value:
 * On success, the converted tokens are returned and f_errno indicates EOF.
 *
 * On failure, NULL is returned and f_errno is set accordingly.
 */
void* fparse(FILE* stream, void* (*convert)(const void*, void*), void* delims);

// Some popular delims wrappers.

/*
 * New line delimiter.
 * Call `free()` afterwards.
 */
void* nldelim();

#endif  // #ifndef FPARSER_H
