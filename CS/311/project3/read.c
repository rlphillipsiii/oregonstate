#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ar.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define BUF_SIZE 4096

#define ERR_READ -1
#define ERR_OPEN -2
#define ERR_SEEK -3
#define ERR_INVALID -1

char *strmode(mode_t mode)
{
	char *str = (char *) malloc(sizeof(char)*10);
	
	str[0] = mode & S_IRUSR ? 'r' : '-';
	str[1] = mode & S_IWUSR ? 'w' : '-';
	str[2] = (mode & S_ISUID ? (mode & S_IXUSR ? 's' : 'S') : (mode & S_IXUSR ? 'x' : '-'));
	str[3] = mode & S_IRGRP ? 'r' : '-';
	str[4] = mode & S_IWGRP ? 'w' : '-';
	str[5] = (mode & S_ISGID ? (mode & S_IXGRP ? 's' : 'S') : (mode & S_IXGRP ? 'x' : '-'));
	str[6] = mode & S_IROTH ? 'r' : '-';
	str[7] = mode & S_IWOTH ? 'w' : '-';
	str[8] = mode & S_IXOTH ? 'x' : '-';
	str[9] = '\0';
	
	return str;
}

int _buf_cpy(char dest[], char src[], int size, int offset)
{
	int i;
	for (i = 0; i < size; i++)
		dest[i] = src[i+offset];

	return offset+i;
}

int read_hdr(struct ar_hdr *file, int fd)
{
	char buf[60];

	int size = 60;
	int r = 0;
	int tmp = 0;
	int loc = 0;
	int i;

	while (size > 0) {
		if ((r = read(fd, buf+loc, size)) == -1)
			return ERR_READ;

		loc += r;
		size -= r;
	}

	if (buf[58] != ARFMAG[0] || buf[59] != ARFMAG[1])
		return ERR_READ;

	for (i = 0; i < 16; i++) {
		if (buf[i] != '/')
			file->ar_name[i] = buf[i];
		else
			file->ar_name[i] = ' ';
	}

	_buf_cpy(file->ar_date, buf, 12, i);
	_buf_cpy(file->ar_uid, buf, 6, i+12);
	_buf_cpy(file->ar_gid, buf, 6, i+18);
	_buf_cpy(file->ar_mode, buf, 8, i+24);
	_buf_cpy(file->ar_size, buf, 10, i+32);

	file->ar_fmag[0] = ARFMAG[0];
	file->ar_fmag[1] = ARFMAG[1];

	return 0;
}

int seek_to_armag(char *fname)
{
	char buf[SARMAG];
	int r = 0;
	int loc = 0;

	int fd, size, i;
	if((fd = open(fname, O_RDONLY)) == -1)
		return ERR_OPEN;

	size = SARMAG;
	while (size > 0) {
		if((r = read(fd, buf+loc, SARMAG)) == -1)
			return ERR_READ;

		loc += r;
		size -= r;
	}

	for (i = 0; i < SARMAG; i++) {
		if (ARMAG[i] != buf[i])
			return ERR_INVALID;
	}

	return fd;
}

long int e(long int num, int pow, int base)
{
	int i;
	for (i = 0; i < pow; i++)
		num *= base;

	return num;
}

long int stoi(char num[], int size, int base)
{
	long int val = 0;
	int count = 0;

	int i, j;
	for (i = size-1; i >= 0; i--) {
		if (num[i] != ' ') {
			val += e(num[i]-48, count, base);
			count++;
		}
	}
	
	return val;
}

void print_buf(char buf[], int size, int spaces)
{
	int i;
	for (i = 0; i < size; i++) {
		if (buf[i] != ' ' || spaces)
			printf("%c", buf[i]);
	}
}

void print_hdr(struct ar_hdr *hdr, int verbose)
{
	long int seconds;
	int i;
	if (verbose) {
		printf(strmode(stoi(hdr->ar_mode, 8, 8)));
		printf(" ");

		print_buf(hdr->ar_uid, 6, 0);
		printf("/");

		print_buf(hdr->ar_gid, 6, 0);
		printf(" ");

		print_buf(hdr->ar_size, 10, 0);
		printf("\t");

		seconds = stoi(hdr->ar_date, 12, 10);

		struct tm *secs = localtime(&seconds);
		char *date = asctime(secs);

		print_buf(date, strlen(date)-1, 1);
		printf(" ");
	}

	print_buf(hdr->ar_name, 16, 1);
	printf("\n");	
}

int seek_to_next(struct ar_hdr *hdr, int fd)
{
	long int seek = stoi(hdr->ar_size, 10, 10);
	if (seek%2 == 1)
		seek++;

	if(lseek(fd, seek, SEEK_CUR) == -1)
		return ERR_SEEK;

	return 0;
}

void print_archive(char *fname, int verbose)
{
	char buf[1];
	int bytes = 0;
	long int seek;
	struct ar_hdr hdr;

	int fd = seek_to_armag(fname);
	if (fd == -1) {
		perror("Unable to find the start of the archive!");
		return;
	}

	while ((bytes = read(fd, buf, 1)) != -1) {
		if(bytes == 0)
			return;

		if(lseek(fd, -1, SEEK_CUR) == -1) {
			perror("Seek failed!");
			return;
		}

 		if(read_hdr(&hdr, fd) != 0) {
			perror("Failed to read header!");
			return;
		}

		if (seek_to_next(&hdr, fd) != 0) {
			perror("Seeek to next file failed!");
			return;
		}

		print_hdr(&hdr, verbose);
	}
}
