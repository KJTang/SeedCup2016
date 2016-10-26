#include "parser.h"

#include <iostream>
#include <stack>

// #define ERR(line, str) \
// do { \
//     err_occur_ = true; \
//     ErrorHandler::GetInstance()->ThrowError(line, str); \
// } while (false);

// #define WARN(line, str) \
// do { \
//     ErrorHandler::GetInstance()->ThrowWarning(line, str); \
// } while (false);

Parser::Parser() {}

Parser::~Parser() {
    for (int i = 0; i != astnode_list_.size(); ++i) {
        delete astnode_list_[i];
    }
}

void Parser::Input(const std::vector<TokenStruct*>& tokens) {
    tokens_ = tokens;
}

void Parser::Output(std::vector<ASTNode*>& astnode_list) {
    astnode_list = astnode_list_;

    // Test
    std::cout<<"======================== Parse Start ===================="<<std::endl;
    for (int i = 0; i != astnode_list_.size(); ++i) {
        astnode_list_[i]->Print();
    }
    std::cout<<"======================== Parse End ======================"<<std::endl;
}

bool Parser::Parse() {
    cur_token_ = tokens_[pos_++];
    while (pos_ < tokens_.size()) {
        ASTNode* node = ParseStatement();
        if (node) {
            astnode_list_.push_back(node);
        }
    }
    return true;
}

ASTNode* Parser::ParseStatement() {
    switch (cur_token_->type) {
        case Token::END_OF_FILE: {
            cur_token_ = tokens_[pos_++];
            return nullptr;
        }
        case Token::KEYWORD_INT: {
            return ParseStatDeclare();
        }
        case Token::KEYWORD_IF: {
            return ParseStatIf();
        }
        case Token::KEYWORD_WHILE: {
            return ParseStatWhile();
        }
        case Token::KEYWORD_DO: {
            return ParseStatDo();
        }
        case Token::KEYWORD_FOR: {
            return ParseStatFor();
        }
        case Token::KEYWORD_BREAK: {
            return ParseStatBreak();
        }
        case static_cast<Token>(';'): {
            cur_token_ = tokens_[pos_++];
            return ParseStatement();
        }
        case static_cast<Token>('}'): {
            if (block_count_ <= 0) {
                // WARN(cur_token_->line, "unexpected token '}'");
                cur_token_ = tokens_[pos_++];   // eat '}'
                return ParseStatement();
            }
        }
        case Token::IDENTIFIER: {
            if (tokens_[pos_]->type == static_cast<Token>('=')) {
                return ParseStatAssign(ParseVariable());
            }
        }
        case static_cast<Token>('('): 
        case Token::CONST_INT: 
        case Token::CONST_STRING: { 
            return ParseExpression();
        }
        default: {
            // ERR(cur_token_->line, "invalid expression");
            // std::cout<<cur_token_->value<<std::endl;
            return nullptr;
        }
    }
}

ASTNode* Parser::ParseBlock() {
    std::cout<<"ParseBlock: "<<cur_token_->value<<std::endl;

    int line = cur_token_->line;
    cur_token_ = tokens_[pos_++];   // eat '{'
    ++block_count_;
    std::vector<ASTNode*> statements;

    if (static_cast<int>(cur_token_->type) == '}') {
        cur_token_ = tokens_[pos_++];   // eat '}'
        --block_count_;
    } else {
        bool is_finished = false;
        while (!is_finished) {
            ASTNode* stat = ParseStatement();
            if (stat) {
                statements.push_back(stat);
            }
            if (static_cast<int>(cur_token_->type) == '}') {
                is_finished = true;
            }
        }
        cur_token_ = tokens_[pos_++];   // eat '}'
        --block_count_;
    }

    ASTNode* node = new ASTBlock(line, statements);
    return node;
}

ASTNode* Parser::ParseConstInt() {
    std::cout<<"ParseConstInt: "<<cur_token_->value<<std::endl;

    int line = cur_token_->line;
    ASTNode* node = new ASTConstInt(line, cur_token_->value);
    cur_token_ = tokens_[pos_++];
    return node;
}

ASTNode* Parser::ParseConstString() {
    std::cout<<"ParseConstString: "<<cur_token_->value<<std::endl;

    int line = cur_token_->line;
    ASTNode* node = new ASTConstString(line, cur_token_->value);
    cur_token_ = tokens_[pos_++];
    return node;
}

ASTNode* Parser::ParseVariable() {
    std::cout<<"ParseVariable: "<<cur_token_->value<<std::endl;

    int line = cur_token_->line;
    ASTNode* node = new ASTVariable(line, cur_token_->value);
    cur_token_ = tokens_[pos_++];
    if (cur_token_->type == static_cast<Token>('(')) {
        ASTNode* var_func = ParseExprCallfunc(node);
        node = var_func;
    }
    return node;
}

ASTNode* Parser::ParseStatDeclare() {
    std::cout<<"ParseStatDeclare: "<<cur_token_->value<<std::endl;

    int line = cur_token_->line;
    std::vector<ASTNode*> var_list;
    do {
        cur_token_ = tokens_[pos_++];   // eat "int" or ','
        ASTNode* var = ParseVariable();
        if (cur_token_->type == static_cast<Token>('=')) {
            var = ParseStatAssign(var);
        }
        var_list.push_back(var);
    } while (cur_token_->type == static_cast<Token>(','));
    ASTNode* node = new ASTStatDeclare(line, var_list);
    return node;
}

ASTNode* Parser::ParseStatAssign(ASTNode* var) {
    std::cout<<"ParseStatAssign: "<<cur_token_->value<<std::endl;

    int line = cur_token_->line;
    cur_token_ = tokens_[pos_++];       // eat '='
    ASTNode* expr = ParseExpression();
    ASTNode* node = new ASTStatAssign(line, var, expr);
    return node;
}

ASTNode* Parser::ParseStatIf() {
    std::cout<<"ParseStatIf: "<<cur_token_->value<<std::endl;

    int line = cur_token_->line;
    cur_token_ = tokens_[pos_++];       // eat "if"
    ASTNode* condition = ParseExpression();
    ASTNode* if_block = nullptr;
    if (cur_token_->type == static_cast<Token>('{')) {
        if_block = ParseBlock();
    } else {
        if_block = ParseStatement();
    }
    ASTNode* else_block = nullptr;
    if (cur_token_->type == Token::KEYWORD_ELSE) {
        cur_token_ = tokens_[pos_++];       // eat "else"
        if (cur_token_->type == static_cast<Token>('{')) {
            else_block = ParseBlock();
        } else {
            else_block = ParseStatement();
        }
    }
    ASTNode* node = new ASTStatIf(line, condition, if_block, else_block);
    return node;
}

ASTNode* Parser::ParseStatWhile() {
    std::cout<<"ParseStatWhile: "<<cur_token_->value<<std::endl;

    return nullptr;
}

ASTNode* Parser::ParseStatDo() {
    std::cout<<"ParseStatDo: "<<cur_token_->value<<std::endl;

    return nullptr;
}

ASTNode* Parser::ParseStatFor() {
    std::cout<<"ParseStatFor: "<<cur_token_->value<<std::endl;

    return nullptr;
}

ASTNode* Parser::ParseStatBreak() {
    std::cout<<"ParseStatBreak: "<<cur_token_->value<<std::endl;

    return nullptr;
}

ASTNode* Parser::ParseExpression() {
    std::cout<<"ParseExpression: "<<cur_token_->value<<std::endl;

    int line = cur_token_->line;
    std::stack<ASTNode*> var_stack;
    std::stack<Token> op_stack;

    bool is_finished = false;
    bool is_last_token_var = false;

    while (!is_finished) {
        switch (cur_token_->type) {
            case Token::IDENTIFIER: {
                var_stack.push(ParseVariable());
                is_last_token_var = true;
                break;
            }
            case Token::CONST_INT: {
                var_stack.push(ParseConstInt());
                is_last_token_var = true;
                break;
            }
            case Token::CONST_STRING: {
                var_stack.push(ParseConstString());
                is_last_token_var = true;
                break;
            }
            case Token::OP_INCREASE: 
            case Token::OP_DECREASE: {
                Token op = cur_token_->type;
                cur_token_ = tokens_[pos_++];
                ASTNode* var = var_stack.top();
                var_stack.pop();
                var_stack.push(ParseExprSingle(var, op));
                is_last_token_var = true;
                break;
            }
            case static_cast<Token>('('): {
                var_stack.push(ParseExprParen());
                is_last_token_var = true;
                break;
            }
            case static_cast<Token>('-'): {
                if (!is_last_token_var) {
                    cur_token_ = tokens_[pos_++];
                    ASTNode* var = nullptr;
                    if (cur_token_->type == Token::CONST_INT) {
                        var = ParseConstInt();
                    } else {
                        var = ParseVariable();
                    }
                    var_stack.push(ParseExprSingle(var, static_cast<Token>('-')));
                    is_last_token_var = true;
                    break;
                }
            }
            case static_cast<Token>('+'): 
            case static_cast<Token>('*'): 
            case static_cast<Token>('/'): 
            case static_cast<Token>('>'): 
            case static_cast<Token>('<'): 
            case Token::OP_GTE: 
            case Token::OP_LTE: 
            case Token::OP_EQU: 
            case Token::OP_NE: {
                if (!op_stack.empty() && 
                    binary_operator_precedence[op_stack.top()] >= binary_operator_precedence[cur_token_->type]) {
                    Token op = op_stack.top();
                    op_stack.pop();
                    ASTNode* r_node = var_stack.top();
                    var_stack.pop();
                    ASTNode* l_node = var_stack.top();
                    var_stack.pop();
                    ASTNode* node = new ASTExpression(line, op, l_node, r_node);
                    var_stack.push(node);
                }
                op_stack.push(cur_token_->type);
                cur_token_ = tokens_[pos_++];
                is_last_token_var = false;
                break;
            }
            default: {
                // ')', ';', ...
                std::cout<<"ParseExpression: Finished"<<std::endl;
                is_finished = true;
                break;
            }
        }
    }

    // empty expression
    if (op_stack.empty()) {
        if (var_stack.empty()) {
            return nullptr;
        } else {
            while (var_stack.size() != 1) {
                delete var_stack.top();
                var_stack.pop();
            }
            return var_stack.top();
        }
    }
    // not empty expression
    while (!op_stack.empty()) {
        Token op = op_stack.top();
        op_stack.pop();
        ASTNode* r_node = var_stack.top();
        var_stack.pop();
        ASTNode* l_node = var_stack.top();
        var_stack.pop();
        ASTNode* node = new ASTExpression(line, op, l_node, r_node);
        var_stack.push(node);
    }
    if (!var_stack.empty()) {
        return var_stack.top();
    } else {
        // ERR(cur_token_->line, "expression is invalid");
        return nullptr;
    }
}

ASTNode* Parser::ParseExprParen() {
    std::cout<<"ParseExprParen: "<<cur_token_->value<<std::endl;

    int line = cur_token_->line;
    cur_token_ = tokens_[pos_++];       // eat '('
    ASTNode* expr = ParseExpression();
    if (cur_token_->type != static_cast<Token>(')')) {
        //
    } else {
        cur_token_ = tokens_[pos_++];
    }
    return expr;
}

ASTNode* Parser::ParseExprSingle(ASTNode* var, Token op) {
    std::cout<<"ParseExprSingle: "<<var->get_value()<<std::endl;

    ASTNode* node = new ASTExprSingle(var->get_line(), var, op);
    return node;
}

ASTNode* Parser::ParseExprComma() {
    std::cout<<"ParseExprComma: "<<cur_token_->value<<std::endl;

    return nullptr;
}

ASTNode* Parser::ParseExprCallfunc(ASTNode* var) {
    std::cout<<"ParseCallFunc: "<<var->get_value()<<std::endl;

    int line = cur_token_->line;
    std::vector<ASTNode*> parameters;
    do {
        cur_token_ = tokens_[pos_++];       // eat '(' or ','
        ASTNode* expr = ParseExpression();
        parameters.push_back(expr);
    } while (cur_token_->type == static_cast<Token>(','));
    if (cur_token_->type != static_cast<Token>(')')) {
        //
    } else {
        cur_token_ = tokens_[pos_++];
    }
    ASTNode* node = new ASTExprCallFunc(line, var, parameters);
    return node;
}