#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include "parser.h"

#define _POSIX_SOURCE
#define _GNU_SOURCE

#define READ 0
#define WRITE 1

#define STDIN 0
#define STDOUT 1
#define STDERR 2

int index_next(char *words[], int size)
{
	int index = 0;
	int done = 0;

	int i;
	for (i = 0; i < size; i++) {
		if (words[i] != NULL) {
			if (strcmp(words[index], words[i]) > 0)
				index = i;
		
			done = 1;
		}
	}

	if (done == 0)
		return -1;

	return index;
}

void init_words(char *words[], int size)
{
	int i;
	for (i = 0; i < size; i++)
		words[i] = NULL;
}

void merge(FILE *fdpipes[], int num_pipes)
{
	char *words[num_pipes];
	char word[100];
	char *last;
	
	int i, index, count;
	init_words(words, num_pipes);
	for (i = 0; i < num_pipes; i++) {
		if (fgets(word, 100, fdpipes[i]) != 0)
			words[i] = strmalloc(word);
	}

	last = (char *) malloc(sizeof(char));
	last[0] = '\0';

	count = 1;
	while ((index = index_next(words, num_pipes)) != -1) {
		if (strcmp(words[index], last) != 0) {
			if (strcmp(last, "") != 0)
				printf("%d:%s", count, last);
			
			last = words[index];
			count = 1;
		} else {
			count++;
		}

		if (fgets(word, 100, fdpipes[index]) == 0)
			words[index] = NULL;
		else
			words[index] = strmalloc(word);
		
	}
}

int fork_merger(int num, int pipes[][2], int p_close[][2])
{
	int i;
	FILE *fpipes[num];

	switch (fork()) {
	case -1:
		return -1;

	case 0:
		for (i = 0; i < num; i++) {
			close(pipes[i][WRITE]);
			close(p_close[i][WRITE]);
			fpipes[i] = fdopen(pipes[i][READ], "r");
		}

		merge(fpipes, num);
		break;

	default:
		for (i = 0; i < num; i++) {
			close(pipes[i][READ]);
			close(pipes[i][WRITE]);
		}

		break;
	}

	return 0;
}
