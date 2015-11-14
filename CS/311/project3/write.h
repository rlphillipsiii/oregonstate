#include <unistd.h>
#include <errno.h>
#include <ar.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

char *extract_fname(char *path);
int populate_ar_hdr(char *fname, struct ar_hdr *arch, struct stat *info);
void cpy_hdr(char buf[], struct ar_hdr *arch);
int write_header(struct ar_hdr *hdr, int fd);
int write_start(int fd);
int write_file(int a_fd, int fd, long int size, int append);
int append_archive(char *aname, int fd, long int size, struct ar_hdr *arch);
int append_files(char **files, char *aname, int count);
