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
#include "calc.h"
#include "auth.h"
#include "server_error.h"
//const int def_qlen = 5;
//const int def_port = 33333;

class Connection
{
private:

    int sock;
    std::unique_ptr <sockaddr_in> serv_addr;
    std::unique_ptr <sockaddr_in> client_addr;
    int queue_len=5;
public:
    void connect(Calculation& clc, Auth& ath, Error& er);
    void disconnect();
    void wait();
    Connection(unsigned short port);
    ~Connection()
    {
        close(sock);
    }
//	Connection()=delete;
};
