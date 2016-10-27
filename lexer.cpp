#include "lexer.h"

Lexer::Lexer() {}

Lexer::~Lexer() {
    for (int i = 0; i != tokens_.size(); ++i) {
        delete tokens_[i];
    }
}

void Lexer::Input(const std::string& raw_str) {
    raw_str_ = raw_str;
}

void Lexer::Output(std::vector<TokenStruct*>& tokens) {
    tokens = tokens_;

    // Test
    std::cout<<"======================== Lex Start ======================"<<std::endl;
    for (int i = 0; i != tokens_.size(); ++i) {
        std::cout<<(int)tokens_[i]->type<<"\t"<<tokens_[i]->value<<"\t"<<tokens_[i]->line<<std::endl;
    }
    std::cout<<"======================== Lex End ========================"<<std::endl;
}

bool Lexer::Lex() {
    Token result;
    do {
        result = this->LexToken();
    } while (result != Token::END_OF_FILE);
	return true;
}

Token Lexer::LexToken() {
    cur_char_ = raw_str_[pos_++];
    // shift space and '\n'
    while (cur_char_ == ' ' || cur_char_ == '\n') {
        if (cur_char_ == '\n') {
            ++line_;
        }
        cur_char_ = raw_str_[pos_++];
    }
    // identifier
    // def: start with [a-zA-Z_], follow by [a-zA-Z_0-9]
    if (isalpha(cur_char_) || cur_char_ == '_') {
        return LexIdentifier();
    }

    // const int/double
    if (isdigit(cur_char_)) {
        return LexNumber();
    }
    // const string
    if (cur_char_ == '\"') {
        return LexString();
    }

    switch (cur_char_) {
        // end of file
        case '\0': {
            tokens_.push_back(new TokenStruct(Token::END_OF_FILE, "\0", line_));
            return Token::END_OF_FILE;
        }
        case '>': {
            if (raw_str_[pos_] == '=') {
                tokens_.push_back(new TokenStruct(Token::OP_GTE, ">=", line_));
                ++pos_;
                return Token::OP_GTE;
            }
            goto ASCII;
        }
        case '<': {
            if (raw_str_[pos_] == '=') {
                tokens_.push_back(new TokenStruct(Token::OP_LTE, "<=", line_));
                ++pos_;
                return Token::OP_LTE;
            }
            goto ASCII;
        }
        case '=': {
            if (raw_str_[pos_] == '=') {
                tokens_.push_back(new TokenStruct(Token::OP_EQU, "==", line_));
                ++pos_;
                return Token::OP_EQU;
            }
            goto ASCII;
        }
        case '!': {
            if (raw_str_[pos_] == '=') {
                tokens_.push_back(new TokenStruct(Token::OP_NE, "!=", line_));
                ++pos_;
                return Token::OP_NE;
            }
            goto ASCII;
        }
        case '&': {
            if (raw_str_[pos_] == '&') {
                tokens_.push_back(new TokenStruct(Token::OP_AND, "&&", line_));
                ++pos_;
                return Token::OP_AND;
            }
            goto ASCII;
        }
        case '|': {
            if (raw_str_[pos_] == '|') {
                tokens_.push_back(new TokenStruct(Token::OP_OR, "||", line_));
                ++pos_;
                return Token::OP_OR;
            }
            goto ASCII;
        }
        case '+': {
            if (raw_str_[pos_] == '+') {
                tokens_.push_back(new TokenStruct(Token::OP_INCREASE, "++", line_));
                ++pos_;
                return Token::OP_INCREASE;
            }
            goto ASCII;
        }
        case '-': {
            if (raw_str_[pos_] == '-') {
                tokens_.push_back(new TokenStruct(Token::OP_DECREASE, "--", line_));
                ++pos_;
                return Token::OP_DECREASE;
            }
            goto ASCII;
        }
        // comment
        // def: start with "//", will comment one line
        //    : start with "/*", will comment until "*/"
        case '/': {
            // "//" comment
            if (raw_str_[pos_] == '/') {
                do {
                    cur_char_ = raw_str_[pos_++];
                } while (cur_char_ != '\0' && cur_char_ != '\n');
                if (cur_char_ != '\0') {
                    ++line_;
                    return this->LexToken();
                } else {
                    tokens_.push_back(new TokenStruct(Token::END_OF_FILE, "\0", line_));
                    return Token::END_OF_FILE;
                }
            // "/*" comment
            } else if (raw_str_[pos_] == '*') {
                do {
                    cur_char_ = raw_str_[pos_++];
                    if (cur_char_ == '\n') {
                        ++line_;
                    } else if (cur_char_ == '*') {
                        if (raw_str_[pos_] == '/') {
                            ++pos_;
                            break;
                        }
                    }
                } while (cur_char_ != '\0');
                if (cur_char_ != '\0') {
                    return this->LexToken();
                } else {
                    tokens_.push_back(new TokenStruct(Token::END_OF_FILE, "\0", line_));
                    return Token::END_OF_FILE;
                }
            }
            // if not "//", or "/*", jump to ascii
            goto ASCII;
        }
        // ascii
        default: {
            ASCII:
            tokens_.push_back(new TokenStruct((Token)cur_char_, std::string(1, cur_char_), line_));
            return (Token)cur_char_;
        }
    }
}

Token Lexer::LexIdentifier() {
    std::string identifier_name;
    cur_char_ = raw_str_[pos_-1];

    identifier_name += cur_char_;
    cur_char_ = raw_str_[pos_++];
    while (isalnum(cur_char_) || cur_char_ == '_') {
        identifier_name += cur_char_;
        cur_char_ = raw_str_[pos_++];
    }
    --pos_;
    // keywords
    if (identifier_name == "if") {
        tokens_.push_back(new TokenStruct(Token::KEYWORD_IF, identifier_name, line_));
        return Token::KEYWORD_IF;
    } else if (identifier_name == "else") {
        tokens_.push_back(new TokenStruct(Token::KEYWORD_ELSE, identifier_name, line_));
        return Token::KEYWORD_ELSE;
    } else if (identifier_name == "while") {
        tokens_.push_back(new TokenStruct(Token::KEYWORD_WHILE, identifier_name, line_));
        return Token::KEYWORD_WHILE;
    } else if (identifier_name == "do") {
        tokens_.push_back(new TokenStruct(Token::KEYWORD_DO, identifier_name, line_));
        return Token::KEYWORD_DO;
    } else if (identifier_name == "for") {
        tokens_.push_back(new TokenStruct(Token::KEYWORD_FOR, identifier_name, line_));
        return Token::KEYWORD_FOR;
    } else if (identifier_name == "break") {
        tokens_.push_back(new TokenStruct(Token::KEYWORD_BREAK, identifier_name, line_));
        return Token::KEYWORD_BREAK;
    } else if (identifier_name == "int") {
        tokens_.push_back(new TokenStruct(Token::KEYWORD_INT, identifier_name, line_));
        return Token::KEYWORD_INT;
    }
    tokens_.push_back(new TokenStruct(Token::IDENTIFIER, identifier_name, line_));
    return Token::IDENTIFIER;
}

Token Lexer::LexNumber() {
    std::string const_str;
    cur_char_ = raw_str_[pos_-1];

    const_str += cur_char_;
    cur_char_ = raw_str_[pos_++];
    bool is_int = true;
    while (isdigit(cur_char_) || cur_char_ == '.') {
        if (cur_char_ == '.') {
            is_int = false;
        }
        const_str += cur_char_;
        cur_char_ = raw_str_[pos_++];
    }
    --pos_;
    if (is_int) {
        tokens_.push_back(new TokenStruct(Token::CONST_INT, const_str, line_));
        return Token::CONST_INT;
    } else {
        tokens_.push_back(new TokenStruct(Token::CONST_DOUBLE, const_str, line_));
        return Token::CONST_DOUBLE;
    }
}

Token Lexer::LexString() {
    std::string const_str;

    cur_char_ = raw_str_[pos_++];
    while (cur_char_ != '\"') {
        if (pos_ >= raw_str_.size()) {
            // ERR(line_, "Missing \"");
            return Token::END_OF_FILE;
        }
        const_str += cur_char_;
        cur_char_ = raw_str_[pos_++];
    }
    tokens_.push_back(new TokenStruct(Token::CONST_STRING, const_str, line_));
    return Token::CONST_STRING;
}