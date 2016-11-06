#include <sstream>
#include <fstream>
#include <iostream>

#include "analyzer.h"
#include "product.h"
#include "tokenizer.h"

int main(int argc, char const *argv[]) {
    std::ifstream fin(argv[1]);
    std::string input_str((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());

    Tokenizer tokenizer(input_str);
    tokenizer.tokenize();

    Analyzer analyzer;
    double result = analyzer.analysis(tokenizer.get_product1(), tokenizer.get_product2());

    std::ofstream fout(argv[2]);
    if (result >= 0.5) {
        fout<<1;
    } else {
        fout<<0;
    }

    return 0;
}
