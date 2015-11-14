#include <stdlib.h>
#include <math.h>
#include <stdio.h>

struct prime_result {
	int num_primes;
	long *primes;
};

void get_primes(struct prime_result *results, long upper_bound)
{
	unsigned char nums[upper_bound];

	long i;
	for (i = 0; i < upper_bound; i++)
		nums[i] = 'p';

	long k = 1;
	int brk_loop = 0;
	while (k < sqrt(upper_bound)) {
		while (nums[k] == 'c') {
			k++;
			
			if (k >= sqrt(upper_bound)) {
				brk_loop = 1;

				break;
			}
		}

		if (brk_loop)
			break;

		long m;
		for (i = 2; i < upper_bound; i++) {
			m = i*(k+1);
			if (m-1 > upper_bound-1)
				break;

			nums[m-1] = 'c';
		}

		k++;
	}

	int count = 0;
	for (i = 1; i < upper_bound; i++) {
		if (nums[i] == 'p') 
			count++;
	}

	long *primes = (long *) malloc(sizeof(int)*count);
	results->num_primes = count;
	
	count = 0;
	for (i = 1; i < upper_bound; i++) {
		if (nums[i] == 'p') {
			primes[count] = i+1;
			count++;
		}
	}	

	results->primes = primes;
}

int main(int argc, char **argv)
{
	struct prime_result results;

	if (argc < 2) 
		get_primes(&results, 50000000);
	else
		get_primes(&results, atoi(argv[1]));
	

	printf("NUMBER OF PRIMES:\t%d\n", results.num_primes);

	int i;
	for (i = 0; i < results.num_primes; i++)
		printf("PRIME:\t%ld\n", results.primes[i]);

	return 0;
}

