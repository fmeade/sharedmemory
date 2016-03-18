#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>

/**
 * producer.c
 *
 * Author: Forrest Meade (fmeade)
 * 
 * Description: Generates a random number and adds it to the circular
 *				queue in the shared memory.
 *
 * Help: http://stackoverflow.com/questions/1921539/using-boolean-values-in-c
 */

int main(int argc, char** argv) {

	const char *NAME = argv[0];
	const int QUEUE_SIZE = atoi(argv[1]);
	const int PRODUCE_TIME = atoi(argv[2]);
	const int SEED = atoi(argv[3]);

	const int SIZE = ((2 + QUEUE_SIZE) * sizeof(int)); // head and tail index plus queue

	srandom(SEED); // Creates a random number pattern based on the seed


	int shm_fd;
	void *ptr;

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


	/* Gets and stores the indexes from shared memory */
	int* head_ptr = (int*) ptr;
	int* tail_ptr = head_ptr + 1;
	int* queue = tail_ptr + 1;

	int production_amount = 1;


	typedef int bool;
	enum { false, true };

	bool loop = true;
	bool loop2 = true;

	/* Loops until master kills the process */
	while(loop) {
		usleep(random() % (PRODUCE_TIME * 1000));

		if(*head_ptr == ((*tail_ptr + 1) % QUEUE_SIZE)) {
			printf("%s\n", "producer: shelf is full; waiting for cookies to be eaten.");
			loop2 = true;

			/* Loops until shelf is no longer full */
			while(loop2) {
				usleep((PRODUCE_TIME* 1000) / 3); // millisecond to microsecond, then cut into a third

				if(!(*head_ptr == ((*tail_ptr + 1) % QUEUE_SIZE))) {
					loop2 = false;
				}
			}
		}


		int randNum = random() % 10000;
		queue[*tail_ptr] = randNum;
		printf("%s%d (%d)\n", "producer: baked  cookie #", production_amount, randNum);
		production_amount = production_amount + 1;

		*tail_ptr = (*tail_ptr + 1) % QUEUE_SIZE;
	}

	return 0;
}