#include <iostream>

#include "analyzer.h"
#include "product.h"
#include "tokenizer.h"

using namespace std;

const char* const DICT_PATH = "../cppjieba/dict/jieba.dict.utf8";
const char* const HMM_PATH = "../cppjieba/dict/hmm_model.utf8";
const char* const USER_DICT_PATH = "../cppjieba/dict/user.dict.utf8";
const char* const IDF_PATH = "../cppjieba/dict/idf.utf8";
const char* const STOP_WORD_PATH = "../cppjieba/dict/stop_words.utf8";

int main(int argc, char const *argv[]) {
    Product p1, p2;

    // cppjieba::Jieba jieba(DICT_PATH,
    //   HMM_PATH,
    //   USER_DICT_PATH,
    //   IDF_PATH,
    //   STOP_WORD_PATH);
    // vector<string> words;
    // vector<cppjieba::Word> jiebawords;
    // string s;

    // s = "他来到了网易杭研大厦";
    // cout << s << endl;
    // cout << "[demo] Cut With HMM" << endl;
    // jieba.Cut(s, words, true);
    // cout << limonp::Join(words.begin(), words.end(), "/") << endl;

    Analyzer analyzer;
    double result = analyzer.analysis(p1, p2);
    std::cout<<"Result: "<<result<<std::endl;

    return 0;
}
