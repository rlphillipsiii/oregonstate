#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "read.h"
#include "write.h"

int cmp_hdr_name(struct ar_hdr *hdr, char *name);
int extract(int a_fd, char *name, struct ar_hdr *hdr);
int delete_files(char *aname, char **files, int count);
int extract_files(char *aname, char **names, int count);
