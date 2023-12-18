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
        return(-FLT_MAX);
    else
        return res;
}
float Calculation::calc(std::vector<float> vec_mean)
{
    float result=1.0;
    for(auto num:vec_mean) {
        result*=num;
    }
    return result;
}
void Calculation::multip()
{
    int rc;
    float value;
    std::vector<float> vec_mean;
    
    rc = recv(work_sock, &count, sizeof(uint32_t), 0);
    if (rc == -1)
        throw std::system_error(errno, std::generic_category(), "Vector count recv error");
    std::clog << "log: Число векторов " << count << std::endl;
    for (uint32_t i=0; i < count; i++) {
        rc = recv(work_sock, &size, sizeof(uint32_t), 0);
        if (rc == -1)
            throw std::system_error(errno, std::generic_category(), "Vector size recv error");
        std::clog << "log: Размер вектора " << size << std::endl;
        std::clog << "log: Рачет значений вектора " << std::endl;
        std::clog << "log: Вектор [ ";
        for (uint32_t j=0; j < size; j++) {
            rc = recv(work_sock, &value,sizeof(uint32_t), 0);
            if (rc == -1)
                throw std::system_error(errno, std::generic_category(), "Recv vector error");
            std::clog << value << " ";
            vec_mean.push_back(value);
        }
        float endres = overflow(calc(vec_mean));
        std::clog << "]" << std::endl;
        std::clog << "log: Произведение вектора: " << endres << std::endl;
        send(work_sock, &endres, sizeof(uint32_t), 0);
    }
}
