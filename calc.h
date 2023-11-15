#pragma once
#include <stdint.h>
#include <vector>
#include <iostream>
#include <errno.h>
#include <float.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <cmath>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <typeinfo>
#include <memory>
#include <system_error>
#include <typeinfo>
#include <limits>
#include "interface.h"

class Calculation: private Interface
{
private:
    float result=1.0;
    uint32_t count;
    std::vector<float> vec_mean;
    uint32_t size;
    int work_sock;

public:
    Calculation(): work_sock(-1)
    {}
    void multip();
    void operator()(int sock);
    float overflow(float res);
};

//class Vector
//{
//private:
//   std::vector<float> vec_mean;
//   uint32_t size;
//};
