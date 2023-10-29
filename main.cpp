#include <string>
#include <vector>
#include <boost/program_options.hpp>

//#include "calc.h"
//#include "auth.h"
#include "interface.h"
//#include "interface.cpp"
#include "connection.h"

int main(int argc, char *argv[]){
    //Interface f;
    //f.data_read(argc, argv);
    Connection c;
    c.connect(33333, 10);
    return 0 ;
}
