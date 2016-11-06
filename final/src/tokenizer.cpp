#include "tokenizer.h"


void Tokenizer::tokenize() {
  vector<string> lines;
  lines = split(str_, '\n');
  //cout << lines << endl;
  vector<string> token1 = seperate_one_line(lines[0]);
  vector<string> token2 = seperate_one_line(lines[1]);
  //cout << token1 << endl;

  id_.push_back(vector<string>{token1[0]});
  id_.push_back(vector<string>{token2[0]});

  const char* const DICT_PATH = "../cppjieba/dict/jieba.dict.utf8";
  const char* const HMM_PATH = "../cppjieba/dict/hmm_model.utf8";
  const char* const USER_DICT_PATH = "../cppjieba/dict/user.dict.utf8";
  const char* const IDF_PATH = "../cppjieba/dict/idf.utf8";
  const char* const STOP_WORD_PATH = "../cppjieba/dict/stop_words.utf8";

  cppjieba::Jieba jieba(DICT_PATH,
    HMM_PATH,
    USER_DICT_PATH,
    IDF_PATH,
    STOP_WORD_PATH);

  vector<string> brand1, brand2;
  if (token1[1] != " null")
    jieba.Cut(token1[1], brand1, true);
  if (token2[1] != " null")
    jieba.Cut(token2[1], brand2, true);

  vector<string> brand1_f, brand2_f;
  for (auto n : brand1) {
    if (n != " " && n != "（" && n != "）")
      brand1_f.push_back(n);
  }
  for (auto n : brand2) {
    if (n != " " && n != "（" && n != "）")
      brand2_f.push_back(n);
  }

  brand_.push_back(brand1_f);
  brand_.push_back(brand2_f);
  //deal_token(token1[1], token2[1], brand_, jieba);
  //cout << brand_[0] << endl << brand_[1] << endl;

  vector<string> series1, series2;
  if (token1[2] != " null")
  jieba.Cut(token1[2], series1, true);
  if (token2[2] != " null")
  jieba.Cut(token2[2], series2, true);

  series_.push_back(series1);
  series_.push_back(series2);
  //cout << series_[0] << endl << series_[1] << endl;

  vector<string> markid1, markid2;
  if (token1[3] != " null")
  jieba.Cut(token1[3], markid1, true);
  if (token2[3] != " null")
  jieba.Cut(token2[3], markid2, true);
  markid_.push_back(markid1);
  markid_.push_back(markid2);
  //cout << markid_[0] << endl << markid_[1] << endl;

  vector<pair<string, string>> title1, title2;
  jieba.Tag(token1[4], title1);
  jieba.Tag(token2[4], title2);

  vector<string> title_1, title_2;
  jieba.Cut(token1[4], title_1);
  jieba.Cut(token2[4], title_2);

  vector<string> title1_f, title2_f;
  for (auto n : title_1) {
    if (n != " " && n != "（" && n != "）")
      title1_f.push_back(n);
  }
  for (auto n : title_2) {
    if (n != " " && n != "（" && n != "）")
      title2_f.push_back(n);
  }


  title_.push_back(title1_f);
  title_.push_back(title2_f);
  //for (auto n : title1) {
  //  cout << n.second;
  //  if (n.second == "n" || n.second == "eng") {
  //    title_1.push_back(n.first);
  //  }
  //}
  //for (auto n : title2) {
  //  if (n.second == "n" || n.second == "eng") {
  //    title_2.push_back(n.first);
  //  }
  //}

  //cout << title1 << endl << title2 << endl;
  // cout << title_1 << endl << title_2 << endl;

  //product1.push_back(id_[0]);
  //product1.push_back(brand_[0]);
  //product1.push_back(series_[0]);
  //product1.push_back(markid_[0]);
  //product1.push_back(title_[0]);

  product_1.id_ = id_[0];
  product_1.brand_ = brand_[0];
  product_1.type_ = series_[0];
  product_1.ext_ = markid_[0];
  product_1.title_ = title_[0];

  product_2.id_ = id_[1];
  product_2.brand_ = brand_[1];
  product_2.type_ = series_[1];
  product_2.ext_ = markid_[1];
  product_2.title_ = title_[1];

  //product2.push_back(id_[1]);
  //product2.push_back(brand_[1]);
  //product2.push_back(series_[1]);
  //product2.push_back(markid_[1]);
  //product2.push_back(title_[1]);
}

vector<string> Tokenizer::seperate_one_line(string line) {
  return split(line, ',');
}

void Tokenizer::deal_token(string t1, string t2, vector<vector<string>> types, cppjieba::Jieba& jieba) {
  vector<string> temp1, temp2;
  jieba.Cut(t1, temp1, true);
  jieba.Cut(t2, temp2, true);
  types.push_back(temp1);
  types.push_back(temp2);
}


void Tokenizer::split(const std::string &s, char delim, std::vector<std::string> &elems) {
  std::stringstream ss;
  ss.str(s);
  std::string item;
  while (std::getline(ss, item, delim)) {
    elems.push_back(item);
  }
}

std::vector<std::string> Tokenizer::split(const std::string &s, char delim) {
  std::vector<std::string> elems;
  split(s, delim, elems);
  return elems;
}