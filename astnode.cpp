#include "astnode.h"

// The flag for break from loop
bool Evaluator::FLAG_BREAK;

// Block-eval evluates every statements in a new scope and check if break
int ASTBlock::eval(Environment<int>& env) {
	env.getin_new_scope();
	for (auto stat : statements_) {
		if (stat) {
			stat->eval(env);
		}
		if (Evaluator::FLAG_BREAK) {
			break;
		}
	}
	env.getout_a_scope();
	return 1;
}

// ConstInt-eval switch its value to int
int ASTConstInt::eval(Environment<int>& env) {
	return std::stoi(get_value());
}

// ConstString-eval doesn't implementation
int ASTConstString::eval(Environment<int>& env) {
	return 0;
}

// Variable-eval looks up variable in environment and return it
int ASTVariable::eval(Environment<int>& env) {
	if (env.is_var_exist(get_value())) {
		return env.lookup_var(get_value());
	}
	return 0;
}

// Declare-eval add a new variable in environment and call var's evaluate
int ASTStatDeclare::eval(Environment<int>& env) {
	for (auto var : var_list_) {
		env.add_var(var->get_value());
		var->eval(env);
	}
	return 1;
}

// Assign-eval calculates right value and updates the variable
int ASTStatAssign::eval(Environment<int>& env) {
	Evaluator::push_line(get_line());
	int rhs = expr_->eval(env);
	env.update_var(var_->get_value(), rhs);
	return rhs;
}

// If-eval checks condition then evaluates body
int ASTStatIf::eval(Environment<int>& env) {
	Evaluator::push_line(get_line());
	int flag = 1;
	if (condition_->eval(env)) {
		if (if_block_) {
			flag = if_block_->eval(env);
		}
	}
	else {
		if (else_block_) {
			flag = else_block_->eval(env);
		}
	}
	return flag;
}

// While-eval checks condition then evaluates block in while-loop and checks break
int ASTStatWhile::eval(Environment<int>& env) {
	Evaluator::push_line(get_line());
	while (condition_->eval(env)) {
		Evaluator::push_line(get_line());
		block_->eval(env);
		if (Evaluator::FLAG_BREAK) {
			Evaluator::reset_break();
			break;
		}
	}
	return 1;
}

// Dowhile-eval checks condition then evaluates block in dowhile-loop and checks break
int ASTStatDo::eval(Environment<int>& env) {
	do {
		block_->eval(env);
		if (Evaluator::FLAG_BREAK) {
			Evaluator::reset_break();
			break;
		}
	} while (Evaluator::push_line(condition_->get_line()), condition_->eval(env));
	return 1;
}

// For-eval create a new scope and checks init, condition, increase 
// then evaluates blocks in while-loop and check break
int ASTStatFor::eval(Environment<int>& env) {
	Evaluator::push_line(get_line());
	env.getin_new_scope();
	if (init_) {
		init_->eval(env);
	}
	while (condition_ ? condition_->eval(env) : true) {
		Evaluator::push_line(get_line());
		block_->eval(env);
		if (Evaluator::FLAG_BREAK) {
			Evaluator::reset_break();
			break;
		}
		else {
			if (increase_) {
				increase_->eval(env);
			}
		}
	}
	env.getout_a_scope();
	return 1;
}

// Break-eval change the flag-break to true
int ASTStatBreak::eval(Environment<int>& env) {
	Evaluator::push_line(get_line());
	Evaluator::FLAG_BREAK = true;
	return 0;
}

int ASTExprSingle::eval(Environment<int>& env) {
	Evaluator::push_line(get_line());
	int temp;
	
	// Check if var is a Variable-Node
	if (var_->is_variable()) {
		// Check if var is a number or variable
		char first = var_->get_value().at(0);
		if (first <= '9' && first >= '0') {
			temp = stoi(var_->get_value());
		}
		else {
			assert(env.is_var_exist(var_->get_value()));
			temp = env.lookup_var(var_->get_value());
		}
	}
	else {
		temp = var_->eval(env);
	}
	switch (op_)
	{
	case Token::OP_INCREASE: {
		env.update_var(var_->get_value(), temp + 1);
		break;
	}
	case Token::OP_DECREASE: {
		env.update_var(var_->get_value(), temp - 1);
		break;
	}
	case static_cast<Token>('-'): {
		temp = -temp;
		break;
	}
	case static_cast<Token>('!') : {
		temp = !temp;
	}
	default:
		break;
	}
	return temp;
}

// CommaExpr-eval evaluates every expression in recursion and return the last
int ASTExprComma::eval(Environment<int>& env) {
	Evaluator::push_line(get_line());
	expra_->eval(env);
	return exprb_->eval(env);
}

// CallFunc-eval evaluates the every parameter
int ASTExprCallFunc::eval(Environment<int>& env) {
	Evaluator::push_line(get_line());
	for (auto expr : parameters_) {
		expr->eval(env);
	}
	return 1;
}

int ASTExpression::eval(Environment<int>& env) {
	Evaluator::push_line(get_line());
	int lhs = l_node_->eval(env);
	int rhs = r_node_->eval(env);
	switch (op_)
	{
	case Token::OP_GTE: {
		return lhs >= rhs;
	}
	case Token::OP_LTE: {
		return lhs <= rhs;
	}
	case Token::OP_EQU: {
		return lhs == rhs;
	}
	case Token::OP_NE: {
		return lhs != rhs;
	}
	case static_cast<Token>('+') : {
		return lhs + rhs;
	}
	case static_cast<Token>('-') : {
		return lhs - rhs;
	}
	case static_cast<Token>('*') : {
		return lhs * rhs;
	}
	case static_cast<Token>('/') : {
		return lhs / rhs;
	}
	case static_cast<Token>('<') : {
		return lhs < rhs;
	}
	case static_cast<Token>('>') : {
		return lhs > rhs;
	}
	case Token::OP_AND: {
		return lhs && rhs;
	}
	case Token::OP_OR: {
		return lhs || rhs;
	}
	default:
		return 0;
	}
}