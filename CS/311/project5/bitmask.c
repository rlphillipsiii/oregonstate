#define _BSD_SOURCE

#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>

#define IS_BIT_SET(byte, bit) (byte & (1 << bit))
#define SET_BIT(byte, bit) (byte |= (1 << bit))
#define SET_BOUND(lower) lower < 2 ? 3:lower
#define SET_ODD(lower, k) lower%2 == 0 ? k:k-1
#define CREATE 1
#define EXISTS 0

unsigned char *nums;

struct args {
	long upper;
	long lower;
};

void *get_primes(void *args)
{
	struct args *params = (struct args *) args;

	long upper = params->upper;
	long lower = params->lower;

	long i, k;
	k = SET_BOUND(lower);
	k = SET_ODD(lower, k);

	for (; k < sqrt(upper); k += 2) {
		if (!IS_BIT_SET(nums[k/8], k%8)) {
			for (i = k*k; i < upper; i += k)
				SET_BIT(nums[i/8], i%8);    
		}
	}
	
	return (void *) 0;
}

void threaded_primes(long max, int num_threads)
{
	struct args params[num_threads];
	pthread_t thread[num_threads];

	long i;
	
	nums = (unsigned char *) malloc(max/8+1);
	memset(nums, 0, sizeof(nums));
	
	for (i = 0; i < num_threads; i++) {
		params[i].upper = max;
		params[i].lower = max/num_threads*i;
		
		if (pthread_create(&thread[i], NULL, get_primes,
		                   (void *) &params[i]) != 0) {

			perror("Can't create thread");
			exit(EXIT_FAILURE);
		}
	}

	for (i = 0; i < num_threads; i++)
		pthread_join(thread[i], NULL);
}

void print_primes(long max)
{
	long i;
	printf("Primes:  2\n");
	for (i = 3; i < max; i += 2) {
		if (!IS_BIT_SET(nums[i/8], i%8))
			printf("Primes:  %ld\n", i);
	}	
}

long count_primes(long max)
{
	long i, count = 1;
	for (i = 3; i < max; i += 2) {
		if (!IS_BIT_SET(nums[i/8], i%8)) 
			count++;
	}

	return count;
}

void *open_shared(char *path, int create, int *fd, long max)
{
	void *addr;

	if (create)
		*fd = shm_open(path, O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
	else
		*fd = shm_open(path, O_RDWR, S_IRUSR | S_IWUSR);

	if (*fd == -1) {
		perror("Shared Memory Failure!");
		exit(EXIT_FAILURE);
	}

	if (ftruncate(*fd, max/8+1) == -1) {
		perror("Resize Failure!");
		exit(EXIT_FAILURE);
	}

	addr = mmap(NULL, max/8+1, PROT_READ | PROT_WRITE, MAP_SHARED, *fd, 0);
	if (addr == MAP_FAILED) {
		perror("Memory Map Failure!");
		exit(EXIT_FAILURE);
	}

	return addr;
}

void fork_procs(int procs, int print, long max, long start)
{
	int shm_fd, i;
	struct args params;
	struct timeval end;
	long stop;

	void *addr;
	addr = open_shared("shm_obj", CREATE, &shm_fd, max);
	nums = addr;

	for (i = 0; i < procs; i++) {
		params.lower = max/procs*i;
		params.upper = max;

		switch (fork()) {
		
		case -1:
			perror("Fork Failure!");
			exit(EXIT_FAILURE);

		case 0:
			addr = open_shared("shm_obj", EXISTS, &shm_fd, max);

			nums = addr;
			get_primes((void *)&params);
			return;
		}
	}

	for (i = 0; i < procs; i++)
		wait(NULL);

	printf("Children Reaped...Counting Primes\n");
	printf("Count:  %ld\n", count_primes(max));
	
	if (print)
		print_primes(max);

	gettimeofday(&end, NULL);
	stop = end.tv_sec*1000+end.tv_usec/1000;

	printf("Time:  %fms\n", (double)(stop-start));
}

int main(int argc, char **argv)
{
	int jobs;
	long i, max, start, stop;
	struct timeval begin, end;

	if (argc < 4)
		exit(EXIT_FAILURE);

	jobs = atoi(argv[3]);
	max = strtol(argv[2], NULL, 10);

	gettimeofday(&begin, NULL);
	start = begin.tv_sec*1000+begin.tv_usec/1000;
	
	if (argv[1][0] == 't') { 
		threaded_primes(max, jobs);

		printf("Threads Joined...Counting Primes\n");
		printf("Count:  %ld\n", count_primes(max));
		
		if (strlen(argv[1]) > 1 && argv[1][1] == 'p')
			print_primes(max);

		gettimeofday(&end, NULL);
		stop = end.tv_sec*1000+end.tv_usec/1000;

		printf("Time:  %fms\n", (double)(stop-start));
	}else if (argv[1][0] == 'p') {
		
		if (strlen(argv[1]) > 1 && argv[1][1] == 'p')
			fork_procs(jobs, 1, max, start);
		else
			fork_procs(jobs, 0, max, start);
	}
	
	return 0;
}

