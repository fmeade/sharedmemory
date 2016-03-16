#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main(int argc, char** argv) {
	int shm_fd;
    void *ptr;

    const char *name = argv[0];
    const int size = atoi(argv[1]);

    /* open the shared memory segment */
    shm_fd = shm_open(name, O_RDONLY, 0666);
    if (shm_fd == -1) {
        perror("opening shared memory failed\n");
        exit(-1);
        }

    /* now map the shared memory segment in the address space of the process */
    ptr = mmap(0, size, PROT_READ, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("Map failed");
        exit(-1);
        }


	return 0;
}