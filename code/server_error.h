#pragma once
#include <stdexcept>
#include <string>

class Error
{
private:
    bool iscritical=false;
public:
    std::string write_log(std::string file_name, std::string e_str);
    void critic_set();
};


class log_error: public std::runtime_error
{
public:
    log_error(const std::string& s) : std::runtime_error(s) {}
    log_error(const char * s) : std::runtime_error(s) {}
};

class auth_error: public std::invalid_argument
{
public:
    auth_error(const std::string& s) : std::invalid_argument(s) {}
    auth_error(const char * s) : std::invalid_argument(s) {}
};
class option_error: public std::runtime_error
{
public:
    option_error(const std::string& s) : std::runtime_error(s) {}
    option_error(const char * s) : std::runtime_error(s) {}
};

