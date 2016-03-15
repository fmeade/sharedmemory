#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>


#include "ibarland-utils.h"
#include "command-line-options.h"

/*
 * master.c
 * 
 * Project 2 - Producer/Consumer via Shared Memory
 * Due: March 16, 2016 @ 11:59p
 * Author: Forrest Meade (fmeade)
 * 
 * Description: Write a program which forks two child processes which communicate via shared memory. 
 *              One child (the producer) will add elements to a circular queue, and the other child 
 *              (the consumer) will process them.
 */

/* The possible command-line options to a program. 
 */
struct option_info options[] =
  {  { "duration",  'd',  "5000", "The length of the overall simulation, in ms." }
    ,{ "queue-size",  'q',  "10", "The size of the cookie-queue." } 
    ,{ "produce-time",  'p',  "1000", "The maximum time required to produce an item, in ms." }
    ,{ "consume-time", 'c', "1000", "The maximum time required to consume an item, in ms." }
    ,{ "seed", 's', 0, "The random-number seed for the simulation." }
  };

#define NUM_OPTIONS SIZEOF_ARRAY(options)


int main(int argc, char** argv) {
	char** settings = allOptions( argc, argv, NUM_OPTIONS, options );
    // Now, the array `settings` contains all the options, in order:
    // either taken from the command-line, or from the default given in `options[]`.

    unsigned int i;
    for (i = 0;  i < NUM_OPTIONS; i++) {
        printf("Option #%d (%s) is \"%s\".\n", i, options[i].longOption, settings[i] );
    }

    // Got stuck and ran out of time.. just starting to understand the steps.
    // Sorry I dont have more done.

	return 0;
}




