#pragma once
#include <string>
#include <iostream>

#include <boost/program_options.hpp>

namespace po = boost::program_options;

class Interface
{
private:
    uint32_t port;
    std::string base_file;
    std::string data;
    std::string b_data;
public:
    Interface(): port(0), base_file(" ")
    {}
    //Interface() = delete;
    //friend std::istream& operator >> (std::istream& cin, Interface& obj);
    void operator()(int sock);
    std::string data_set(std::string str);
    void set_options(int argscount, char *argvectors[]);
    std::string get_basefile() const;
    uint32_t get_port() const;
    //std::string string_recv();
    //std::string data_byte(std::string str);
    // std::string data_unbyte(std::string str);
    // void write_help();

};
