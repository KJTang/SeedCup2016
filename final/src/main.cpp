#include <sstream>
#include <fstream>
#include <iostream>

#include "analyzer.h"
#include "product.h"
#include "tokenizer.h"


;

int main(int argc, char const *argv[]) {
    //Product p1, p2;
    //// p1.

    //Analyzer analyzer;
    //double result = analyzer.analysis(p1, p2);
    //std::cout<<"Result: "<<result<<std::endl;

    //cppjieba::Jieba jieba(DICT_PATH,
    //  HMM_PATH,
    //  USER_DICT_PATH,
    //  IDF_PATH,
    //  STOP_WORD_PATH);
    //std::vector<std::string> words;
    //std::vector<cppjieba::Word> jiebawords;
    //std::string s;

    //s = "他来到了网易杭研大厦";
    //std::cout << s << std::endl;
    //std::cout << "[demo] Cut With HMM" << std::endl;
    //jieba.Cut(s, words, true);
    //std::cout << limonp::Join(words.begin(), words.end(), "/") << std::endl;

    std::ifstream fin(argv[1]);
    //std::cout << argv[1] <<std::endl;
    std::string input_str((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());

    Tokenizer tokenizer(input_str);
    tokenizer.tokenize();
    //cout << endl;
    //cout << tokenizer.get_product1().title_ << endl;


    return 0;
}
