#pragma once

#include <string>
#include <vector>
#include <map>

// #include "err_handler.h"
#include "lexer.h"
#include "astnode.h"

// operators(+ - / *) precedence
static std::map<Token, int> binary_operator_precedence = {
    {Token::OP_AND, 1}, 
    {Token::OP_OR, 1}, 
    {static_cast<Token>('<'), 2}, 
    {static_cast<Token>('>'), 2}, 
    {Token::OP_GTE, 2}, 
    {Token::OP_LTE, 2}, 
    {Token::OP_EQU, 2}, 
    {Token::OP_NE, 2}, 
    {static_cast<Token>('+'), 3}, 
    {static_cast<Token>('-'), 3}, 
    {static_cast<Token>('*'), 4}, 
    {static_cast<Token>('/'), 4},
};

class Parser {
private:
    // input
    std::vector<TokenStruct*> tokens_;
    // output
    std::vector<ASTNode*> astnode_list_;

    TokenStruct* cur_token_;
    int pos_ = 0;

    // count block {}
    int block_count_ = 0;

    ASTNode* ParseStatement();
    ASTNode* ParseBlock();

    ASTNode* ParseConstInt();
    ASTNode* ParseConstString();
    ASTNode* ParseVariable();

    ASTNode* ParseStatDeclare();
    ASTNode* ParseStatAssign(ASTNode* var);
    ASTNode* ParseStatIf();
    ASTNode* ParseStatWhile();
    ASTNode* ParseStatDo();
    ASTNode* ParseStatFor();
    ASTNode* ParseStatBreak();

    ASTNode* ParseExpression();
    ASTNode* ParseExprParen();
    ASTNode* ParseExprSingle(ASTNode* var, Token op);
    ASTNode* ParseExprComma();
    ASTNode* ParseExprCallfunc(ASTNode* var);
public:
    Parser();
    ~Parser();
    
    void Input(const std::vector<TokenStruct*>& tokens);
    void Output(std::vector<ASTNode*>& astnode_list);
    bool Parse();
};