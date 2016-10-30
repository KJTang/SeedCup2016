#include "parser.h"

#include <iostream>
#include <stack>

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
            return nullptr;
        }
    }
}

ASTNode* Parser::ParseBlock() {
    int line = cur_token_->line;
    cur_token_ = tokens_[pos_++];             // eat '{'
    std::vector<ASTNode*> statements;

    if (static_cast<int>(cur_token_->type) != '}') {
        do {
            ASTNode* stat = ParseStatement();
            if (stat) {
                statements.push_back(stat);
            }
        } while (cur_token_->type != static_cast<Token>('}'));
    }
    cur_token_ = tokens_[pos_++];             // eat '}'
    return new ASTBlock(line, statements);
}

ASTNode* Parser::ParseConstInt() {
    int line = cur_token_->line;
    std::string value = cur_token_->value;
    cur_token_ = tokens_[pos_++];
    return new ASTConstInt(line, value);
}

ASTNode* Parser::ParseConstString() {
    int line = cur_token_->line;
    cur_token_ = tokens_[pos_++];
    return new ASTConstString(line, cur_token_->value);
}

ASTNode* Parser::ParseVariable() {
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
    int line = cur_token_->line;
    std::vector<ASTNode*> var_list;
    do {
        cur_token_ = tokens_[pos_++];            // eat "int" or ','
        ASTNode* var = ParseVariable();
        if (cur_token_->type == static_cast<Token>('=')) {
            var = ParseStatAssign(var);
        }
        var_list.push_back(var);
    } while (cur_token_->type == static_cast<Token>(','));
    return new ASTStatDeclare(line, var_list);
}

ASTNode* Parser::ParseStatAssign(ASTNode* var) {
    int line = cur_token_->line;
    cur_token_ = tokens_[pos_++];               // eat '='
    ASTNode* expr = ParseExpression();
    return new ASTStatAssign(line, var, expr);
}

ASTNode* Parser::ParseStatIf() {
    int line = cur_token_->line;
    cur_token_ = tokens_[pos_++];               // eat "if"

    std::stack<ASTNode*> condition_stack;
    do {
        cur_token_ = tokens_[pos_++];       // eat '(' or ','
        condition_stack.push(ParseStatement());
    } while (cur_token_->type != static_cast<Token>(')'));
    while (condition_stack.size() >= 2) {
        ASTNode* r_node = condition_stack.top();
        condition_stack.pop();
        ASTNode* l_node = condition_stack.top();
        condition_stack.pop();
        condition_stack.push(ParseExprComma(l_node, r_node));
    }
    ASTNode* condition = condition_stack.top();
    cur_token_ = tokens_[pos_++];           // eat ')'

    ASTNode* if_block = nullptr;
    if (cur_token_->type == static_cast<Token>('{')) {
        if_block = ParseBlock();
    } else {
        if_block = ParseStatement();
        cur_token_ = tokens_[pos_++];           // eat ';'
    }
    ASTNode* else_block = nullptr;
    if (cur_token_->type == Token::KEYWORD_ELSE) {
        cur_token_ = tokens_[pos_++];           // eat "else"
        if (cur_token_->type == static_cast<Token>('{')) {
            else_block = ParseBlock();
        } else {
            else_block = ParseStatement();
        }
    }
    return new ASTStatIf(line, condition, if_block, else_block);
}

ASTNode* Parser::ParseStatWhile() {
    int line = cur_token_->line;
    cur_token_ = tokens_[pos_++];           // eat "while"
    std::stack<ASTNode*> condition_stack;
    do {
        cur_token_ = tokens_[pos_++];       // eat '(' or ','
        condition_stack.push(ParseStatement());
    } while (cur_token_->type != static_cast<Token>(')'));
    while (condition_stack.size() >= 2) {
        ASTNode* r_node = condition_stack.top();
        condition_stack.pop();
        ASTNode* l_node = condition_stack.top();
        condition_stack.pop();
        condition_stack.push(ParseExprComma(l_node, r_node));
    }
    ASTNode* condition = condition_stack.top();
    cur_token_ = tokens_[pos_++];           // eat ')'

    ASTNode* block = nullptr;
    if (cur_token_->type == static_cast<Token>('{')) {
        block = ParseBlock();
    } else {
        block = ParseStatement();
    }
    return new ASTStatWhile(line, condition, block);
}

ASTNode* Parser::ParseStatDo() {
    int line = cur_token_->line;
    cur_token_ = tokens_[pos_++];           // eat "do"
    ASTNode* block = nullptr;
    if (cur_token_->type == static_cast<Token>('{')) {
        block = ParseBlock();
    } else {
        block = ParseStatement();
    }
    cur_token_ = tokens_[pos_++];           // eat "while"
    std::stack<ASTNode*> condition_stack;
    do {
        cur_token_ = tokens_[pos_++];       // eat '(' or ','
        condition_stack.push(ParseStatement());
    } while (cur_token_->type != static_cast<Token>(')'));
    while (condition_stack.size() >= 2) {
        ASTNode* r_node = condition_stack.top();
        condition_stack.pop();
        ASTNode* l_node = condition_stack.top();
        condition_stack.pop();
        condition_stack.push(ParseExprComma(l_node, r_node));
    }
    ASTNode* condition = condition_stack.top();
    cur_token_ = tokens_[pos_++];           // eat ')'
    return new ASTStatDo(line, block, condition);
}

ASTNode* Parser::ParseStatFor() {
    int line = cur_token_->line;
    cur_token_ = tokens_[pos_++];           // eat "for"

    std::stack<ASTNode*> init_stack;
    std::stack<ASTNode*> condition_stack;
    std::stack<ASTNode*> increase_stack;
    // init
    if (tokens_[pos_]->type != static_cast<Token>(';')) {
        do {
            cur_token_ = tokens_[pos_++];       // eat '(' or ','
            init_stack.push(ParseStatement());
        } while (cur_token_->type != static_cast<Token>(';'));
        while (init_stack.size() >= 2) {
            ASTNode* r_node = init_stack.top();
            init_stack.pop();
            ASTNode* l_node = init_stack.top();
            init_stack.pop();
            init_stack.push(ParseExprComma(l_node, r_node));
        }
    } else {
        cur_token_ = tokens_[pos_++];
    }
    ASTNode* init = init_stack.empty() ? nullptr : init_stack.top();
    // condition
    if (tokens_[pos_]->type != static_cast<Token>(';')) {
        do {
            cur_token_ = tokens_[pos_++];       // eat ';' or ','
            condition_stack.push(ParseStatement());
        } while (cur_token_->type != static_cast<Token>(';'));
        while (condition_stack.size() >= 2) {
            ASTNode* r_node = condition_stack.top();
            condition_stack.pop();
            ASTNode* l_node = condition_stack.top();
            condition_stack.pop();
            condition_stack.push(ParseExprComma(l_node, r_node));
        }
    } else {
        cur_token_ = tokens_[pos_++];
    }
    ASTNode* condition = condition_stack.empty() ? nullptr : condition_stack.top();
    // increase    
    if (tokens_[pos_]->type != static_cast<Token>(')')) {
        do {
            cur_token_ = tokens_[pos_++];       // eat ';' or ','
            increase_stack.push(ParseStatement());
        } while (cur_token_->type != static_cast<Token>(')'));
        while (increase_stack.size() >= 2) {
            ASTNode* r_node = increase_stack.top();
            increase_stack.pop();
            ASTNode* l_node = increase_stack.top();
            increase_stack.pop();
            increase_stack.push(ParseExprComma(l_node, r_node));
        }
    } else {
        cur_token_ = tokens_[pos_++];           // eat ';'
    }
    ASTNode* increase = increase_stack.empty() ? nullptr : increase_stack.top();
    // block
    cur_token_ = tokens_[pos_++];               // eat ')'
    ASTNode* block = nullptr;
    if (cur_token_->type == static_cast<Token>('{')) {
        block = ParseBlock();
    } else {
        block = ParseStatement();
    }

    return new ASTStatFor(line, init, condition, increase, block);
}

ASTNode* Parser::ParseStatBreak() {
    int line = cur_token_->line;
    cur_token_ = tokens_[pos_++];
    return new ASTStatBreak(line);
}

ASTNode* Parser::ParseExpression() {
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
            case static_cast<Token>('!'): {
                cur_token_ = tokens_[pos_++];
                ASTNode* var = nullptr;
                if (cur_token_->type == Token::CONST_INT) {
                    var = ParseConstInt();
                } else {
                    var = ParseVariable();
                }
                var_stack.push(ParseExprSingle(var, static_cast<Token>('!')));
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
    return new ASTExprSingle(var->get_line(), var, op);
}

ASTNode* Parser::ParseExprComma(ASTNode* l, ASTNode* r) {
    return new ASTExprComma(l->get_line(), l, r);
}

ASTNode* Parser::ParseExprCallfunc(ASTNode* var) {
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
    return new ASTExprCallFunc(line, var, parameters);
}