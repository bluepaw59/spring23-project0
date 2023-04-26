//copied from https://github.com/dboyan/CS118-S23-1A/blob/main/Week%201/server.c
//with reference to https://beej.us/guide/bgnet/html/split/index.html
//modified by Karen Jin
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>

#define PORT 8080
#define BACKLOG 10 //pending connections queueueue size

int main(int argc, char const *argv[]) {
    int server_fd, new_socket, valread;
    //server_fd to listen
    //new_socket for new connection
    struct sockaddr_in address; //address
    struct sockaddr_in client_address;
    int sin_size;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char str[INET_ADDRSTRLEN];
    char *hello = "Hello from server";

    // Creating socket file descriptor
    //PF_INET for IPv4
    if ((server_fd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    // Note: https://stackoverflow.com/questions/58599070/socket-programming-setsockopt-protocol-not-available
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(PORT);

    memset(address.sin_zero, '\0', sizeof(address.sin_zero));

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *) &address,
             sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }


    if (listen(server_fd, BACKLOG) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    while(1)
    {
      sin_size = sizeof(struct sockaddr_in);
      if ((new_socket = accept(server_fd, (struct sockaddr *) &address,
                             (socklen_t * ) & addrlen)) < 0) {
          perror("accept");
          exit(EXIT_FAILURE);
          continue;
      }
      valread = read(new_socket, buffer, 1024);
      printf("%s\n", buffer);
      send(new_socket, hello, strlen(hello), 0);
      printf("Hello message sent\n");
      printf("server: got connection from %s\n", inet_ntop(AF_INET, &(address.sin_addr), str, INET_ADDRSTRLEN));
      close(new_socket);
    }

    
    valread = read(new_socket, buffer, 1024);
    printf("%s\n", buffer);
    send(new_socket, hello, strlen(hello), 0);
    printf("Hello message sent\n");
    return 0;
}