#pragma once
// C++
#include <iostream>
#include <memory>
#include <random>
#include <map>
#include <fstream>

// POSIX
#include <unistd.h>
#include <sys/socket.h>
#include <sys/ioctl.h>

//Crypto++
#include <cryptopp/cryptlib.h>
#include <cryptopp/hex.h>
#include <cryptopp/files.h>
#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1
#include <cryptopp/md5.h>

class Auth
{
private:
    int SALT;
    int work_sock;
    std::map<std::string, std::string> base;
    // User user;
public:
    Auth(): work_sock(-1) {}
    void operator()(int sock);
    void auth();
    //ident();
    std::string string_recv();
    void base_read(std::string file_name);
    //hash();

};
