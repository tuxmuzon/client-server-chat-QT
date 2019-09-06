#ifndef SERVER_H
#define SERVER_H
#include <iostream>
#include <map>

using namespace std;

class Server
{
public:
    Server();
    bool serverStart();
private:
    std::map<std::string, std::string> clientList;
};

#endif // SERVER_H
