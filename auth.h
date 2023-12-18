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
    int work_sock;
    std::map<std::string, std::string> base_cont;
    bool id_check(std::string id, std::map<std::string, std::string> base_c);
    bool pw_check(std::string pw_from_cl, std::string hashed_pw);
public:
    std::map<std::string, std::string> base_read(std::string file_name);
    void set_base(std::map<std::string, std::string> base_c);
    Auth(): work_sock(-1) {}
    void operator()(int sock);
    void auth();
    std::string string_recv();
    std::string salt_gen();
    std::string pw_hash(std::string salt, std::string password);
};
