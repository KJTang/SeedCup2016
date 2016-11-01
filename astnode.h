#pragma once

#include <string>
#include <vector>
#include <cassert>

#include "lexer.h"
#include "Environment.h"
#include "Evaluator.h"

/**
 *  ASTNode:
 *      base class of astnode
 */
class ASTNode {
private:
    std::string value_ = "null";
    ASTNode* parent_ = nullptr;
    int line_ = 0;
public:
    ASTNode() {}
    virtual ~ASTNode() {}

    void set_value(const std::string& value) { value_ = value; }
    virtual std::string get_value() { return value_; }

    void set_parent(ASTNode* parent) { parent_ = parent; }
    ASTNode* get_parent() { return parent_; }

    void set_line(int line) { line_ = line; }
    int get_line() { return line_; }

	virtual bool is_variable() { return false; }

	virtual int eval(Environment<int>& env) { return 0; }
};

/**
 *  ASTBlock:
 *      like:
 *          {
 *              // do sth.  
 *          }
 */
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

	virtual int eval(Environment<int>& env);
};

/**
 *  ASTConstInt:
 *      like: 1, 2, 3, ...
 */
class ASTConstInt : public ASTNode {
public:
    ASTConstInt(int line, const std::string& value) {
        set_line(line);
        set_value(value);
    }
    ~ASTConstInt() {}

	virtual bool is_variable() { return true; }
    
	virtual int eval(Environment<int>& env);
};

/**
 *  ASTConstString:
 *      like: "hello world"
 */
class ASTConstString : public ASTNode {
public:
    ASTConstString(int line, const std::string& value) {
        set_line(line);
        set_value(value);
    }
    ~ASTConstString() {}
    
	virtual int eval(Environment<int>& env);
};

/**
 *  ASTVariable:
 *      variable name, like: a, b, func
 */
class ASTVariable : public ASTNode {
public:
    ASTVariable(int line, const std::string& var_name) {
        set_line(line);
        set_value(var_name);
    }
    ~ASTVariable() {}

	virtual bool is_variable() { return true; }

	virtual int eval(Environment<int>& env);
};

/**
 *  ASTStatDeclare:
 *      variable decalration, like: int a, b;
 */
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
    
	virtual int eval(Environment<int>& env);
};

/**
 *  ASTStatAssign:
 *      like: a = 0; b = c + d;
 */
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

	virtual std::string get_value() {
		return var_->get_value();
	}

	virtual int eval(Environment<int>& env);
};

/**
 *  ASTStatIf:
 *      if-else statement, like:
 *          if (1) {
 *              printf("test 1");
 *          } else {
 *              printf("test 2");   
 *          }
 */
class ASTStatIf : public ASTNode {
private:
    ASTNode* condition_;
    ASTNode* if_block_;
    ASTNode* else_block_;
public:
    ASTStatIf(int line, ASTNode* condition, ASTNode* if_block, ASTNode* else_block) : condition_(condition), if_block_(if_block), else_block_(else_block) {
        set_line(line);
        condition_->set_parent(this);
        if (if_block_) {
            if_block_->set_parent(this);
        }
        if (else_block_) {
            else_block_->set_parent(this);
        }
    }
    ~ASTStatIf() {
        delete condition_;
        if (if_block_) {
            delete if_block_;
        }
        if (else_block_) {
            delete else_block_;
        }
    }

	virtual int eval(Environment<int>& env);
};

/**
 *  ASTStatWhile:
 *      while statement, like: 
 *          while (1) {
 *             a++;
 *          }
 */
class ASTStatWhile : public ASTNode {
private:
    ASTNode* condition_;
    ASTNode* block_;
public:
    ASTStatWhile(int line, ASTNode* condition, ASTNode* block) : condition_(condition), block_(block) {
        set_line(line);
        condition_->set_parent(this);
        if (block_) {
            block_->set_parent(this);
        }
    }
    ~ASTStatWhile() {
        delete condition_;
        if (block_) {
            delete block_;
        }
    }
    
	virtual int eval(Environment<int>& env);
};

/**
 *  ASTStatDo:
 *      do-while statement, like: 
 *          do {
 *              a = a * 2;
 *          } while (a < 100);
 */
class ASTStatDo : public ASTNode {
private:
    ASTNode* block_;
    ASTNode* condition_;
public:
    ASTStatDo(int line, ASTNode* block, ASTNode* condition) : block_(block), condition_(condition) {
        set_line(line);
        if (block_) {
            block_->set_parent(this);
        }
        condition_->set_parent(this);
    }
    ~ASTStatDo() {
        if (block_) {
            delete block_;
        }
        delete condition_;
    }

	virtual int eval(Environment<int>& env);
};

/**
 *  ASTStatFor:
 *      for statement, like:
 *          for (int i = 0; i != 5; i++) {
 *             printf("%d", i);
 *          }
 */
class ASTStatFor : public ASTNode {
private:
    ASTNode* init_;
    ASTNode* condition_;
    ASTNode* increase_;
    ASTNode* block_;
public:
    ASTStatFor(int line, ASTNode* init, ASTNode* condition, ASTNode* increase, ASTNode* block) : init_(init), condition_(condition), increase_(increase), block_(block) {
        set_line(line);
        if (init_) {
            init_->set_parent(this);
        }
        if (condition_) {
            condition_->set_parent(this);
        }
        if (increase_) {
            increase_->set_parent(this);
        }
        if (block_) {
            block_->set_parent(this);
        }
    }
    ~ASTStatFor() {
        if (init_) {
            delete init_;
        }
        if (condition_) {
            delete condition_;
        }
        if (increase_) {
            delete increase_;
        }
        if (block_) {
            delete block_;
        }
    }
    
	virtual int eval(Environment<int>& env);
};

/**
 *  ASTStatBreak:
 *      break keyword, used in loops, like: 
 *          while (1) {
 *             break;
 *          }
 */
class ASTStatBreak : public ASTNode {
public:
    ASTStatBreak(int line) {
        set_line(line);
    }
    ~ASTStatBreak() {}

	virtual int eval(Environment<int>& env);
};

/**
 *  ASTExprSingle:
 *      single operator(!, -, ++, --), like:
 *          int a = b++;
 *          while (!quit) {}
 */
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
    
	virtual int eval(Environment<int>& env);
};

/**
 *  ASTExprComma:
 *      comma expression, like:
 *          a = 1, b++;
 */
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
    
	virtual int eval(Environment<int>& env);
};

/**
 *  ASTExprCallFunc:
 *      function call, like: printf("%d", a);
 */
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

	virtual int eval(Environment<int>& env);
};

/**
 *  ASTExpression:
 *      compute expression, like:
 *          1 + 2 / 3 - 4;
 *          a++ +b - c--;
 */
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

	virtual int eval(Environment<int>& env);
};
