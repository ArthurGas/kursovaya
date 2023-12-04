#include <string>
#include <cstring>
#include <boost/program_options.hpp>

#include "calc.h"
#include "auth.h"
#include "interface.h"
#include "connection.h"
#include "server_error.h"


int main(int argc, char *argv[])
{
    Interface i;
    i.set_options(argc, argv);
    try {
        Connection serv(i.get_port());
        Calculation c; // объект класса вычислений
        Auth a; // объект класса аутентификации
        a.base_read(i.get_basefile());
        serv.connect(c, a); // передача объекту значения сокета
        
    }catch(log_error &e){
        std::cerr<< e.what() << std::endl;
    }
    catch (std::system_error &e) {
        std::cerr << e.what() << std::endl;
        write_log(i.get_logfile(), e.what());
    }
    catch(std::exception &e){
        std::cerr<< e.what() << std::endl;
        write_log(i.get_logfile(), e.what());
    }
    return 0 ;
}
