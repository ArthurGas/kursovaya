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
    Interface i;// объект класса интерфейса
    Error er; //объект класс для обработки ошибок
    try {
        if(!i.set_options(argc, argv, er))
            return 0;
        Connection serv(i.get_port());
        Calculation c; // объект класса вычислений
        Auth a; // объект класса аутентификации

        std::map<std::string, std::string>bs=a.base_read(i.get_basefile());
        a.set_base(bs);
        serv.connect(c, a, er); // передача объекту значения сокета

    } catch(std::logic_error &e) {
        std::cerr<< e.what() << std::endl;
    }catch(option_error &e) { // ошибка опций не может быть записана в неоткрытый файл
        std::cerr<< e.what() << std::endl;
    } catch(log_error &e) { // ошибка открытия лог-файла не модет быть записана
        std::cerr<< e.what() << std::endl;
    } catch(std::exception &e) {
        std::cerr<< e.what() << std::endl;
        er.write_log(i.get_logfile(), e.what());
    }
    return 0 ;
}
