#include <string>
#include <vector>
#include <boost/program_options.hpp>

//#include "calc.h"
//#include "auth.h"
#include "interface.h"
//#include "interface.cpp"
#include "connection.h"

namespace po = boost::program_options;

struct {
    uint32_t q=5;
    uint32_t p=33333;

    bool not_set()
    {
        return(q==5 && p==33333);
    }
} params;

int main(int argc, char *argv[])
{
    Interface f("3746 3434");

    po::options_description desc{"Options"};
    desc.add_options()
    ("help,h", "Show help")
    ("port,p", po::value<uint32_t>(&params.p),
     "Set server port")
    ("qlen,q", po::value<uint32_t>(&params.q),
     "Set queue len")
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
        std::cout << "Программа запущена с параметрами по умолчанию" << "\n" << "Порт: " << params.p << "\n";
        std::cout << "Длина очереди: " << params.q<< "\n";
        try {
            Connection serv(params.p, params.q);
            serv.connect(f);
        } catch (std::system_error &e) {
            std::cerr << e.what() << std::endl;
        }
    }
// вывод значений параметров
    else {
        std::cout << "Порт: " << params.p << "\n";
        std::cout << "Длина очереди: " << params.q<< "\n";
        Connection serv(params.p, params.q);
        serv.connect(f);
    }
    return 0 ;
}
