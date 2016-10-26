#pragma once

#include <iostream>
#include <vector>

enum class Token : int {
    IDENTIFIER = 256, 

    CONST_INT,          // const int: 123
    CONST_DOUBLE,          // const double: 12.3
    CONST_STRING,       // const string: "str"

    OP_GTE,             // >=
    OP_LTE,             // <=
    OP_EQU,             // ==
    OP_NE,              // !=
    OP_AND,             // &&
    OP_OR,              // ||
    OP_INCREASE,        // ++
    OP_DECREASE,        // --

    KEYWORD_IF,         // if
    KEYWORD_ELSE,       // else
    KEYWORD_WHILE,      // while
    KEYWORD_DO,         // do
    KEYWORD_FOR,        // for
    KEYWORD_BREAK,      // break
    KEYWORD_INT,        // int

    END_OF_FILE = -1
};

struct TokenStruct {
    Token type;
    std::string value;
    int line;

    TokenStruct(Token t, const std::string& v, int l) {
        type = t;
        value = v;
        line = l;
    }
};

class Lexer {
private:
    // input
    std::string raw_str_;
    // output
    std::vector<TokenStruct*> tokens_;

    char cur_char_;
    int pos_ = 0;
    int line_ = 1;

    Token LexToken();

    Token LexIdentifier();
    Token LexNumber();
    Token LexString();
public:
    Lexer();
    ~Lexer();
    
    void Input(const std::string& raw_str);
    void Output(std::vector<TokenStruct*>& tokens);
    bool Lex();
};