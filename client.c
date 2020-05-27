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

#define STRING_SIZE 100
#define PORT 8081
#define SECONDS_IN_DAY 86400
#define SA struct sockaddr

typedef struct Grave
{
  int64_t row, col;
  char info[STRING_SIZE];
  bool reservedFor15years, reservedForever;
  time_t passedAway;
} Grave;

// function which gets client input and sends it to server, then it prints the received data from server in console
void getGrave(int sockfd)
{
    struct Grave grave;
    char buff[STRING_SIZE];
    int n = 0;

    puts("Enter your searched grave row and column:");
    while((buff[n++] = getchar()) != '\n');
    printf("Entered = %s", buff);

    write(sockfd, buff, sizeof(buff));
    recv(sockfd, &grave, sizeof(grave), 0);

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
        printf("%d days after death", floor(diffInSeconds/SECONDS_IN_DAY));
    }
}

// starts the client program and tries to connect to server on port localhost:8081
int main(void)
{
	int sockfd, connfd;
	struct sockaddr_in cli, servaddr = {
        .sin_family = AF_INET,
        .sin_addr.s_addr = inet_addr("127.0.0.1"),
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

    getGrave(sockfd);

    close(sockfd);
}