#include "headers/llist.h"
#include "headers/logs.h"
#include "headers/server.h"
#include <netinet/in.h>
#include <pthread.h>
#include <regex.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

char *func_example_1(int *client_fd, char *buffer) {
  log_info(buffer);
  char *response = "HTTP/1.1 200 OK";

  log_info("Here is the client_fd:");
  send(*client_fd, response, 100, 0);
  return response;
}

void func_example_2() { log_info("This is a void function 2"); }

int main() {
  log_info("Configuring server...");
  Server_Configs *server_configs = server_configs_create(8080);

  log_info("Adding functions to server...");
  server_add_route(server_configs, "/djqdnjnjqdn", func_example_1, METHOD_GET);

  log_info("Starting server...");
  server_start(server_configs);
}

//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <unistd.h>
//#include <arpa/inet.h>
//
//#define PORT 8080
//#define RESPONSE "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 12\r\n\r\nHello, World"
//
//int main() {
//    int server_fd, new_socket;
//    struct sockaddr_in address;
//    int opt = 1;
//    int addrlen = sizeof(address);
//
//    // Creating socket file descriptor
//    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
//        perror("socket failed");
//        exit(EXIT_FAILURE);
//    }
//
//    // Forcefully attaching socket to the port 8080
//    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
//        perror("setsockopt");
//        exit(EXIT_FAILURE);
//    }
//    address.sin_family = AF_INET;
//    address.sin_addr.s_addr = INADDR_ANY;
//    address.sin_port = htons(PORT);
//
//    // Forcefully attaching socket to the port 8080
//    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
//        perror("bind failed");
//        exit(EXIT_FAILURE);
//    }
//
//    if (listen(server_fd, 3) < 0) {
//        perror("listen");
//        exit(EXIT_FAILURE);
//    }
//
//    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
//        perror("accept");
//        exit(EXIT_FAILURE);
//    }
//
//    // Send response
//    send(new_socket, RESPONSE, strlen(RESPONSE), 0);
//    printf("Response sent\n");
//
//    return 0;
//}
