#include <string>
#include <cstring>
#include <boost/program_options.hpp>

#include "calc.h"
#include "auth.h"
#include "interface.h"
#include "connection.h"

namespace po = boost::program_options;

struct {
    uint32_t q=5;
    uint32_t p=33333;
    std::string b = "base.txt";
    bool not_set()
    {
        return(q==5 && p==33333 && b=="base.txt");
    }
} params;

int main(int argc, char *argv[])
{
    po::options_description desc{"Options"};
    desc.add_options()
    ("help,h", "Show help")
    ("port,p", po::value<uint32_t>(&params.p),
     "Set server port")
    ("qlen,q", po::value<uint32_t>(&params.q),
     "Set queue len")
    ("base,b", po::value<std::string>(&params.b),
     "Set base filename")
    ;
// переменная для результатов парсинга
    po::variables_map vm;
// парсинг
    po::store(po::parse_command_line(argc, argv, desc), vm);
// присвоение значений по умолчанию
    po::notify(vm);
// выполнение варианта "Выдать справку"
    if(vm.count("help") || params.not_set()) {
        std::cout << desc << "\n";
        std::cout << "Program use default parameters" << "\n" << "Port: " << params.p << "\n";
        std::cout << "Queue length: " << params.q<< "\n";
        std::cout << "Base filename: " << params.b<< "\n";
        try {
            Connection serv(params.p, params.q);
            Calculation c; // объект класса вычислений
            Auth a; // объект класса аутентификации
            Interface i;
            a.base_read(params.b);
            serv.connect(c, a, i); // передача объекту значения сокета
        } catch (std::system_error &e) {
            std::cerr << e.what() << std::endl;
        }
    }
// вывод значений параметров
    else {
        std::cout << "Порт: " << params.p << "\n";
        std::cout << "Длина очереди: " << params.q<< "\n";
        Connection serv(params.p, params.q);
        Calculation c;
        Auth a;
        Interface i;
        serv.connect(c, a, i);


    }
    return 0 ;
}
