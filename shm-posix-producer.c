/**
 * Simple program demonstrating shared memory in POSIX systems.
 *
 * This is the producer process that writes to the shared memory region.
 * Run this program BEFORE running the consumer process, since it
 *   *creates* the shared region.
 *
 * To run this program on rucs, add the flag `-lrt` when compiling:
 *    gcc -Wall shm-posix-consumer.c -lrt
 * This lets it link with the library "runtime"(?), 
 * to create the final executable.
 *
 * Figure 3.17
 *
 * @author Silberschatz, Galvin, and Gagne
 * Operating System Concepts  - Ninth Edition
 * Copyright John Wiley & Sons - 2013
 * (slight modifications 2016-feb-18 --ibarland)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>  // for ftruncate

int main() {
    const int SIZE = 4096;
    const char *name = "ibarland-shm-example"; // Name yours different from any other student!
    const char *message0= "Stu\n";
    const char *message1= "O\0S\n";
    const char *message2= "Fun!\n";

    int shm_fd;
    void *ptr;

    /* create the shared memory segment */
    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("creating shared memory failed\n");
        exit(-1);
        }

    /* configure the size of the shared memory segment */
    ftruncate(shm_fd,SIZE);

    /* now map the shared memory segment in the address space of the process */
    ptr = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        fprintf(stderr, "Map failed\n");
       	return -1;
        }

    /**
     * Now write to the shared memory region.
     * Note how we increment the value of ptr after each write,
     * since we're using repeated calls to 'sprintf'.
     */
    sprintf(ptr,"%s",message0);
    ptr += strlen(message0);    // Q: *exactly* which char is `ptr` at, now?
    /* Note: sprintf prints its format string, 
     * THEN (unlike printf/fprintf) it also tacks on a terminating-null.
     */
    sprintf(ptr,"%s",message1);
    ptr += strlen(message1);
    sleep(3);
    sprintf(ptr,"%s",message2);
    ptr += strlen(message2);

    return 0;
    }
