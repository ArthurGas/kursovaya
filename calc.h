#pragma once
#include <stdint.h>
#include <vector>
#include <iostream>
#include <errno.h>
#include <float.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <memory>
#include <system_error>
#include "interface.h"

class Calculation
{
private:
    uint32_t count;
    uint32_t size;
    int work_sock;

public:
    Calculation(): work_sock(-1)
    {}
    void multip();
    float calc(std::vector<float> vector);
    void operator()(int sock);
    float overflow(float res);
};
