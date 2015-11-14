#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <errno.h>

#include "read.h"
#include "write.h"
#include "extract.h"

#define _BSD_SOURCE
#define INVALID_CMD -10

/**
struct ar_hdr
{
  char ar_name[16];          Member file name, sometimes / terminated
  char ar_date[12];          File date, decimal seconds since Epoch
  char ar_uid[6], ar_gid[6]; User and group IDs, in ASCII decimal
  char ar_mode[8];           File mode, in ASCII octal
  char ar_size[10];          File size, in ASCII decimal
  char ar_fmag[2];           Always contains ARFMAG
}
*/

int run_cmd(char cmd, int argc, char **argv) 
{
	int i, fd;
	char **args;
	char * n;
	DIR *dir;
	char *files[1];
	struct dirent *file;
	struct ar_hdr hdr;
	struct stat info;

	if (cmd == 't') {
		print_archive(argv[2], 0);
		return 0;
	}

	if (cmd == 'v') {
		print_archive(argv[2], 1);
		return 0;
	}

	if (cmd == 'A') {
		if (open(argv[2], O_RDONLY) == -1)
			return -1;
		
		memset(&hdr, 0, sizeof(hdr));
		dir = opendir("./");

		n = extract_fname(argv[0]);
		while (file = readdir(dir)) {
			if (stat(file->d_name, &info) == -1)
				return -1;

			if (S_ISREG(info.st_mode) && strcmp(file->d_name, n) && strcmp(file->d_name, argv[2])) {
				files[0] = file->d_name;
				if (append_files(files, argv[2], 1) != 0)
					return -1;
			}
		}

		return 0;
	}

	if (argc < 4)
		return -1;

	args = (char **) malloc(sizeof(char *)*(argc-3)); 
	for (i = 3; i < argc; i++)
		args[i-3] = argv[i];

	if (cmd == 'x')
		return extract_files(argv[2], args, argc-3);
	
	if (cmd == 'q')
		return append_files(args, argv[2], argc-3);

	if (cmd == 'd')
		return delete_files(argv[2], args, argc-3);

	
	return INVALID_CMD;
}

int main(int argc, char **argv)
{
	char cmd;
	int code;

	if (argc < 3) {
		fprintf(stderr, "Invalid number of arguments!\n");
		exit(-1);
	}

	if (strlen(argv[1]) != 1) {
		fprintf(stderr, "Invalid command!\n");
		exit(-1);
	}

	cmd = argv[1][0];
	code = run_cmd(cmd, argc, argv);
	if (code != 0)
		perror("Error in processing your request!  ERRNO message");

	return code;
}
