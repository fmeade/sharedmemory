/* #include this file, if you want to use the helper-function `allOptions`.
 * If callers provide named-arguments on the command-line in any order,
 * then your program can call `allOptions` to get back an array of ALL the option-values
 * (with predefined defaults used for those the caller didn't specify.)
 *
 *
 *  Example: If you want the caller to be able to optionally provide any of the
 *  command-line options `--file`, `--name` and `--size`, then add the following
 *  to your program:
 *
 *    #include "command-line-options.h"
 *    struct option_info options[] = {
 *        { "file", 'f', NULL, "the file containing the glubglub" },
 *        { "name", 'n', "ibarland", "the name of the package-author" },
 *        { "size", 's', "45", "the size of the frobzat, in meters." },
 *        };
 *     #define NUM_OPTIONS (SIZEOF_ARRAY(options))
 *
 *  The four pieces of info you must provide for each option are:
 *    long-version (`--name`), short-version (`-n`), a default value if not provided ("ibarland"),
 *    and a string which might someday be used in a help-message (but is not currently used).
 *  
 * 
 * So if the caller invoked "sample --size 27 -f stuff.txt`,
 * then `allOPtions` would return the array { "stuff.txt", "ibarland", "27" }.
 * The items in the return-array are the same order as you list them in `options[]`.
 * 
 *
 * See also: `pargs`, a more robust library for doing this 
 * (but requires more work to extract the info).
 *
 * Known bugs:
 * We assume every option is followed by a value!  (No 'binary' flag-options.)
 * If a value looks like an option, this code will get confused (e.g. if you
 * try to specify a --file whose name is "--size", or if the name of the
 * executable argv[0] is "-n", etc.)!
 */


/* Instructions for compiling your programs that use `allOptions`.
 * Suppose you have a file `sample.c`.
 *
 *  - In your file:
 *      #include "command-line-options.h"
 *   (note the use of quotes, not angle-brackets.)
 *
 *  - When compiling your file:
 *      gcc -Wall command-line-options.o  sample.c  -o sample
 * 
 *  - If the '.o' file doesn't already exist, create it with `gcc -c`:
 *    gcc -Wall -c command-line-options.c
 *
 * You shouldn't need to look inside the file `command-line-options.c`,
 * but if you do, it is all code that that should be understandable to you,
 * if familiar with `struct`.
 */


#ifndef COMMAND_LINE_OPTIONS_H
#define COMMAND_LINE_OPTIONS_H

// the four-field struct used to define an option:
struct option_info {
    char* longOption;
    char shortOption;
    char* defaultValue;
    char* helpString;
    };

// Return an array of option-values, in the same order as in `options`,
// taking the values from `argv` (or else the default in `options[i]`).
//
char** allOptions( int argc, char* argv[], int numOptions, struct option_info options[] );

#endif
