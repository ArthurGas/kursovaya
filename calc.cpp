#include "calc.h"

void Calculation::operator()(int sock)
{
    work_sock = sock;
    Calculation::multip();
}
//float
void Calculation::multip()
{
    int rc;
    size_t len = 32; //начальный размер массива принимаемых данных
    char buf[len];
    char msg[sizeof(uint32_t)];
    std::vector<float> msg_cont;
    float mult1;
    float mult2;
    rc = recv(work_sock, buf, len, 0);
    if (rc == -1)
        throw std::system_error(errno, std::generic_category(), "Recv number of vectors error");
    for(uint i=0; i<sizeof(uint32_t); i++) {
        msg[i]=buf[i];
    }
    count = atoi(msg);
    std::clog << "log: Число векторов " << count << std::endl;
    for (uint32_t i=0; i < count; i++) {
        rc = recv(work_sock, buf, len, 0);
        if (rc == -1)
            throw std::system_error(errno, std::generic_category(), "Recv vector size error");
        for(uint i=0; i<sizeof(uint32_t); i++) {
            msg[i]=buf[i];
        }
        size = atoi(msg);
        std::clog << "log: Размер вектора " << size << std::endl;
        //std::clog << "log: Рачет значений вектора " << std::endl;
        std::clog << "log: Вектор [ ";
        for (uint32_t j=0; j < size; j++) {
            rc = recv(work_sock, buf, len, 0);
            if (rc == -1)
                throw std::system_error(errno, std::generic_category(), "Recv vector error");
            for(uint i=0; i<sizeof(uint32_t); i++) {
                msg[i]=buf[i];
            }
            std::clog << atof(msg) << " ";
            if(i%2!=0){
                mult1 = atof(msg);
                result=result*mult1;
            }else{
                mult2 = atof(msg);
                result=result*mult2;
            }
        }
        std::clog << "]" << std::endl;
        std::clog << "log: Произведение вектора: " << result << std::endl;
    }
}
/*
for (uint32_t i = 0; i < count; ++i) {
    std::vector<char> buf();
    rc = recv(work_sock, &size, sizeof size, 0);
    if (rc == -1)
        throw std::system_error(errno, std::generic_category(), "Recv vector size error");
    std::clog << "log: Vector " << i <<" size " << size<< std::endl;
    //std::unique_ptr<float> data (new size);
    //std::clog << "log: Memory allocated at "  << data.get() << std::endl;
    //rc = recv(work_sock, data.get(), sizeof(float) * size, 0);

}
 * */
