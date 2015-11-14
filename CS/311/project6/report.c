#define _BSD_SOURCE

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>

int open_socket_connection(char *ip, int port)
{
	int sockfd;
	struct sockaddr_in serv;

	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	
	memset(&serv, 0, sizeof(serv));
	serv.sin_family = AF_INET;
	serv.sin_port = htons(port);
	
	inet_pton(AF_INET, ip, &serv.sin_addr);
	if (connect(sockfd, (struct sockaddr *) &serv, sizeof(serv)) == -1) {
		perror("Connection failed");
		exit(EXIT_FAILURE);
	}

	return sockfd;
}

void get_nums(int sockfd, int kill)
{
	char num[1024];
	char *cmd;
	int res;

	if (kill)
		cmd = "kill";
	else
		cmd = "report";

	if (write(sockfd, cmd, strlen(cmd)) == -1) {
		perror("Write to socket failed");
		exit(EXIT_FAILURE);
	}

	memset(num, 0, sizeof(num));
	while ((res = read(sockfd, num, 1024)) != 0) {
		if (res == -1) {
			perror("Error reading from socket");
			exit(EXIT_FAILURE);
		}

		fputs(num, stdout);
		memset(num, 0, sizeof(num));
	} 
}

void report(char *ip, int kill)
{
	int sockfd = open_socket_connection(ip, 1148);
	get_nums(sockfd, kill);
}

int main(int argc, char **argv)
{
	if (argc < 3) {
		report(argv[1], 0);
		return 0;
	}

	if (strcmp(argv[1], "-k") == 0)
		report(argv[2], 1);
	else
		printf("Invalid Command:  %s\n", argv[1]);

	return 0;
}
