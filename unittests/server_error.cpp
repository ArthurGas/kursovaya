#include "server_error.h"
#include <string>
#include <fstream>
#include <ctime>
#include <algorithm>
std::string Error::write_log(std::string file_name, std::string e_str)
{
    std::string log_str;
    std::ofstream file(file_name.c_str(), std::ios::app);
    if (file.is_open()) {
        std::time_t now = time(0);
        std::string date=std::ctime(&now);
        log_str=" - "+e_str;
        date.erase(std::remove(date.begin(), date.end(), '\n'), date.cend());
        if(iscritical)
            file <<date+log_str+" - Critical error\n";
        else
            file <<date+log_str+" - Not Critical error\n";
        file.close();
    } else {
        throw log_error("Open log file error");
    }
    return log_str;
}
void Error::critic_set(){
        iscritical=true;
    }