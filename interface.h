#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <boost/program_options.hpp>

class Interface
{
private:
    std::string data;
    std::string b_data;
    int work_sock;
public:
    Interface(std::string d=" ")
    {
        data = d;
    }
    //Interface() = delete;
    //friend std::istream& operator >> (std::istream& cin, Interface& obj);
    std::string data_set(std::string str);
    std::string data_get() const;
    std::string data_byte(std::string str);
    std::string data_unbyte(std::string str);
    void operator()(int sock);
   // void write_help();

};
