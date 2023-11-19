#include "auth.h"
#include "server_error.h"
#include <string_view>
#include <iostream>
#include <string>
#include "interface.h"
#include <stdexcept>

void Auth::operator()(int sock)
{
    work_sock = sock;
    Auth::auth();
}
//модуль заполнения базы пользователей с помощью словаря
void Auth::base_read(std::string file_name)
{
    std::string sep=":";
    size_t pos;
    std::string buf, login, pass;

    std::ifstream file(file_name.c_str());
    if (file.is_open()) {
        //if(!getline(file, buf))
        //    throw std::system_error(errno, std::generic_category(), "Base file is empty");
        while(getline(file, buf)) {
            while (buf.find(" ") < buf.size()) {
                buf.erase(buf.find(" "), 1);
            }
            pos = buf.find(sep, 0);
            login = buf.substr(0, pos);
            pass = buf.substr(pos+1, buf.size());
            base[login] = pass;
        }
        file.close();
    }
    else {
        throw std::system_error(errno, std::generic_category(), "Base read error");
    }

}

std::string Auth::string_recv()
{
    int rc;
    int buflen = 16;
    std::unique_ptr <char[]> buf(new char[buflen]);
    while (true) {
        rc = recv(work_sock, buf.get(), buflen, MSG_PEEK);
        if (rc == -1)
            throw std::system_error(errno, std::generic_category(), "Recv string error");
        if (rc < buflen)
            break;
        buflen *= 2;
        buf = std::unique_ptr<char[]>(new char[buflen]);
    }
    std::string res(buf.get(), rc);
    rc = recv(work_sock, nullptr, rc, MSG_TRUNC);
    if (rc == -1)
        throw std::system_error(errno, std::generic_category(), "Clear bufer error");
    res.resize(res.find_last_not_of("\n\r") + 1);
    return res;
}

void Auth::auth()
{
    // static constexpr std::string_view login = "user";
    // static constexpr std::string_view password = "P@ssW0rd";
    namespace cpp = CryptoPP;
    std::mt19937_64 gen(time(nullptr));
    int rc;
    std::string res = string_recv();
    if (base.find(res) == base.end())
        throw auth_error("Identification error");
    std::clog << "log: user " << res << '\n';
    uint64_t rnd = gen();
    std::string salt, message;
    cpp::StringSource((const cpp::byte*)&rnd,
                      8,
                      true,
                      new cpp::HexEncoder(new cpp::StringSink(salt)));
    rc = send(work_sock, salt.c_str(), 16, 0);
    if (rc == -1)
        throw std::system_error(errno, std::generic_category(), "Salt send error");
    std::clog << "log: sending SALT " << salt << std::endl;
    cpp::Weak::MD5 hash;
    cpp::StringSource(salt + std::string(base[res]),
                      true,
                      new cpp::HashFilter(hash, new cpp::HexEncoder(new cpp::StringSink(message))));
    std::clog << "log: waiting MESSAGE " << message << std::endl;
    if (string_recv() != message)
        throw auth_error("Auth error: password mismatch");
    std::clog <<"log: auth success, sending OK\n";
    rc = send(work_sock, "OK", 2, 0);
    if (rc == -1)
        throw std::system_error(errno, std::generic_category(), "Send OK error");
}
