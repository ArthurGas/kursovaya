#pragma once
#include <string>
#include <iostream>
#include <fstream> 
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

class Interface
{
private:
    int work_sock;
    std::string data;
    std::string b_data;
public:
    Interface(): work_sock(-1) 
    {}
    //Interface() = delete;
    //friend std::istream& operator >> (std::istream& cin, Interface& obj);
    void operator()(int sock);
    std::string data_set(std::string str);
    //std::string string_recv();
    //std::string data_byte(std::string str);
    // std::string data_unbyte(std::string str);
    // void write_help();

};
