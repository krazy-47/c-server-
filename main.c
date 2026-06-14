//c srever made by crazy 8
//created at 23:25 13/06/2026 

#include <stdio.h>      // printf(), perror()
#include <stdlib.h>     // exit(), EXIT_FAILURE
#include <string.h>     // memset(), strerror()
#include <unistd.h>     // close(), read(), write()
#include <sys/socket.h> // socket(), bind(), listen(), accept()
#include <netinet/in.h> // struct sockaddr_in, htons(), INADDR_ANY
#include <arpa/inet.h>  // inet_ntop(), inet_pton() (for later)
//defin port and buffer
#define PORT 8080
#define BUFFER_SIZE 1024
// define variables 
int opt = 1;
struct sockaddr_in address;
socklen_t addrlen = sizeof(address);

int main() {
//erase trash data 
memset(&address, 0, sizeof(address));

//defining 
address.sin_port = htons(PORT);

address.sin_family = AF_INET;

address.sin_addr.s_addr = INADDR_ANY;

//declare 
int server_fd, new_socket;

//get socket from os
server_fd = socket(AF_INET, SOCK_STREAM, 0);

//check if socket is valid 
if (server_fd < 0) {
perror("error");
exit(EXIT_FAILURE);
}

//reserve socket easy restart 
int ret = setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
if (ret < 0){
    perror("error");
}
//bind + check if debug ia working 
if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0){
perror("error bind");
exit(EXIT_FAILURE);
}
//debug
printf("Socket created FD: %d\n", server_fd);

//listen for peer coenction and chek errors
if (listen(server_fd, 5) < 0){
    perror("error lisening");
    exit(EXIT_FAILURE);
}
else {
    printf("Server listening on port 8080\n");
}



while(1){
    char *response = "HTTP/1.0 200 OK\r\nContent-Type: text/html\r\n\r\n<html><body><h1>Hello from my server!</h1></body></html>";

    
    new_socket = accept(server_fd, (struct sockaddr*)&address, &addrlen);

    if (new_socket < 0){
        perror("error acepting peer conection");
        continue;
    }
    printf("peer conected\n");

    char  request[BUFFER_SIZE] = {0};
    if (recv(new_socket, request, BUFFER_SIZE - 1, 0) < 0) {
        perror("error reciving info ");
    }
    if (send(new_socket, response, strlen(response), 0) < 0) {
    perror("error sending response");
    }


    close(new_socket);
}
}