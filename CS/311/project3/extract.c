#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <utime.h>

#include "read.h"
#include "write.h"

#define ERR_EXTRACT -1
#define ERR_OPEN -2
#define ERR_TIME -3
#define ERR_CLOSE -4
#define ERR_DELETE -5
#define ERR_UNLINK -6
#define ERR_STAT -7


int cmp_hdr_name(struct ar_hdr *hdr, char *name)
{
	int i, j;
	for (i = 15; i >= 0; i--) {
		if (hdr->ar_name[i] != ' ' && hdr->ar_name[i] != '/')
			break;
	}

	if(strlen(name) != i+1)
		return 0;

	for (j = 0; j < i+1; j++) {
		if (name[j] != hdr->ar_name[j])
			return 0;
	}

	return 1;
}

int extract(int a_fd, char *name, struct ar_hdr *hdr)
{
	struct utimbuf t;
	int fd = open(name, O_WRONLY | O_CREAT, stoi(hdr->ar_mode, 8, 8));
	
	if (fd == -1)
		return ERR_OPEN;

	if (write_file(fd, a_fd, stoi(hdr->ar_size, 10, 10), 0) != 0)
		return ERR_EXTRACT;

	if (close(fd) == -1)
		return ERR_CLOSE;

	t.actime = stoi(hdr->ar_date, 10, 10);
	t.modtime = stoi(hdr->ar_date, 10, 10);

	if (utime(name, &t) == -1)
		return ERR_TIME;

	return 0;
}

int extract_files(char *aname, char **names, int count)
{
	struct ar_hdr hdr;
	int i, err, fd, bytes, seek;
	char buf[1];

	int a_fd = seek_to_armag(aname);
	if (a_fd < 0)
		return ERR_EXTRACT;

	while ((bytes = read(a_fd, buf, 1)) != -1) {
		if (bytes == 0)
			return 0;
		
		if (lseek(a_fd, -1, SEEK_CUR) == -1)
			return ERR_EXTRACT;

		if (read_hdr(&hdr, a_fd) != 0)
			return ERR_EXTRACT;

		seek = 1;
		for (i = 0; i < count; i++) {	
			if (cmp_hdr_name(&hdr, names[i])) {
				if (extract(a_fd, names[i], &hdr) != 0)
					return ERR_EXTRACT;
				
				names[i] = "";
				seek = 0;
			} 
		}

		if (seek) {
			if (seek_to_next(&hdr, a_fd) != 0)
				return ERR_EXTRACT;
		} else {
			if (stoi(hdr.ar_size, 10, 10)%2 == 1) {
				if (lseek(a_fd, 1, SEEK_CUR) == -1)
					return ERR_EXTRACT;
			}
		}
	}

	return 0;
}

int delete_files(char *aname, char **files, int count)
{
	struct ar_hdr hdr;
	struct stat info;
	int i, err, fd, bytes, seek;
	long int size;
	char buf[1];

	int a_fd = seek_to_armag(aname);
	if (a_fd == -1)
		return ERR_DELETE;
	
	if (fstat(a_fd, &info) == -1)
		return ERR_STAT;
	
	if (unlink(aname) == -1)
		return ERR_UNLINK;

	fd = open(aname, O_WRONLY | O_CREAT, info.st_mode);
	if (fd == -1)
		return ERR_UNLINK;

	if (write_start(fd) != 0)
		return ERR_DELETE;
	
	while ((bytes = read(a_fd, buf, 1)) != -1) {
		if (bytes == 0)
			return 0;

		if (lseek(a_fd, -1, SEEK_CUR) == -1)
			return ERR_DELETE;

		if (read_hdr(&hdr, a_fd) != 0)
			return ERR_DELETE;

		seek = 1;
		for (i = 0; i < count; i++) {
			if (cmp_hdr_name(&hdr, files[i])) {
				if (seek_to_next(&hdr, a_fd) != 0)
					return ERR_DELETE;
				
				files[i] = "";
				seek = 0;
			}
		}

		size = stoi(hdr.ar_size, 10, 10);
		if (seek) {
			if (write_header(&hdr, fd) != 0)
				return ERR_DELETE;

			if (size%2 == 1)
				size++;

			if (write_file(fd, a_fd, size, 1) != 0)
				return ERR_DELETE;
		}
	}

	return 0;
}
