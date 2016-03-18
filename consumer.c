#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "ibarland-utils.h"

int main(int argc, char** argv) {

	const char *NAME = argv[0];
	const int QUEUE_SIZE = atoi(argv[1]);
	const int CONSUME_TIME = atoi(argv[2]);
	const int SEED = atoi(argv[3]);

	srandom(SEED + 1);

	int shm_fd;
	void *ptr;


	const int SIZE = ((2 + QUEUE_SIZE) * sizeof(int));


	shm_fd = shm_open(NAME, O_RDWR, 0666);

	if (shm_fd == -1){
		perror("Failed to open shared memory.");
		exit(-1);
	}

	ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (ptr == MAP_FAILED){
		perror("Failed to map shared memory.");
		exit(-1);
	}

	int* head_ptr = (int*) ptr;
	int* tail_ptr = head_ptr + 1;
	int* queue = tail_ptr + 1;

	int amount_ate = 1;

	bool loop = true;
	bool loop2 = true;

	while(loop) {

		if(*head_ptr == *tail_ptr) {
			printf("%s\n", "consumer: shelf is empty; waiting for cookies to be baked.");
			loop2 = true;

			while(loop2) {
				usleep((CONSUME_TIME * 1000) / 3); // milli to micro sec then cut into a third

				if(*head_ptr != *tail_ptr) {
					loop2 = false;
				}
			}
		}

		int consumption_amount =  queue[*head_ptr];
		printf("%s%d (%d)\n", "consumer: eating cookie #", amount_ate, consumption_amount);
		amount_ate = amount_ate + 1;

		*head_ptr = (*head_ptr + 1) % QUEUE_SIZE;

		usleep(random() % (CONSUME_TIME * 1000));
	}

	return 0;
}