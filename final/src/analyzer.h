#pragma once

#include <vector>
#include <string>

#include "product.h"

class Analyzer {
private:
    double analysis_id(const std::vector<std::string> p1, const std::vector<std::string> p2);
    double analysis_brand(const std::vector<std::string> p1, const std::vector<std::string> p2);
    double analysis_type(const std::vector<std::string> p1, const std::vector<std::string> p2);
    double analysis_ext(const std::vector<std::string> p1, const std::vector<std::string> p2);
    double analysis_title(const std::vector<std::string> p1, const std::vector<std::string> p2);
public:
    Analyzer() {}
    ~Analyzer() {}
    
    double analysis(Product p1, Product p2);
};