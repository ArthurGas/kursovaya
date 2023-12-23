#include "auth.h"
#include "server_error.h"
#include <iostream>
#include <string>
#include <stdexcept>
namespace cpp = CryptoPP; 

void Auth::set_base(std::map<std::string, std::string> base_c)
{
    base_cont=base_c;
}
void Auth::operator()(int sock, Error& er)
{
    work_sock = sock;
    Auth::auth(er);
}
//модуль заполнения базы пользователей с помощью словаря
std::map<std::string, std::string> Auth::base_read(std::string file_name)
{

    std::string sep=":";
    size_t pos;
    std::string buf, login, pass;
    std::map<std::string, std::string> base;

    std::ifstream file(file_name.c_str());
    if (file.is_open()) {
        if(file.peek() == std::ifstream::traits_type::eof())
            throw std::length_error("base File Is Empty");
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
        return base;
    } else {
        throw std::system_error(errno, std::generic_category(), "base Read Error");
    }
}
std::string Auth::string_recv(Error& er)
{
    int rc;
    int buflen = 16;
    std::unique_ptr <char[]> buf(new char[buflen]);
    
    while (true) {
        rc = recv(work_sock, buf.get(), buflen, MSG_PEEK);
        if (rc == -1){
            er.critic_set();
            throw std::system_error(errno, std::generic_category(), "Recv string error");
        }
        if (rc < buflen)
            break;
        buflen *= 2;
        buf = std::unique_ptr<char[]>(new char[buflen]);
    }
    std::string res(buf.get(), rc);
    rc = recv(work_sock, nullptr, rc, MSG_TRUNC);
    if (rc == -1){
        er.critic_set();
        throw std::system_error(errno, std::generic_category(), "Clear bufer error");
    }
    res.resize(res.find_last_not_of("\n\r") + 1);
    return res;
}
bool Auth::id_check(std::string id, std::map<std::string, std::string> base_c)
{
    std::map<std::string, std::string> base(base_c);
    if (base.find(id) == base.end()){
        throw auth_error("Auth error: Identification error");
    }
	return true;
}
std::string Auth::salt_gen()
{
    std::mt19937_64 gen(time(nullptr));
    uint64_t rnd = gen();
    std::string salt;
    
    cpp::StringSource((const cpp::byte*)&rnd,
                      8,
                      true,
                      new cpp::HexEncoder(new cpp::StringSink(salt)));
    return(salt);
}
std::string Auth::pw_hash(std::string salt, std::string password){
    std::string hashed_pw;
    
    cpp::Weak::MD5 hash;
    cpp::StringSource(salt + std::string(password),
                      true,
                      new cpp::HashFilter(hash, new cpp::HexEncoder(new cpp::StringSink(hashed_pw))));
    return hashed_pw;
}
bool Auth::pw_check(std::string pw_from_cl, std::string hashed_pw){
	if (pw_from_cl != hashed_pw)
        throw auth_error("Auth error: password dismatch");
	return true;
}
void Auth::auth(Error& er)
{
    int rc;
    std::string id = string_recv(er);
    std::string salt, hashed_pw;
    
    id_check(id, base_cont);
    std::clog << "log: Пользователь: " << id << '\n';
    salt=salt_gen();
    rc = send(work_sock, salt.c_str(), 16, 0);
    if (rc == -1)
        
        throw std::system_error(errno, std::generic_category(), "Salt send error");
    std::clog << "log: Отправка SALT " << salt << std::endl;
    hashed_pw=pw_hash(salt, base_cont[id]);
    std::clog << "log: Ожидание хешированного пароля:" << hashed_pw << std::endl;
    if (string_recv(er) != hashed_pw)
        throw auth_error("Auth error: password dismatch");
    std::clog <<"log: Успешный вход, отправка сообщения ""OK""\n";
    rc = send(work_sock, "OK", 2, 0);
    if (rc == -1)
        throw std::system_error(errno, std::generic_category(), "Send ""OK"" error");
}