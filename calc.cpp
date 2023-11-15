#include "calc.h"
void Calculation::operator()(int sock)
{
    work_sock = sock;
    Calculation::multip();
}

float Calculation::overflow(float res)
{
    if(res==INFINITY)
        return(FLT_MAX);
    else if(res==-INFINITY)
        return(FLT_MAX);
    else
        return res;
}

void Calculation::multip()
{
    int rc;
    //size_t len = 32; //начальный размер массива принимаемых данных
    // char buf[len];
    float value;
    //char msg[sizeof(uint32_t)];
    std::vector<float> msg_cont;
    rc = recv(work_sock, &count, sizeof(uint32_t), 0);
    if (rc == -1)
        throw std::system_error(errno, std::generic_category(), "Vector count recv error");
    std::clog << "log: Число векторов " << count << std::endl;
    for (uint32_t i=0; i < count; i++) {
        rc = recv(work_sock, &size, sizeof(uint32_t), 0);
        if (rc == -1)
            throw std::system_error(errno, std::generic_category(), "Vector size recv error");
        std::clog << "log: Размер вектора " << size << std::endl;
        //std::clog << "log: Рачет значений вектора " << std::endl;
        std::clog << "log: Вектор [ ";
        for (uint32_t j=0; j < size; j++) {
            rc = recv(work_sock, &value,sizeof(uint32_t), 0);
            if (rc == -1)
                throw std::system_error(errno, std::generic_category(), "Recv vector error");
            std::clog << value << " ";
            result*=value;
        }
        float endres = overflow(result);
        std::clog << "]" << std::endl;
        std::clog << "log: Произведение вектора: " << endres << std::endl;
        send(work_sock, &endres, sizeof(uint32_t), 0);
        result = 1;
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
