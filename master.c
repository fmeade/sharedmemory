#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <fcntl.h>
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
 * Help: example programs from the assignment, googled library manuals
 * 
 * Shared memory -- all ints, so I'll declare it as  `int* shm`:
 *    This memory will be init'd by the master process, for the children to use:
 *      shm[0]: head of queue
 *      shm[1]: tail of queue
 *      shm[2]: start of queue
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

    const int DURATION = atoi(settings[0]);
    const int QUEUE_SIZE = atoi(settings[1]);
    const int PRODUCE_TIME = atoi(settings[2]);
    const int CONSUME_TIME = atoi(settings[3]);
    int SEED;

    // generates a random number for the seed if not provided
    srandom(time(0));
    if (settings[4] == NULL)
        SEED = random();
        //Now, the seed is a random integer
    else
        SEED = atoi(settings[4]);



    // name and size of shared memory
    const char *NAME = "shm_fmeade"; 
    const int SIZE = ((2 + QUEUE_SIZE) * sizeof(int));

    int shm_fd;
    void *ptr;

    /* create the shared memory segment */
    shm_fd = shm_open(NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("Failed to create shared memory.\n");
        exit(-1);
    }

    /* configure the size of the shared memory segment */
    ftruncate(shm_fd, SIZE);

    /* Now, map the shared memory segment in the address space of the process */
    ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        fprintf(stderr, "Map failed\n");
        return -1;
    }

    /* Gets pointers to head & tail */
    int* ptr_to_head = (int*) ptr;
    int* ptr_to_tail = ptr_to_head + 1;

    /* Puts head & tail into shared memory */
    *ptr_to_head = 0;
    *ptr_to_tail = 0;   

    /* Turn ints back into chars to allow correct size to store (malloc is hard to understand)*/
    char* queue_size_str = malloc(sizeof(int) + 1);
    char* produce_time_str = malloc(sizeof(int) + 1);
    char* consume_time_str = malloc(sizeof(int) + 1);
    char* seed_str = malloc(sizeof(int) + 1);

    /* Put values into allocated memory */
    sprintf(queue_size_str, "%d", QUEUE_SIZE);
    sprintf(produce_time_str, "%d", PRODUCE_TIME);
    sprintf(consume_time_str, "%d", CONSUME_TIME);
    sprintf(seed_str, "%d", SEED);
    

    /* fork producer process */
    pid_t producer = fork();

    if(producer < 0) {
        fprintf(stderr, "fork failed\n");
        return producer;
    }
    else if(producer == 0) {

        execl("./producer", NAME, queue_size_str, produce_time_str, seed_str, NULL);

        fprintf(stderr, "exec error.");
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
    else if(consumer == 0) {
        execl("./consumer", NAME, queue_size_str, consume_time_str, seed_str, NULL);

        fprintf(stderr, "exec error.");
        exit(-1);
    }
    else {
        printf("%s\n", "master  : started consumer");
    }


    usleep(DURATION * 1000); // millisecond to microsecond, then sleep


    printf("%s\n", "master  : terminating processes; bye!");

    kill(producer, SIGTERM);
    kill(consumer, SIGTERM);

    /* remove the shared memory segment */
    if (shm_unlink(NAME) == -1) {
        printf("Error removing %s\n", NAME);
        exit(-1);
        }

    return 0;
}




