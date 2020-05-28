#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netdb.h>
#include <time.h>
#include <math.h>
#include "grave.h"

#define BUFF_SIZE 100
#define PORT 8081
#define IP_ADDRESS "127.0.0.1"
#define SECONDS_IN_DAY 86400
#define SA struct sockaddr

// function which creates client socket, assigns IP and PORT, and attempts to connect with server socket
int attemptConnnectionToServer(int sockfd, int connfd)
{
    struct sockaddr_in cli, servaddr = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = inet_addr(IP_ADDRESS),
        .sin_port = htons(PORT)
	};

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		puts("Socket creation failed...");
		exit(0);
	}
	else {
        puts("Socket successfully created...");
    }

	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
		puts("Connection with the server failed...");
		exit(0);
	}
	else {
        puts("Connected to the server...");
    }
    return sockfd;
}

// function, which prints all information about given grave
void printGraveInformation(struct Grave grave)
{
    puts("Grave information:");
    printf("Row = %d\n", grave.row);
    printf("Column = %d\n", grave.col);
    printf("Info = %s\n", grave.info);
    printf("Reserved for 15 years = %s\n", grave.reservedFor15years ? "yes" : "no");
    printf("Reserved forever = %s\n", grave.reservedForever ? "yes" : "no");
    if(grave.passedAway > 0)
    {
        time_t now = time(NULL);
        double diffInSeconds = difftime(now, grave.passedAway);
        printf("Passed away on %s", asctime(localtime(&grave.passedAway)));
        printf("%.0f days after death", round(diffInSeconds/SECONDS_IN_DAY));
    }
}

// function which gets client input about grave's row and column, sends it to the server and then receives Grave structure
void sendRequest(int sockfd)
{
    struct Grave grave;
    char buff[BUFF_SIZE];
    int n = 0;

    puts("Enter your searched grave row and column:");
    while((buff[n++] = getchar()) != '\n');
    printf("Entered = %s", buff);

    write(sockfd, buff, sizeof(buff));
    recv(sockfd, &grave, sizeof(grave), 0);

    close(sockfd);

    printGraveInformation(grave);
}

// starts the client program
int main(void)
{
	int sockfd, connfd;
    sendRequest(attemptConnnectionToServer(sockfd, connfd));
}
