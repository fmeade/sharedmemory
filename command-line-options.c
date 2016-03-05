/* This file provides the implementation of the function `allOptions`;
 * see `command-line-options.h` for instructions on what that function does,
 * along with compile-notes.
 */

#define _GNU_SOURCE   // needed on rucs, for `strnlen`s declaration.
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "ibarland-utils.h"
#include "command-line-options.h"


  

/* Given a string of form "--otherStuff", return the otherStuff.
 * If it wasn't of that form, return NULL.
 * (The result is a pointer into the provided argument.)
 */
char* extractLongOptionName( char* arg ) {
    if (arg==NULL || (strnlen(arg,3) <= 2) || strncmp(arg,"--",2)!=0) 
        return NULL;
    else
        return (arg+2);
    }
 
                          
/* Given a string of form "-someChar", return the char.
 * If it wasn't of that form, return '\0'.
 */
char extractShortOptionName( char* arg ) {
    if (arg==NULL || (strnlen(arg,3) != 2) || strncmp(arg,"-",1)!=0 || streq(arg,"--")) {
        return '\0';
        }
    else {
        return *(arg+1);
        }
    }
                           
                           
/* Search haystack[] for the indicated option, and return the *next* item of haystack as its value.
 * If not located, return the option's default value.
 * If multiple occurrences of the option, we take the last one ('overwriting' previous ones),
 * except that a "--" will halt the option-searching.
 */
char* findOption( struct option_info target, int n, char* haystack[] ) {
    char* answerSoFar = target.defaultValue;
	int i;
    for (i=0;  i<n-1;  i += 1) {
        if (streq(haystack[i],"--")) break;  /* "--" stops option-processing */
        char* asLongOption  = extractLongOptionName( haystack[i]);
        char  asShortOption = extractShortOptionName(haystack[i]);
        if (    (asLongOption  != NULL  && streq(asLongOption, target.longOption))
             || (asShortOption != '\0' && asShortOption == target.shortOption)) {
            answerSoFar = haystack[i+1];
            ++i;  // skip over haystack[i+1] as a potential next-arg.
            }
        }
    return answerSoFar;
    }


/* If `arg` looks like it's an option, is it one of the allowed ones in `options`?
 */
bool apparentOptionIsLegal( int numOptions, struct option_info options[], char* arg ) {
    char* asLongOption  = extractLongOptionName(  arg );
    char  asShortOption = extractShortOptionName( arg );
    if (asLongOption==NULL && asShortOption=='\0') return true; 
    /* Doesn't look like it's trying to be an option, so no problem. */
    
	int i;
    for (i=0;  i<numOptions;  ++i) {
        if (asLongOption!=NULL) {
            if (streq(asLongOption,options[i].longOption)) return true;
            }
        else if (asShortOption!='\0') {
            if (asShortOption==options[i].shortOption) return true;
            }
        else {
          DPRINTF( "not supposed to reach here!" );
          }
        }
    return false;
    }




/* Given command-line arguments,
 * return an array with the values for ALL possible options.
 * The strings are taken from the command-line if provided, else from `options[i].default`
 * So if argv = { "--size", "27", "-f", "foo.txt" }
 * and options was an array with the options
 *    { {"file","f","-"}, {"name",'n',"ibarland"}, {"size",'s',NULL} }
 * then we'd return {"foo.txt", "ibarland", "27"}.
 */
char** allOptions( int argc, char* argv[], int numOptions, struct option_info options[] ) {
    char** allOpts = (char**) malloc( numOptions * sizeof(char*) );
	int i;
    for (i=0;  i<numOptions;  ++i) {
        allOpts[i] = findOption( options[i], argc, argv );
        }

    // Also: make sure that everything that LOOKS like an arg is valid:
    for (i=0;  i<argc;  ++i) {
        if (streq(argv[i],"--")) break;
        if (!apparentOptionIsLegal(numOptions, options, argv[i])) {
            fprintf(stderr,"Warning: argument #%d, \"%s\", is not a known option.\n", i, argv[i]);
            }
        }
  
    return allOpts;
    }
  
