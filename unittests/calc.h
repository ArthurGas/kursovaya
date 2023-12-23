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
#include "server_error.h"

class Calculation
{
private:
    uint32_t count;
    uint32_t size;
    int work_sock;

public:
    Calculation(): work_sock(-1)
    {}
    void calc(Error& er);
    float multip(std::vector<float> vector);
    void operator()(int sock, Error& er);
    float overflow(float res);
};
