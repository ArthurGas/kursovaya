#include "interface.h"

namespace po = boost::program_options;
struct {
    std::string userName;
    uint32_t vectorCount=0;
    uint32_t vectorSize=0;
    float * vectorMean = new float [vectorSize];

    bool not_set()
    {
        return userName.empty() || vectorCount==0;
    }
} params;

std::string Interface::data_read(int argc, char *argv[])
{
    po::options_description desc{"Options"};
    desc.add_options()
    ("help,h", "Show help")
    //("input,i", po::value<std::string>(&params.inFileName)->
    // default_value("input.txt"),"Set input file name")
    ("user,u", po::value<std::string>(&params.userName),
     "Set user name")
    ("count,c", po::value<uint32_t>(&params.vectorCount),
     "Set vectors count")
    //("size,s", po::value<uint32_t>(&params.vectorSize),
    //"Set vector size")
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
    }
// вывод значений параметров для варианта "Выполнить вычисления"
    else {
        std::cout << "User: " << params.userName << "\n";
        std::cout << "Vectors count: " << params.vectorCount << "\n";
        for(uint i=0; i<params.vectorCount; i++){
            std::cin >> params.vectorSize;
            for(uint j=0; j<params.vectorSize; j++){
                std::cin >> params.vectorMean[j];
            }
            std::cout << "Vector size: " << params.vectorSize << "\n";
            std::cout << "Vector Mean: [" ;
            for(uint j=0; j<params.vectorSize; j++){
                std::cout << params.vectorMean[j]<< " ";
            }
            std::cout << "]";
            std::cout<<"\n";
        }
        //std::cout << "Vectors size: " << params.vectorSize << "\n";
    }

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
