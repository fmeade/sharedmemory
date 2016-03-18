#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main(int argc, char** argv) {

	const char *NAME = argv[0];
	const int QUEUE_SIZE = atoi(argv[1]);
	const int PRODUCE_TIME = atoi(argv[2]);
	const int SEED = atoi(argv[3]);

	srandom(SEED);

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

	int* head_ptr = (int*) ptr + 1;
	int* tail_ptr = head_ptr + 1;
	int* queue = tail_ptr + 1;

	int production_amount = 1;

	while(1) {
		sleep(random() % PRODUCE_TIME);

		if(*head_ptr == (*tail_ptr + 1) % QUEUE_SIZE) {
			printf("%s\n", "producer: shelf is full; waiting for cookies to be eaten.");

			while(1) {
				sleep(PRODUCE_TIME / 3000);

				if(!(*head_ptr == (*tail_ptr + 1) % QUEUE_SIZE)) {
					break;
				}
			}
		}

		int randNum = random() % 10000;
		queue[*tail_ptr] = randNum;
		printf("%s %d (%d)\n", "producer: baked cookie #", production_amount, randNum);
		production_amount = production_amount + 1;

		*tail_ptr = (*tail_ptr + 1) % QUEUE_SIZE;
	}


	return 0;
}