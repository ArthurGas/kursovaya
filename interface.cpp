#include "interface.h"
#include <sys/socket.h>
#include <sys/ioctl.h>

std::string Interface::data_set(std::string str)
{
    return(data);
}
/*std::istream& operator >> (std::istream& cin, Interface& obj)
{
    cin >> obj.data;
    return cin;
}
*/
std::string Interface::data_get() const
{
    return data;
}
std::string Interface::data_byte(std::string str)
{
    return b_data;
}
std::string Interface::data_unbyte(std::string str)
{
    return data;
}
void Interface::operator()(int sock)
{
    work_sock = sock;
    int rc;
    char msg[15];
    rc = recv(work_sock, msg, sizeof msg, 0);
    std::cout << msg;
}
/*
void Interface::write_help()
{
    std::fstream fs;
    fs.open("/home/stud/Kursovaya/Code/readme.md");
    if(!fs.is_open()) {
        std::cout<<"Файл не открыт";
    } else {
        std::string line;
        while (std::getline(fs, line)) {
            std::cout << line << std::endl;
        }
    }
    fs.close();
}
 */
