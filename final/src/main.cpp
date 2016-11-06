#include <iostream>

#include "analyzer.h"
#include "product.h"

int main(int argc, char const *argv[]) {
    Product p1, p2;
    // p1.

    Analyzer analyzer;
    double result = analyzer.analysis(p1, p2);
    std::cout<<"Result: "<<result<<std::endl;

    return 0;
}