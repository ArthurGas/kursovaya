#include "auth.h"
#include "server_error.h"
#include <iostream>
#include <string>
#include <stdexcept>

void Auth::operator()(int sock)
{
    work_sock = sock;
    Auth::auth();
}
//модуль заполнения базы пользователей с помощью словаря
Void Auth::base_read(std::string File_name)
{
    Std::string Sep=":";
    Size_t Pos;
    Std::string Buf, Login, Pass;

    Std::ifstream File(file_name.C_str());
    If (file.Is_open()) {
        If(file.Peek() == Std::ifstream::traits_type::eof())
            Throw Std::length_error("base File Is Empty");
        While(getline(file, Buf)) {
            While (buf.Find(" ") < Buf.Size()) {
                Buf.Erase(buf.Find(" "), 1);
            }
            Pos = Buf.Find(sep, 0);
            Login = Buf.Substr(0, Pos);
            Pass = Buf.Substr(pos+1, Buf.Size());
            Base[login] = Pass;
        }
        File.Close();
    }
    Else {
        Throw Std::system_error(errno, Std::generic_category(), "base Read Error");
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
        throw std::system_error(errno, std::generic_category(), "Send ""OK"" error");
}
