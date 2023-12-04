#pragma once
#include <stdexcept>
#include <string>
class log_error: public std::runtime_error
{
public:
    log_error(const std::string& s) : std::runtime_error(s) {}
    log_error(const char * s) : std::runtime_error(s) {}
};

class auth_error: public std::runtime_error
{
public:
    auth_error(const std::string& s) : std::runtime_error(s) {}
    auth_error(const char * s) : std::runtime_error(s) {}
};

class vector_error: public std::invalid_argument
{
public:
    vector_error(const std::string& s) : std::invalid_argument(s) {}
    vector_error(const char * s) : std::invalid_argument(s) {}
};

class option_error: public std::runtime_error
{
public:
    option_error(const std::string& s) : std::runtime_error(s) {}
    option_error(const char * s) : std::runtime_error(s) {}
};

inline std::string write_log(std::string file_name, std::string e_str)
{
    std::string log_str;
    std::ofstream file(file_name.c_str(), std::ios::app);
    if (file.is_open()) {
        std::time_t now = time(0);
        char* date=std::ctime(&now);
        log_str=(std::string)date+" - "+e_str+'\n';
        file << log_str.c_str();
        file.close();
    } else {
        throw log_error("Open log file error");
    }
    return log_str;
}
