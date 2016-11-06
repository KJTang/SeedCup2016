#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "product.h"

static void print(const std::vector<std::string> v) {
    std::cout<<"-----"<<std::endl;
    for (auto str : v) {
        std::cout<<str<<std::endl;
    }
    std::cout<<"-----"<<std::endl;
}

class Analyzer {
private:
    double analysis_describe(const std::vector<std::string>& d1, const std::vector<std::string>& d2);
    double analysis_string(const std::string& str1, const std::string& str2);
public:
    Analyzer() {}
    ~Analyzer() {}
    
    double analysis(const Product& p1, const Product& p2);
};