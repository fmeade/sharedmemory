#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
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
 * 
 * Shared memory -- all ints, so I'll declare it as  `int* shm`:
 *    This memory will be init'd by the master process, for the children to use:
 *      shm[0]: queue-size
 *      shm[1]: produce-time
 *      shm[2]: consume-time
 *      shm[3]: seed
 *      shm[4]: queue-start
 */

/* The possible command-line options to the program. 
 */
struct option_info options[] =
  {  { "duration",  'd',  "5000", "The length of the overall simulation, in ms." }
    ,{ "queue-size",  'q',  "10", "The size of the cookie-queue." } 
    ,{ "produce-time",  'p',  "1000", "The maximum time required to produce an item, in ms." }
    ,{ "consume-time", 'c', "1000", "The maximum time required to consume an item, in ms." }
    ,{ "seed", 's', NULL, "The random-number seed for the simulation." }
  };

#define NUM_OPTIONS SIZEOF_ARRAY(options)


int main(int argc, char** argv) {
    char** settings = allOptions( argc, argv, NUM_OPTIONS, options );
    // Now, the array `settings` contains all the options, in order:
    // either taken from the command-line, or from the default given in `options[]`.

    // generates a random number for the seed if not provided
    srandom(time(0));
    if (settings[4] == NULL)
        settings[4] = intToString(random());
        //Now, the seed is a random integer


    // name and size of shared memory
    const char *name = "fmeade-shm"; 
    const int size = (4 + atoi(settings[1])) * sizeof(int);

    int shm_fd;
    void *ptr;

    /* create the shared memory segment */
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("creating shared memory failed\n");
        exit(-1);
    }

    /* configure the size of the shared memory segment */
    ftruncate(shm_fd, size);

    /* now map the shared memory segment in the address space of the process */
    ptr = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        fprintf(stderr, "Map failed\n");
        return -1;
    }

    /* put start/end into shared memory */
    


    /* add circular queue to shared memory */


    /* fork producer process */
    pid_t producer = fork();

    if(producer < 0) {
        fprintf(stderr, "fork failed\n");
        return producer;
    }
    else if(producer == 0) {
        execl("./consumer", name, settings[1], settings[2], settings[4], NULL);

        printf("%s\n", "exec error.");
        exit(-1);
    }
    else {
        printf("%s\n", "master  : started producer");
    }


    /* fork consumer process */
    pid_t consumer = fork();

    if(consumer < 0) {
        fprintf(stderr, "fork failed\n");
        return consumer;
    }
    else if(consumer == 0 && producer != 0) {
        execl("./consumer", name, settings[1], settings[3], settings[4], NULL);

        printf("%s\n", "exec error.");
        exit(-1);
    }
    else {
        printf("%s\n", "master  : started consumer");
    }



    sleep(atoi(settings[0])/1000); // ms to s

    if(producer == 0 && consumer == 0) {
        printf("%s\n", "master  : terminating processes; bye!");
        kill(producer, SIGTERM);
        kill(consumer, SIGTERM);
    }

    return 0;
}




