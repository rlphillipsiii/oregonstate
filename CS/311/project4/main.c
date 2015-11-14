#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>

#include "parser.h"
#include "merger.h"

#define _GNU_SOURCE
#define _XOPEN_SOURCE
#define _POSIX_SOURCE
#define _POSIX_C_SOURCE 1

int fork_procs(int procs, int pipes[][2], int p_outs[][2])
{
	int i;
	for (i = 0; i < procs; i++) {
		if (pipe(pipes[i]) == -1)
			return -1;
		
		if (pipe(p_outs[i]) == -1)
			return -1;

		if (exec_sort(i, pipes, p_outs) == -1)
			return -1;
	}
	
	if (fork_merger(procs, p_outs, pipes) == -1)
		return -1;

	return 0;
}

void deal_words(FILE *f, int pipes[][2], int procs)
{
	int i;
	FILE *fpipes[procs];
	char *word;

	for (i = 0; i < procs; i++)
		fpipes[i] = fdopen(pipes[i][1], "w");
	
	i = 0;
	while ((word = get_next_str(f)) != NULL) {
		fputs(word, fpipes[i]);
		free(word);

		i++;
		if (i == procs)
			i = 0;
	}	

	for (i = 0; i < procs; i++)
		fclose(fpipes[i]);
}

int main(int argc, char **argv)
{
	FILE *f, *o;
	int procs, i;
	char *word;
	struct timeval begin, end;
	float time;

	if (argc < 2)
		return EXIT_FAILURE;

	gettimeofday(&begin, NULL);
	
	if (argc < 3)
		f = fopen("books/book1.txt", "r");
	else
		f = fopen(argv[2], "r");

	if (f == NULL)
		return EXIT_FAILURE;

	procs = atoi(argv[1]);
	int pipes[procs][2];
	int p_outs[procs][2];
	
	if (fork_procs(procs, pipes, p_outs) == -1)
		return EXIT_FAILURE;

	deal_words(f, pipes, procs);

	for (i = 0; i < procs; i++)
		close(pipes[i][1]);
	
	for (i = 0; i < procs+1; i++)
		wait(NULL);

	fclose(f);
	gettimeofday(&end, NULL);

	printf("Start:  %f\n", begin.tv_usec/1000.0);
	printf("End:  %f\n", end.tv_usec/1000.0);

	time = (end.tv_usec-begin.tv_usec)/1000.0;
	if (time < 0)
		time = (1000000+end.tv_usec-begin.tv_usec)/1000.0;

	printf("Time:  %f\n", time);
	return EXIT_SUCCESS;
}
