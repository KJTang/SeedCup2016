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
    static constexpr double ratio_[5] = {0.0, 0.6, 0.1, 0.1, 0.2};
    double analysis_describe(const std::vector<std::string>& d1, const std::vector<std::string>& d2);
    double analysis_string(const std::string& str1, const std::string& str2);
public:
    Analyzer() {}
    ~Analyzer() {}
    
    double analysis(const Product& p1, const Product& p2);
};