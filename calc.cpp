#include "calc.h"

float Calculation::multip(){
    std::cin >> size;
    for(uint i=0; i<size; i++){
        std::cin>>vec_mean[i];
        result += vec_mean[i];
    }
    return result;
}