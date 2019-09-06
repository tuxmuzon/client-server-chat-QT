#include "server.h"
#include <iostream>     ///< cout
#include <cstring>      ///< memset
#include <errno.h>      ///< errno
#include <sys/socket.h> ///< socket
#include <netinet/in.h> ///< sockaddr_in
#include <arpa/inet.h>  ///< getsockname
#include <unistd.h>     ///< close
#include <thread>
#include <vector>

class ClientHandler{
public:
    void operator()(int s){
        char recvbuf[256];
        while (true) {
            int k = recv(s, recvbuf, sizeof(recvbuf), 0);
            if(k>0){
                std::cout <<recvbuf <<  std::endl;
            }
        }

    }

};


Server::Server()
{
    serverStart();
}

bool Server::serverStart()
{
    //    WSAData wsa;
    //    WORD version = MAKEWORD(2,1);

    //    WSAStartup(version, &wsa);

    //    SOCKET Listener = socket(AF_INET,SOCK_STREAM,NULL);
    //    SOCKET Connect = socket(AF_INET,SOCK_STREAM,NULL);

    //    SOCKADDR_IN Server;

    //    Server.sin_addr.s_addr = INADDR_ANY;
    //    Server.sin_family = AF_INET;
    //    Server.sin_port = htons(5125);

    //    int sizeOf = sizeof(Server);
    //    bind(Listener, (SOCKADDR*)&Server , sizeOf);
    //    listen(Listener,100);
    //    cout << "Listening" << endl;
    //    for(;;){
    //        Connect = accept(Listener,(SOCKADDR*)&Server , &sizeOf);
    //        cout << "CONNECTION MADE!" << endl;
    //        thread t1((ClientHandler()) ,Connect);
    //    }

    const char* google_dns_server = "8.8.8.8";
    int dns_port = 1025;

    struct sockaddr_in serv;
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    int Connect = socket(AF_INET, SOCK_DGRAM, 0);

    //Socket could not be created
    if(sock < 0)
    {
        std::cout << "Socket error" << std::endl;
    }

    memset(&serv, 0, sizeof(serv));
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = inet_addr(google_dns_server);
    serv.sin_port = htons(dns_port);

    int err = connect(sock, (const struct sockaddr*)&serv, sizeof(serv));
    if (err < 0)
    {
        std::cout << "Error number: " << errno
                  << ". Error message: " << strerror(errno) << std::endl;
    }

    struct sockaddr_in name;
    socklen_t namelen = sizeof(name);
    err = getsockname(sock, (struct sockaddr*)&name, &namelen);

    char buffer[80];
    const char* p = inet_ntop(AF_INET, &name.sin_addr, buffer, 80);
    if(p != nullptr)
    {
        std::cout << "Local IP address is: " << buffer << std::endl;
    }
    else
    {
        std::cout << "Error number: " << errno
                  << ". Error message: " << strerror(errno) << std::endl;
    }
    std::vector<std::unique_ptr<std::thread>> threads;
    for(;;){
        struct sockaddr_in addr_c;
             socklen_t sizeOf = sizeof(addr_c);
              Connect = accept(sock, (struct sockaddr*)&addr_c, &sizeOf);
              cout << "CONNECTION MADE!" << endl;
              threads.emplace_back(new std::thread((ClientHandler()) ,Connect));
            }

    close(sock);


    return false;
}
