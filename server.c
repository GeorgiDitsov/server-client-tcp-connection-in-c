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
#include "grave.h"

#define FILE_PATH "graves.dat"
#define BUFF_SIZE 100
#define PORT 8081
#define SA struct sockaddr

// example function, which writes some structures of type Grave in file graves.dat
void writeAllGravesInFile(void)
{
    int fd, sz;

    fd = open(FILE_PATH, O_WRONLY | O_TRUNC);
    if (fd < 0)
    {
        puts("Error");
        exit(1);
    }

    struct Grave input1 = {1, 1, "Stefan Stefanov, male, 4809162201", false, false, time(NULL)};
    struct Grave input2 = {1, 2, "Stoyan Stoyanov, male, 8001194123", true, false, NULL};
    struct Grave input3 = {3, 6, "Dimitrinka Pelova, female, 6911015671", false, true, NULL};

    sz = write(fd, &input1, sizeof(input1));
    sz = write(fd, &input2, sizeof(input2));
    sz = write(fd, &input3, sizeof(input3));

    close(fd);
}

// function which finds requested grave by its row and column, and sends it back to the client
void findRequestedGrave(int row, int col, int sockfd)
{
    int fileDescriptor = open(FILE_PATH, O_RDONLY);
    if(fileDescriptor == -1)
    {
        puts("Error occured when attempting to open the file");
        exit(1);
    }

    struct Grave grave;

    while(read(fileDescriptor, &grave, sizeof(grave)) != 0)
    {
        printf("Grave[row=%d, col=%d, info=%s, reservedFor15years=%d, reservedForever=%d, passedAway=%d]\n",
            grave.row, grave.col, grave.info, grave.reservedFor15years, grave.reservedForever, grave.passedAway);
        if(row == grave.row && col == grave.col)
        {
            send(sockfd, &grave, sizeof(grave), 0);
            break;
        }
    }

    close(fileDescriptor);
}

// function which receives client request about grave's row and column
void receiveRequest(int sockfd)
{
    int row, col;
    char buff[BUFF_SIZE];

    read(sockfd, buff, sizeof(buff));
    printf("Client sent = %s", buff);
    sscanf(buff, "%d %d", &row, &col);

    findRequestedGrave(row, col, sockfd);
}

// starts the server on port localhost:8081 and listens for clients to connect
int main(void)
{
    int sockfd, connfd, len;
    struct sockaddr_in cli, servaddr =
    {
        .sin_family = AF_INET,
        .sin_addr.s_addr = htonl(INADDR_ANY),
        .sin_port = htons(PORT)
    };

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1)
    {
        puts("Socket creation failed...");
        exit(0);
    }

    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0)
    {
        puts("Socket bind failed...");
        exit(0);
    }

    while(true)
    {
        if ((listen(sockfd, 5)) != 0)
        {
            puts("Listen failed...");
            exit(0);
        }
        else
        {
            puts("Server listening...");
        }

        len = sizeof(cli);
        connfd = accept(sockfd, (SA*)&cli, &len);
        if (connfd < 0)
        {
            puts("Server acccept failed...");
            exit(0);
        }
        else
        {
            puts("Server acccept the client...");
        }

        receiveRequest(connfd);
    }

    close(sockfd);
}
