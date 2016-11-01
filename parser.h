/**
 *  Parser:
 *      used to analysis tokens. 
 *      when tokens match syntax, it will generate an abstract syntax tree (AST)
 *      for example, tokens: "a" "=" "1" "+" "b" "*" "2"
 *      will generate ast like:
 *
 *                          =
 *                        /   \
 *                       a     +
 *                           /   \
 *                          1     *
 *                              /   \
 *                             b     2
 */

/**
 *  EBNF:
 *      the ebnf used to parse is: 
 *
 *      const-int = ...
 *      const-string = ...
 *      variable = ...
 *
 *      block = "{", [ statement, { ";", statement } ], "}"
 *
 *      stat-declare = "int", variable | stat-assign, { ",", variable | stat-assign }
 *      stat-assign = variable, "=", const-int | expression
 *      stat-if = "if", "(", expression, ")", statement | block, { "else", statement | block }
 *      stat-while = "while", "(", expression, ")", statement | block
 *      stat-do = "do", statement | block, "while", "(", expression, ")"
 *      stat-for = "for", "(", expression, ";", expression, ";", expression, ")", statement | block
 *      stat-break = "break"
 *
 *      expr-single = (variable, "++" | "--") | ("-", variable)
 *      expr-comma = expression, ",", expression
 *      expr-callfunc = variable, "(", expression, { ",", expression } ")"
 *      expression = variable | expr-single | expr-callfunc, { operator, variable | expr-single | expr-callfunc }
 *
 *      statement = stat-declare | stat-assign | stat-if | stat-while | stat-do | stat-for | stat-break | expression | expr-comma
 */

#pragma once

#include <string>
#include <vector>
#include <map>

#include "lexer.h"
#include "astnode.h"

// operators(+ - / *) precedence
static std::map<Token, int> binary_operator_precedence = {
    { Token::OP_AND, 1 },                   // &&
    { Token::OP_OR, 1 },                    // ||
    { static_cast<Token>('<'), 2 },         // <
    { static_cast<Token>('>'), 2 },         // >
    { Token::OP_GTE, 2 },                   // >=
    { Token::OP_LTE, 2 },                   // <=
    { Token::OP_EQU, 2 },                   // ==
    { Token::OP_NE, 2 },                    // ==
    { static_cast<Token>('+'), 3 },         // +
    { static_cast<Token>('-'), 3 },         // -
    { static_cast<Token>('*'), 4 },         // *
    { static_cast<Token>('/'), 4 },         // /
};

class Parser {
private:
    // input
    std::vector<TokenStruct*> tokens_;
    // output
    std::vector<ASTNode*> astnode_list_;

    TokenStruct* cur_token_;
    int pos_ = 0;

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
    ASTNode* ParseExprComma(ASTNode* l, ASTNode* r);
    ASTNode* ParseExprCallfunc(ASTNode* var);
public:
    Parser();
    ~Parser();
    
    void Input(const std::vector<TokenStruct*>& tokens);
    void Output(std::vector<ASTNode*>& astnode_list);
    bool Parse();
};