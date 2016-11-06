#pragma once

#ifndef __TOKENIZER_H__
#define __TOKENIZER_H__
#include <cppjieba/Jieba.hpp>
#include <vector>
#include "product.h"

using namespace std;

class Tokenizer {
public:
  Tokenizer(std::string input) : str_(input) {
;
  }
  void tokenize();

  Product get_product1() {
    return product_1;
  }
  Product get_product2() {
    return product_2;
  }
 
private:
  std::string str_;

  vector<vector<string>> id_;
  vector<vector<string>> brand_;
  vector<vector<string>> series_;
  vector<vector<string>> markid_;
  vector<vector<string>> title_;

  vector<vector<string>> product1;
  vector<vector<string>> product2;
  Product product_1;
  Product product_2;

  void split(const std::string &s, char delim, std::vector<std::string> &elems);
  std::vector<std::string> split(const std::string &s, char delim);

  vector<string> seperate_one_line(string line);
  void deal_token(string t1, string t2, vector<vector<string>> types, cppjieba::Jieba& jieba);
};



#endif // !__TOKENIZER_H__
