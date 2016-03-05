#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>


#include "ibarland-utils.h"

/*
 * 
 * 
 * Project 2 - Producer/Consumer via Shared Memory
 * Due: March 16, 2016 @ 11:59p
 * Author: Forrest Meade (fmeade)
 *
 * How to compile/run: 
 * 
 * Description: 
 * 
 */
int main(int argc, char** argv) {


	return 0;
}


/* The possible command-line options to a program. 
 */
struct option_info options[] =
  {  { "duration",  'd',  5000, "The length of the overall simulation, in ms." }
    ,{ "queue-size",  'q',  10, "The size of the cookie-queue." } 
    ,{ "produce-time",  'p',  1000, "The maximum time required to produce an item, in ms." }
    ,{ "consume-time", 'c', 1000, "The maximum time required to consume an item, in ms." }
    ,{ "seed", 's', time(0), "The random-number seed for the simulation." }
  };



