/* A file to demo the use of command-line-options's `allOptions`.
 * Compile this file with
 *    gcc -Wall sample.c command-line-options.o ibarland-utils.o -o sample -lm
 *
 * (If the .o file doesn't exist, create it by compiling with -c:)
 *   gcc -Wall -c command-line-options.c
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "ibarland-utils.h"

#include "command-line-options.h"


/* The possible command-line options to a program. 
 */
struct option_info options[] =
  {  { "file",  'f',  "foo.txt", "the file to blazblarg" }
    ,{ "name",  'n',  "ibarland", "the primary blazlbarger" } 
    ,{ "size",  's',  "98", "how many blazzes to blarg (in dozens)" }
    ,{ "stuff", '\0', NULL, "what to call your stuff" }
    ,{ "otherStuff", 'o', "blarg", "the help info for other stuff" }
    };
/* After declaring the above, you can invoke the program with (say) 
 *   command-line arguments  `--size 44 -f baz.txt`,
 * and then `allOptions` will return:
 *    { "baz.txt", "ibarland", "44", NULL, "blarg" }
 * Note that these values are in the order that you specify in your array-of-option_info.
 */
  
#define NUM_OPTIONS SIZEOF_ARRAY(options)


int main( int argc, char** argv ) {
    char** settings = allOptions( argc, argv, NUM_OPTIONS, options );
    // Now, the array `settings` contains all the options, in order:
    // either taken from the command-line, or from the default given in `options[]`.
    
    unsigned int i;
    for (i=0;  i<NUM_OPTIONS;  ++i) {
        printf("Option #%d (%s) is \"%s\".\n", i, options[i].longOption, settings[i] );
        }

    return 0;
    }
