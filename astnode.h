#pragma once

/**
    EBNF:

    const-int = ...
    const-string = ...
    variable = ...

    block = "{", [ statement, { ";", statement } ], "}"

    stat-declare = "int", variable | stat-assign, { ",", variable | stat-assign }
    stat-assign = variable, "=", const-int | expression
    stat-if = "if", "(", expression, ")", statement | block, { "else", statement | block }
    stat-while = "while", "(", expression, ")", statement | block
    stat-do = "do", statement | block, "while", "(", expression, ")"
    stat-for = "for", "(", expression, ";", expression, ";", expression, ")", statement | block
    stat-break = "break"

    expr-single = (variable, "++" | "--") | ("-", variable)
    expr-comma = expression, ",", expression
    expr-callfunc = variable, "(", expression, { ",", expression } ")"
    expression = variable | expr-single | expr-callfunc, { operator, variable | expr-single | expr-callfunc }

    statement = stat-declare | stat-assign | stat-if | stat-while | stat-do | stat-for | stat-break | expression | expr-comma
*/

#include <string>
#include <vector>

#include "lexer.h"

// enum class ASTTYPE {
//     ASTNODE,
//     CONST_INT, 
//     CONST_STRING, 
//     VARIABLE, 
//     BLOCK, 
//     STAT_DECLARE, 
//     STAT_ASSIGN, 
//     STAT_IF, 
//     STAT_WHILE, 
//     STAT_DO, 
//     STAT_FOR, 
//     STAT_BREAK, 
//     EXPR_SINGLE,
//     EXPR_COMMA,
//     EXPR_CALLFUNC, 
//     EXPRESSION, 
// };

class ASTNode {
private:
    std::string value_ = "null";
    ASTNode* parent_ = nullptr;
    int line_ = 0;
public:
    ASTNode() {}
    virtual ~ASTNode() {}

    void set_value(const std::string& value) { value_ = value; }
    std::string get_value() { return value_; }

    void set_parent(ASTNode* parent) { parent_ = parent; }
    ASTNode* get_parent() { return parent_; }

    void set_line(int line) { line_ = line; }
    int get_line() { return line_; }

    // Test
    virtual void Print() {}
};

class ASTBlock : public ASTNode {
private:
    std::vector<ASTNode*> statements_;
public:
    ASTBlock(int line, const std::vector<ASTNode*>& statements) : statements_(statements) {
        set_line(line);
        for (int i = 0; i != statements_.size(); ++i) {
            statements_[i]->set_parent(this);
        }
    }
    ~ASTBlock() {
        for (int i = 0; i != statements_.size(); ++i) {
            delete statements_[i];
        }
    }

    // Test
    virtual void Print() {
        std::cout<<"ASTBlock: \t"<<"{"<<statements_.size()<<"}"<<std::endl;
        for (int i = 0; i != statements_.size(); ++i) {
            statements_[i]->Print();
        }
    }
};

class ASTConstInt : public ASTNode {
public:
    ASTConstInt(int line, const std::string& value) {
        set_line(line);
        set_value(value);
    }
    ~ASTConstInt() {}
    
    // Test
    virtual void Print() {
        std::cout<<"ASTConstInt: \t"<<get_value()<<std::endl;
    }
};

class ASTConstString : public ASTNode {
public:
    ASTConstString(int line, const std::string& value) {
        set_line(line);
        set_value(value);
    }
    ~ASTConstString() {}
    
    // Test
    virtual void Print() {
        std::cout<<"ASTConstString: \t"<<get_value()<<std::endl;
    }
};

class ASTVariable : public ASTNode {
public:
    ASTVariable(int line, const std::string& var_name) {
        set_line(line);
        set_value(var_name);
    }
    ~ASTVariable() {}

    // Test
    virtual void Print() {
        std::cout<<"ASTVariable: \t"<<get_value()<<std::endl;
    }
};

class ASTStatDeclare : public ASTNode {
private:
    std::vector<ASTNode*> var_list_;
public:
    ASTStatDeclare(int line, const std::vector<ASTNode*>& var_list) : var_list_(var_list) {
        set_line(line);
        for (int i = 0; i != var_list_.size(); ++i) {
            var_list_[i]->set_parent(this);
        }
    }
    ~ASTStatDeclare() {
        for (int i = 0; i != var_list_.size(); ++i) {
            delete var_list_[i];
        }
    }
    
    // Test
    virtual void Print() {
        std::cout<<"ASTStatDeclare: \t"<<"int: "<<var_list_.size()<<std::endl;
        for (int i = 0; i != var_list_.size(); ++i) {
            var_list_[i]->Print();
        }
    }
};

class ASTStatAssign : public ASTNode {
private:
    ASTNode* var_;
    ASTNode* expr_;
public:
    ASTStatAssign(int line, ASTNode* var, ASTNode* expr) : var_(var), expr_(expr) {
        set_line(line);
        var_->set_parent(this);
        expr_->set_parent(this);
    }
    ~ASTStatAssign() {
        delete var_;
        delete expr_;
    }

    // Test
    virtual void Print() {
        std::cout<<"ASTStatAssign: \t"<<"="<<std::endl;
        var_->Print();
        expr_->Print();
    }
};

class ASTStatIf : public ASTNode {
private:
    ASTNode* condition_;
    ASTNode* if_block_;
    ASTNode* else_block_;
public:
    ASTStatIf(int line, ASTNode* condition, ASTNode* if_block, ASTNode* else_block) : condition_(condition), if_block_(if_block), else_block_(else_block) {
        set_line(line);
        condition_->set_parent(this);
        if_block_->set_parent(this);
        if (else_block_) {
            else_block_->set_parent(this);
        }
    }
    ~ASTStatIf() {
        delete condition_;
        delete if_block_;
        if (else_block_) {
            delete else_block_;
        }
    }

    // Test
    virtual void Print() {
        std::cout<<"ASTStatIf: \t"<<"if"<<std::endl;
        condition_->Print();
        if_block_->Print();
        std::cout<<"ASTStatIf: \t"<<"else"<<std::endl;
        if (else_block_) {
            else_block_->Print();
        }
    }  
};

class ASTStatWhile : public ASTNode {
private:
    ASTNode* condition_;
    ASTNode* block_;
public:
    ASTStatWhile(int line, ASTNode* condition, ASTNode* block) : condition_(condition), block_(block) {
        set_line(line);
        condition_->set_parent(this);
        block_->set_parent(this);
    }
    ~ASTStatWhile() {
        delete condition_;
        delete block_;
    }
    
    // Test
    virtual void Print() {
        std::cout<<"ASTStatWhile: \t"<<"while"<<std::endl;
        condition_->Print();
        block_->Print();
    }  
};

class ASTStatDo : public ASTNode {
private:
    ASTNode* condition_;
    ASTNode* block_;
public:
    ASTStatDo(int line, ASTNode* condition, ASTNode* block) : condition_(condition), block_(block) {
        set_line(line);
        condition_->set_parent(this);
        block_->set_parent(this);
    }
    ~ASTStatDo() {
        delete condition_;
        delete block_;
    }
    
    // Test
    virtual void Print() {
        std::cout<<"ASTStatDo: \t"<<"dowhile"<<std::endl;
        block_->Print();
        condition_->Print();
    }  
};

class ASTStatFor : public ASTNode {
private:
    ASTNode* init_;
    ASTNode* condition_;
    ASTNode* increase_;
    ASTNode* block_;
public:
    ASTStatFor(int line, ASTNode* condition, ASTNode* block) : condition_(condition), block_(block) {
        set_line(line);
        init_->set_parent(this);
        condition_->set_parent(this);
        increase_->set_parent(this);
        block_->set_parent(this);
    }
    ~ASTStatFor() {
        delete init_;
        delete condition_;
        delete increase_;
        delete block_;
    }
    
    // Test
    virtual void Print() {
        std::cout<<"ASTStatFor: \t"<<"for"<<std::endl;
        init_->Print();
        condition_->Print();
        increase_->Print();
        block_->Print();
    }  
};

class ASTStatBreak : public ASTNode {
public:
    ASTStatBreak(int line) {
        set_line(line);
    }
    ~ASTStatBreak() {}
    
    // Test
    virtual void Print() {
        std::cout<<"ASTStatBreak: \t"<<"break"<<std::endl;
    }
};

class ASTExprSingle : public ASTNode {
private:
    ASTNode* var_;
    Token op_;
public:
    ASTExprSingle(int line, ASTNode* var, Token op) : var_(var), op_(op) {
        set_line(line);
        var_->set_parent(this);
    }
    ~ASTExprSingle() {
        delete var_;
    }
    
    // Test
    virtual void Print() {
        std::string str;
        switch (op_) {
            case Token::OP_INCREASE: {
                str = "++";
                break;
            }
            case Token::OP_DECREASE: {
                str = "--";
                break;
            }
            default: {
                str = (char)op_;
            }
        }
        std::cout<<"ASTExprSingle: \t"<<str<<std::endl;
        var_->Print();
    }  
};

class ASTExprComma : public ASTNode {
private:
    ASTNode* expra_;
    ASTNode* exprb_;
public:
    ASTExprComma(int line, ASTNode* expra, ASTNode* exprb) : expra_(expra), exprb_(exprb) {
        set_line(line);
        expra_->set_parent(this);
        exprb_->set_parent(this);
    }
    ~ASTExprComma() {
        delete expra_;
        delete exprb_;
    }
    
    // Test
    virtual void Print() {
        std::cout<<"ASTExprComma: \t"<<std::endl;
        expra_->Print();
        exprb_->Print();
    }  
};

class ASTExprCallFunc : public ASTNode {
private:
    ASTNode* var_;
    std::vector<ASTNode*> parameters_;
public:
    ASTExprCallFunc(int line, ASTNode* var, const std::vector<ASTNode*>& parameters) : var_(var), parameters_(parameters) {
        set_line(line);
        var_->set_parent(this);
        for (int i = 0; i != parameters_.size(); ++i) {
            parameters_[i]->set_parent(this);
        }
    }
    ~ASTExprCallFunc() {
        delete var_;
        for (int i = 0; i != parameters_.size(); ++i) {
            delete parameters_[i];
        }
    }

    // Test
    virtual void Print() {
        std::cout<<"ASTExprCallFunc: \t"<<"("<<parameters_.size()<<")"<<std::endl;
        var_->Print();
        for (int i = 0; i != parameters_.size(); ++i) {
            parameters_[i]->Print();
        }
    }
};

class ASTExpression : public ASTNode {
private:
    Token op_;
    ASTNode* l_node_;
    ASTNode* r_node_;
public:
    ASTExpression(int line, Token op, ASTNode* l_node, ASTNode* r_node) : op_(op), l_node_(l_node), r_node_(r_node) {
        set_line(line);
        l_node_->set_parent(this);
        r_node_->set_parent(this);
    }
    ~ASTExpression() {
        delete l_node_;
        delete r_node_;
    }

    // Test
    virtual void Print() {
        std::string str;
        switch (op_) {
            case Token::OP_GTE: {
                str = ">=";
                break;
            }
            case Token::OP_LTE: {
                str = "<=";
                break;
            }
            case Token::OP_EQU: {
                str = "==";
                break;
            }
            case Token::OP_NE: {
                str = "!=";
                break;
            }
            default: {
                str = (char)op_;
            }
        }
        std::cout<<"ASTExpression: \t"<<str<<std::endl;
        l_node_->Print();
        r_node_->Print();
    }
};
