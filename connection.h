#pragma once
#include <string>
#include <iomanip>
#include <map>
#include <typeinfo>
#include <system_error>

//UNIX & POSIX
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <memory.h>

class Connection
{
private:
    int port;
    int queue_len;
    int sock;
    
public:
    void connect(unsigned short port, int qlen);
    void disconnect();
    void wait();
};
