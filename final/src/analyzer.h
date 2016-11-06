#pragma once

#include <vector>
#include <string>

#include "product.h"

class Analyzer {
private:
    double analysis_describe(const std::vector<std::string>& d1, const std::vector<std::string>& d2);
    double analysis_string(const std::string& str1, const std::string& str2);
public:
    Analyzer() {}
    ~Analyzer() {}
    
    double analysis(const Product& p1, const Product& p2);
};