/* See .h file for general-info. */

#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <sys/time.h>
#include "ibarland-utils.h"

// string-equal and string-different -- using `strcmp` in boolean expressions goofs me up otherwise.
bool streq( char* s1, char* s2 ) { return strcmp(s1,s2)==0; }
bool strdiff( char* s1, char* s2 ) { return !streq(s1,s2); }
// Note that we use the name 'strdiff' rather than 'strneq', since that's ambiguous with 'streq up to n chars'.
bool strempty( char* s ) { return streq(s,""); }

// A flag for whether successful test-cases should print a very-short indicator.
bool print_on_test_success = true;

int testCount = 0;
int testFailCount = 0;
const int TEST_INDICATOR_GROUP_SIZE = 5;

/* Are two strings the same (or, both null)?
 * If not, print an error message;
 * if so, and print_on_test_success, print a very-short indicator.
 */
void testStr( char* actual, char* expected ) {
    ++testCount;
    if (actual==expected || (actual != NULL && expected != NULL && streq(actual,expected))) {
        if (print_on_test_success) fprintf(stderr, ".");
        if (testCount % TEST_INDICATOR_GROUP_SIZE == 0) fprintf(stderr," ");
        }
    else {
        fprintf(stderr, "\n**test-fail:\nactual: \"%s\"\nexpect: \"%s\".\n", actual, expected);
        ++testFailCount;
        }
    }

/* Are two chars the same?
 * If not, print an error message;
 * if so, and print_on_test_success, print a very-short indicator.
 */
void testChar( char actual, char expected ) {
    ++testCount;
    if (actual==expected) {
        if (print_on_test_success) fprintf(stderr, ".");
        if (testCount % TEST_INDICATOR_GROUP_SIZE == 0) fprintf(stderr," ");
        }
    else {
        fprintf(stderr, "**test-fail:\nactual: '%c'\nexpect: '%c'.\n", actual, expected);
        ++testFailCount;
        }
    }


/* Are two ints the same?
 * If not, print an error message;
 * if so, and print_on_test_success, print a very-short indicator.
 */
void testInt( int actual, int expected ) {
    ++testCount;
    if (actual==expected) {
        if (print_on_test_success) fprintf(stderr, ".");
        if (testCount % TEST_INDICATOR_GROUP_SIZE == 0) fprintf(stderr," ");
        }
    else {
        fprintf(stderr, "**test-fail:\nactual: \"%d\"\nexpect: \"%d\".\n", actual, expected);
        ++testFailCount;
        }
    }

/* Are two bools the same?
 * If not, print an error message;
 * if so, and print_on_test_success, print a very-short indicator.
 */
void testBool( bool actual, bool expected ) {
    ++testCount;
    if ((actual&&expected) || (!actual && !expected)) {
        if (print_on_test_success) fprintf(stderr, ".");
        if (testCount % TEST_INDICATOR_GROUP_SIZE == 0) fprintf(stderr," ");
        }
    else {
        fprintf(stderr, "**test-fail (as bools):\nactual: %d\nexpect: %d.\n", actual, expected);
        ++testFailCount;
        }
    }


/* Return the #microseconds since the standard epoch. */
long time_usec() {
  struct timeval now;
  gettimeofday(&now,NULL);
  return now.tv_sec*1000000L + now.tv_usec;
  }

/* Return a string numeral, for the given int.
 * The string is heap-allocated; IT IS THE CALLER'S RESPONSIBILITY TO FREE THE STRING when done with it.
 */
char* intToString( int n ) {
  // We could just allocate enough space for 11-ish digits, which currently works, but only if sizeof(int) <= 4.
  // We could allocate enough space based on sizeof(int):   ((int)ceil(8*sizeof(int) * log10(2))   digits
  // But hey, we might as well allocate just the right amount:    ceil(log10(n))
  int nSameDigits = (n==0) ? 1 : (abs(n) < 0 ? abs(n+1) : abs(n)); // A version of n that has the same #digits.
  int numDigits = (int)ceil( log10(nSameDigits+0.001) );  // hack: add epsilon so that n=1000 rounds up to 4, not 3; AND handle n=0.
  char* nAsStr = (char*) malloc( (numDigits+1+1) * sizeof(char) ); // +1 for sign, +1 for terminating null.
  sprintf( nAsStr, "%d", n );
  return nAsStr;
  }

/* Return a string numeral, for the given long.
 * The string is heap-allocated; IT IS THE CALLER'S RESPONSIBILITY TO FREE THE STRING when done with it.
 */
char* longToString( long n ) {
  // We could just allocate enough space for 20 digits, which currently works, but only if sizeof(long) <= 8.
  // We could allocate enough space based on sizeof(long):   ((int)ceil(8*sizeof(long) * log10(2))   digits
  // But hey, we might as well allocate just the right amount:    ceil(log10(n))
  long nSameDigits = (n==0) ? 1 : (labs(n) < 0 ? labs(n+1) : labs(n)); // A version of n that has the same #digits.
  int numDigits = (int)ceil( log10(nSameDigits+0.001) );  // hack: add epsilon so that n=1000 rounds up to 4, not 3; AND handle n=0.
  char* nAsStr = (char*) malloc( (numDigits+1+1) * sizeof(char) ); // +1 for sign, +1 for terminating null.
  sprintf( nAsStr, "%ld", n );
  return nAsStr;
  }
