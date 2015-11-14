#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ar.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

char *strmode(mode_t mode);
int read_hdr(struct ar_hdr *file, int fd);
int seek_to_armag(char *fname);
int seek_to_next(struct ar_hdr *hdr, int fd);
long int e(long int num, int pow, int base);
long int stoi(char num[], int size, int base);
void print_archive(char *fname, int verbose);
