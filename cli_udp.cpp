#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstring>
#include <sys/time.h>


#define  SERVER_PORT    5556
#define  SERVER_NAME   "127.0.0.1"
#define  BUFLEN         512
#define  NCOPY          5


int main(void) {
    hostent *hostinfo = gethostbyname(SERVER_NAME);
    if (!hostinfo) {
        fprintf (stderr, "Unknown host %s.\n",SERVER_NAME);
        exit (EXIT_FAILURE);
    }

    // Заполняем адресную структуру для последующего
    // использования при посылке сообщения
    struct sockaddr_in server_addr;
    server_addr.sin_family = hostinfo->h_addrtype;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr = *(struct in_addr*) hostinfo->h_addr;

    // Создаем UDP сокет.
    int sock = socket(AF_INET,SOCK_DGRAM,0);
    if (sock < 0) {
        perror ("Client: socket was not created");
        exit (EXIT_FAILURE);
    }

    // Привязываем сокет к произвольному адресу
    struct sockaddr_in client_addr;
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    client_addr.sin_port = htons(0);
    if (bind(sock, (struct sockaddr*)&client_addr, sizeof(client_addr)) < 0) {
        perror ("Client: cannot bind socket");
        exit (EXIT_FAILURE);
    }

    std::string tosend;
    std::cout << "Client: ready to send your message >";
    while (getline(std::cin, tosend)){
        if (sendto(sock, tosend.c_str(), tosend.length(), 0, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
            std::cerr << "Client: cannot send data" << std::endl;
            close(sock);
            exit (EXIT_FAILURE);
        }
        if (tosend == "end"){
            break;
        }
        char buf[BUFLEN]{};
        socklen_t size = sizeof(server_addr);
        if(recvfrom(sock, buf, BUFLEN, 0, (struct sockaddr*)&server_addr, &size) < 0) {
            std::cerr << "Client: read failure" << std::endl;
            continue;
        }
        std::cout << buf << std::endl;
        std::cout << "Client: ready to send your message >";
    }

    return 0;
}
