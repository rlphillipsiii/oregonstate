#include <unistd.h>
#include <errno.h>
#include <ar.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MODE (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)
#define ERR_OPEN -1
#define ERR_STAT -2
#define ERR_INVALID_NAME -3
#define ERR_INVALID_FD -4
#define ERR_WRITE -5
#define ERR_READ -6
#define ERR_WRITE_HDR -7
#define ERR_APPEND -8

#define BUF_SIZE 4096


char *extract_fname(char *path)
{
	int i, size;
	int offset = 0;
	for (i = 0; i < strlen(path); i++) {
		if (path[i] == '/')
			offset = i+1;
	}

	size = strlen(path)-offset;
	char *fname = (char *) malloc(sizeof(char)*size);
	
	for (i = 0; i < size+1; i++)
		fname[size-i] = path[strlen(path)-i];
		
	return fname;
}

void _dec_cpy(long int src, char dest[], char *fmtspec, int size)
{
	char buf[13];
	memset(buf, 0, sizeof(buf));

	sprintf(buf, fmtspec, src);
	memcpy(dest, buf, size);
}

int populate_ar_hdr(char *fname, struct ar_hdr *arch, struct stat *info)
{
	int fd;
	char *name;
	char buf[13];
	if (strlen(name = extract_fname(fname)) > 16)
		return ERR_INVALID_NAME;

	fd = open(fname, O_RDONLY);
	if (fd == -1) 
		return ERR_OPEN;
	
	if (fstat(fd, info) != 0) 
		return ERR_STAT;
       	
	memcpy(arch->ar_name, name, strlen(name));
	memset(buf, 0, sizeof(buf));

	_dec_cpy(info->st_mtime, arch->ar_date, "%ld", 12);
	_dec_cpy(info->st_gid, arch->ar_gid, "%d", 6);
	_dec_cpy(info->st_uid, arch->ar_uid, "%d", 6);
	_dec_cpy(info->st_mode, arch->ar_mode, "%o", 8);
	_dec_cpy(info->st_size, arch->ar_size, "%ld", 10);

	memcpy(arch->ar_fmag, &ARFMAG, sizeof(ARFMAG));
	
	return fd;
}

void _cpy_helper(char src[], char dest[], int offset, int size)
{
	int i;
	for (i = 0; i < size; i++)
		dest[i+offset] = src[i];
}

void cpy_hdr(char buf[], struct ar_hdr *arch)
{
	int i;
	int offset = 0;
	
	memset(buf, 0, sizeof(buf));
	_cpy_helper(arch->ar_name, buf, 0, 16);
	_cpy_helper(arch->ar_date, buf, 16, 12);
	_cpy_helper(arch->ar_uid, buf, 28, 6);
	_cpy_helper(arch->ar_gid, buf, 34, 6);
	_cpy_helper(arch->ar_mode, buf, 40, 8);
	_cpy_helper(arch->ar_size, buf, 48, 10);

	buf[58] = arch->ar_fmag[0];
	buf[59] = arch->ar_fmag[1];
}

int write_header(struct ar_hdr *hdr, int fd)
{
	int i, w, r, tmp, loc;
	char buf[60];
	memset(buf, 0, sizeof(buf));

	cpy_hdr(buf, hdr);
	for (i = 0; i < 60; i++) {
		if(buf[i] < 1 || buf[i] > 128)
			buf[i] = ' ';
	}

	w = loc = 0;
	r = 60;
	while (w < r) {
		loc += w;

		if ((tmp = write(fd, buf+loc, r-w)) == -1)
			return -1;

		w += tmp;
	}
       
	return 0;
}

int write_file(int a_fd, int fd, long int size, int append)
{	  
	int w, r, loc, tmp;
	char buf[BUF_SIZE];

	int read_size;
	if (size < BUF_SIZE)
		read_size = size;
	else
		read_size = sizeof(buf);

	w = 0;
	while (w < size) {
		if ((r = read(fd, buf, read_size)) == -1)
			return ERR_READ;

		loc = 0;
		while (r != loc) {
			if ((tmp = write(a_fd, buf+loc, r-loc)) == -1)
				return ERR_WRITE;

			loc += tmp;
			w += tmp;
		}

	}
	
	if(size%2 == 1 && append) {
		sprintf(buf, "\n");
		if(write(a_fd, buf, 1) == -1)
			return ERR_WRITE;
	}

	return 0;
}

int write_start(int fd)
{
	int w, r, tmp, loc;
	
	w = loc = 0;
	r = SARMAG;
	while (w != r) {
		loc += w;

		if ((tmp = write(fd, ARMAG+loc, r-w)) == -1)
			return -1;

		w += tmp;
	}
       
	return 0;
}

int append_archive(char *aname, int fd, long int size, struct ar_hdr *arch)
{
	int a_fd;
	int err = 0;
	char buf[BUF_SIZE];
	
	if (fd == -1)
		return ERR_INVALID_FD;

	a_fd = open(aname, O_WRONLY | O_APPEND);
	if (a_fd == -1) {
		err = 1;

		if (errno == ENOENT)
			a_fd = open(aname, O_WRONLY | O_APPEND | O_CREAT, MODE);
		else
			return ERR_OPEN;
	}

	if (a_fd == -1)
		return ERR_OPEN;

	if (err) {
		if (write_start(a_fd) != 0)
			return ERR_WRITE;
	}

	if (write_header(arch, a_fd) != 0)
		return ERR_WRITE_HDR;	

	if (write_file(a_fd, fd, size, 1) != 0)
		return ERR_WRITE_HDR;

	return 0;
}

int append_files(char **files, char *aname, int count)
{
	struct ar_hdr hdr;
	struct stat info;

	int fd, i;
	memset(&hdr, 0, sizeof(hdr));

	for (i = 0; i < count; i++) {
		fd = populate_ar_hdr(files[i], &hdr, &info);

		if (fd == -1)
			return ERR_APPEND;

		if (append_archive(aname, fd, info.st_size, &hdr) != 0)
			return ERR_APPEND;

		memset(&hdr, 0, sizeof(hdr));
	}

	return 0;
}
