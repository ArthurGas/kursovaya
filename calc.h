#pragma once
#include <stdint.h>
#include <vector>
#include <iostream>
class Calculation
{
private:
    float result=0;
    uint32_t count;
    std::vector<float> vec_mean;
    uint32_t size;

public:
    float multip();
    bool overflow();
};

//class Vector
//{
//private:
 //   std::vector<float> vec_mean;
 //   uint32_t size;
//};
