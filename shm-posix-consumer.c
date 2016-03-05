/**
 * Simple program demonstrating shared memory in POSIX systems.
 *
 * This is the consumer process.
 * Run this *after* the producer process 
 * (since that one creates the shared memory).
 *
 * To run this program on rucs, add the flag `-lrt` when compiling:
 *    gcc -Wall shm-posix-consumer.c -lrt
 * This lets it link with the library "runtime"(?), 
 * to create the final executable.
 *
 * Figure 3.18
 *
 * @author Gagne, Galvin, Silberschatz
 * Operating System Concepts - Ninth Edition
 * Copyright John Wiley & Sons - 2013
 * (slight modifications 2016-feb-18 --ibarland)
 */

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main() {
    const char *name = "ibarland-shm-example";
    const int SIZE = 4096;

    int shm_fd;
    void *ptr;

    /* open the shared memory segment */
    shm_fd = shm_open(name, O_RDONLY, 0666);
    if (shm_fd == -1) {
        perror("opening shared memory failed\n");
        exit(-1);
        }

    /* now map the shared memory segment in the address space of the process */
    ptr = mmap(0,1, PROT_READ, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("Map failed");
        exit(-1);
        }

    /* now read from the shared memory region */
    printf("%s",(char*)ptr);

    /* remove the shared memory segment */
    if (shm_unlink(name) == -1) {
        printf("Error removing %s\n",name);
        exit(-1);
        }

    return 0;
    }
