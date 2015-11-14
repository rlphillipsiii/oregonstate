#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#define _POSIX_SOURCE
#define _GNU_SOURCE

#define READ 0
#define WRITE 1

#define STDIN 0
#define STDOUT 1
#define STDERR 2

int is_letter(int letter)
{
	if ((letter > 96 && letter < 122) || letter == 45)
		return letter;

	if (letter+32 > 96 && letter+32 < 122)
		return letter+32;

	return -1;
}

char *strmalloc(char buf[])
{
	char *word = (char *) malloc(sizeof(char)*strlen(buf)+2);
	char c;

	int i, j;
	for (i = j = 0; i < strlen(buf); i++) {
		if ((c = is_letter(buf[i])) != -1) {
			word[j] = c;
			j++;
		}
	}

	word[j] = '\n';
	word[j+1] = 0;
	return word;
}

char *get_next_str(FILE *f)
{
	char buf[2048];
	if (f == NULL)
		return NULL;

	do {
		if (feof(f))
			return NULL;
	
		fscanf(f, "%s", buf);	
	} while (!((buf[0] > 64 && buf[0] < 91) ||
	           (buf[0] > 96 && buf[0] < 123)));

	return strmalloc(buf);
}

int exec_sort(int index, int pipes[][2], int p_out[][2])
{
	int i;
	FILE *fd;
	char word[100];
	switch (fork()) {
	case -1:
		return -1;

	case 0:
		for (i = 0; i < index+1; i++) {
			close(pipes[i][WRITE]);
			close(p_out[i][READ]);
			if (i != index)
				close(p_out[i][WRITE]);
		}

		if (dup2(pipes[index][READ], STDIN) == -1)
			return -1;
		
		if (dup2(p_out[index][WRITE], STDOUT) == -1)
			return -1;
		
		close(pipes[index][READ]);
		close(p_out[index][WRITE]);
		if (execlp("/bin/sort", "/bin/sort", (char *)NULL) == -1)
			return -1;
		
 		close(STDOUT);
		close(STDIN);
		break;

	default:
		close(pipes[index][READ]);
		break;
	}

	return 0;
}
