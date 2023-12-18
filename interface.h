#pragma once
#include <string>
#include <iostream>
#include <ctime>
#include <boost/program_options.hpp>
#include <fstream>

namespace po = boost::program_options;

class Interface
{
private:
    uint32_t port;
    std::string base_file;
    std::string log_file;
    void operator()(int sock);
public:
    Interface(): port(0), base_file(" ")
    {}
    std::string data_set(std::string str);
    bool set_options(int argscount, char *argvectors[]);
    std::string get_basefile() const;
    uint32_t get_port() const;
    std::string get_logfile() const;
 
};
