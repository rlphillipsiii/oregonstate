#include <stdlib.h>
#include <stdio.h>

int is_letter(int letter);
char *get_next_str(FILE *f);
int exec_sort(int index, int pipes[][2], int p_out[][2]);
char *strmalloc(char buf[]);
