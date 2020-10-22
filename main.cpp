#include <iostream>
#include <cerrno>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <cstdio>
#include <unistd.h>
#include <cstring>
#include <sstream>
#include "Student.h"

const int PORT = 5556;
const int BUFLEN = 100;
const string helpmsg = "Operations with the database are performed by the following commands:\n\
insert [NAME] [GROUP] [RATING]\n\n\
remove [NAME] [GROUP] [RATING]\n\n\
{select | reselect} [ARG]=[DIAPASON]... end\n\n\
print [ARGS]  {sort ARG} end\n";

int main() {

    Student DB;
    int sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        std::cerr << "Server: cannot create socket" << std::endl;
        exit (EXIT_FAILURE);
    }
    sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(PORT);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sock,(struct sockaddr*)&addr,sizeof(addr)) < 0) {
        std::cerr << "Server: cannot bind socket" << std::endl;
        exit (EXIT_FAILURE);
    }
    std::cout << "Server is ready to get data" << std::endl;

    sockaddr_in client;
    memset(&client, 0, sizeof(addr));
    while(true) {
        char buf[BUFLEN];{};
        std::string answer;
        // читаем данные от клиента; ждем на этом месте, пока они появятся
        socklen_t size = sizeof(client);
        if(recvfrom(sock, buf, BUFLEN, 0, (struct sockaddr*)&client, &size) < 0) {
            std::cerr << "Server: read failure" << std::endl;
            continue;
        }
        std::cout << "Server has got message: " << buf << std::endl;
        std::string str(buf);
        if (str == "end"){
            std::cout << "closing" << std::endl;
            break;
        }
        if (str == "help"){
            answer = helpmsg;
        } else {
            try {
                std::ostringstream out;
                DB.Process(str, out);
                answer = out.str();
                std::cout << answer << '\n';
            } catch (const char *s){
                answer = s;
            }
        }
        // ответ
        std::cout << "sending back" << std::endl;
        if(sendto(sock, answer.c_str(), answer.size() + 1, 0, (struct sockaddr*)&client, sizeof(client)) < 0) {
            std::cerr << "Server: cannot send data echo" << std::endl;
        }
    }
    return 0;
}