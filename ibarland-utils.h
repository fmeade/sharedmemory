/** ibarland-utils.h
 * Helper functions for common C tasks.
 * @author Ian Barland, ibarland@radford.edu
 * @version 2016-Feb-21
 *
 * Macros provided:
 *   DPRINTF      (N.B. To enable debugging, `#define DEBUG` in a file BEFORE `#include`ing this .h.)
 *   SIZEOF_ARRAY (N.B. good only for local, stack-allocated arrays, not pointers)
 *   
 * Functions provided:
 *    time_usec
 *    intToString    (N.B. Caller must free the returned-string.)
 *    longToString   (N.B. Caller must free the returned-string.)
 *
 *    streq
 *    strdiff
 *    strempty
 *    
 *    testStr
 *    testChar
 *    testInt
 *    testBool
 */


/* To use these functions:
 * As with any .h file:
 * In your program, include the .h file.  
 * And when compiling(linking) your program, add 'ibarland-utils.o' to the `gcc` line,
 *   e.g.     gcc -Wall myProg.c -o myProg   ibarland-utils.o
 * If that .o file doesn't exist, first create it with `gcc -Wall -c ibarland-utils.c`.
 *
 * Btw, to include the .h file, put the filename in quotes, not angle-brackets (which'd
 * indicate to search system-standard directories).  If the .h file isn't in the same
 * dir as the program you're compiling, see `gcc -I` for adding dirs to gcc's include-path.
 */

#ifndef IBARLAND_UTILS_H
#define IBARLAND_UTILS_H

#include <string.h> // for strcmp
#include <stdbool.h> // for people calling TEST_BOOL

#define SIZEOF_ARRAY(arr) (sizeof(arr)/sizeof(arr[0]))
/* CAUTION: `SIZEOF_ARRAY` works ONLY with arrays declared with a `[]` 
 * (NOT as a pointer, via malloc), AND when we're in the same scope where it was declared.
 * Otherwise we're seeing the array as a pointer, whose sizeof is 1word, regardless of array size).
 */

// string-equal and string-different -- using `strcmp` in boolean expressions goofs me up otherwise.
bool streq( char* s1, char* s2 );
bool strdiff( char* s1, char* s2 );
// Note that we use the name 'strdiff' rather than 'strneq', since that's ambiguous with 'streq up to n chars'.
bool strempty( char* s );



/* Use 'DPRINTF' for debug printing: */
/* Taken from: http://stackoverflow.com/questions/1941307/c-debug-print-macros, by Tom K. */
/* If your C-compiler doesn't like this, try Aidan Cully's simpler version on same page. */

#ifdef DEBUG  // To enable debug, `#define DEBUG` BEFORE you #include this .h
  #define DPRINTF(fmt, args...) \
    fprintf(stderr, "DEBUG: %s:%d:%s(): " fmt, __FILE__, __LINE__, __func__, ##args)
#else
  #define DPRINTF(fmt, args...) /* macro expands to: nothing! */
#endif



// A flag for whether successful test-cases should print a very-short indicator.
bool print_on_test_success;


/* Are two strings the same (or, both null)?
 * If not, print an error message;
 * if so, and print_on_test_success, print a very-short indicator.
 */
void testStr( char* actual, char* expected );

/* Are two chars the same?
 * If not, print an error message;
 * if so, and print_on_test_success, print a very-short indicator.
 */
void testChar( char actual, char expected );

/* Are two ints the same?
 * If not, print an error message;
 * if so, and print_on_test_success, print a very-short indicator.
 */
void testInt( int actual, int expected );

/* Are two bools the same?
 * If not, print an error message;
 * if so, and print_on_test_success, print a very-short indicator.
 */
void testBool( bool actual, bool expected );


/* Return the #microseconds since the standard epoch. */
long time_usec();

/* Return a string numeral, for the given int.
 * The string is heap-allocated; IT IS THE CALLER'S RESPONSIBILITY TO FREE THE STRING when done with it.
 */
char* intToString( int n );

/* Return a string numeral, for the given long.
 * The string is heap-allocated; IT IS THE CALLER'S RESPONSIBILITY TO FREE THE STRING when done with it.
 */
char* longToString( long n );

#endif