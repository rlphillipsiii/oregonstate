#define _BSD_SOURCE
#define _POSIX_SOURCE

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
#include <pthread.h>
#include <fcntl.h>
#include <signal.h>

int sock;
long count;

struct bounds {
	long upper;
	long lower;
};

struct args {
	int sockfd;
	int pid;
};

int is_perfect(long num)
{
	int total = 1;
	unsigned int i;

	for (i = 2; i < num; i++) {
		if (num%i == 0)
			total += i;

		count++;
	}

	if (total == num)
		return 1;

	return 0;
}

void find_perfect(int sockfd, long upper, long lower)
{
	char num[4096];
	unsigned int i;
	
	memset(num, 0, sizeof(num));
	for (i = lower; i < upper; i++) {
		if (is_perfect(i))
			sprintf(num, "%s%d\r\n", num, i); 
	}

	printf("computation complete\n");
	if (strlen(num) > 0) {
		if (write(sockfd, num, strlen(num)-2) == -1) {
			perror("number writing failed");
			exit(EXIT_FAILURE);
		}
	}

	if (close(sockfd)) {
		perror("failed to close the socket");
		exit(EXIT_FAILURE);
	}
}

long time_ops()
{
	struct timeval s, e;
	long start, cur;
	int i;
	long count = 0;

	if (gettimeofday(&s, NULL)  == -1) {
		perror("Time failed");
		exit(EXIT_FAILURE);
	}

	start = s.tv_sec*1000+s.tv_usec/1000;
	cur = start;

	while (cur-start < 1000) {
		for (i = 0; i < 1000; i++)
			5%5;

		if (gettimeofday(&e, NULL) == -1) {
			perror("Time failed");
			exit(EXIT_FAILURE);
		}

		cur = e.tv_sec*1000+e.tv_usec/1000;
		count += 1000;
	}

	return 15*count;
}

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

void get_range(int sockfd, struct bounds *bnds, long res)
{
	char num[25];
	sprintf(num, "%ld", res);

	if (write(sockfd, num, strlen(num)) == -1) {
		perror("Write to socket failed");
		exit(EXIT_FAILURE);
	}

	memset(num, 0, sizeof(num));
	if (read(sockfd, num, 64) == 0) {
		perror("Server killed connection");
		exit(EXIT_FAILURE);
	}
	bnds->lower = strtol(num, NULL, 10);
	printf("checking range:  %s-", num);

	memset(num, 0, sizeof(num));
	if (read(sockfd, num, 64) == 0) {
		perror("Server killed connection");
		exit(EXIT_FAILURE);
	}
	bnds->upper = strtol(num, NULL, 10); 
	printf("%s\n", num);
}

void *monitor(void *params)
{
	char resp[1024];
	int res, sockfd, pid;
	struct args *argv;
	
	argv = (struct args *) params;
	sockfd = argv->sockfd;
	pid = argv->pid;

	printf("monitoring thread started\n");
	memset(resp, 0, sizeof(1024));
	while ((res = read(sockfd, resp, 1024)) != 0) {
		if (res == -1) {
			perror("couldn't monitor thread");
			exit(EXIT_FAILURE);
		}
		
		if (strcmp(resp, "kill") == 0) {
			kill(pid, SIGINT);
			return (void *) NULL;
		}
	} 

	return (void *) NULL;
}

void callback(int signum)
{
	int res;
	int offset = 0;
	char *msg = "stop signal received...cleaning up\n";
	
	while ((res = write(1, msg+offset, strlen(msg)-offset)) != 0) {
		if (res == -1)
			break;

		offset += res;
	}

	close(sock);
	exit(signum);
}

void compute(char *ip)
{
	struct bounds bnds;
	struct args argv;
	int sockfd, i;
	long ops;
	pthread_t thread;

	ops = time_ops()/300;	
	sock = sockfd = open_socket_connection(ip, 1148);
	get_range(sockfd, &bnds, ops);

	signal(SIGINT, callback);
	signal(SIGQUIT, callback);
	signal(SIGHUP, callback);

	argv.sockfd = sockfd;
	argv.pid = getpid();
	if (pthread_create(&thread, NULL, monitor, (void *) &argv) != 0) {
		perror("can't create thread");
		exit(EXIT_FAILURE);
	}

	find_perfect(sockfd, bnds.upper, bnds.lower);
}

int main(int argc, char **argv)
{
	compute(argv[1]);

	return 0;
}
