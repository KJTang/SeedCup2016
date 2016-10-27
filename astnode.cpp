#include "astnode.h"

int ASTBlock::eval(Environment<int>& env) {
	int flag = 1;
	env.getin_new_scope();
	for (auto stat : statements_) {
		if (!stat->eval(env)) {
			flag = 0;
			break;
		}
	}
	env.getout_a_scope();
	return flag;
}

int ASTConstInt::eval(Environment<int>& env) {
	return std::stoi(get_value());
}

int ASTConstString::eval(Environment<int>& env) {
	return 0;
}

int ASTVariable::eval(Environment<int>& env) {
	if (env.is_var_exist(get_value())) {
		return env.lookup_var(get_value());
	}
	return 0;
}

int ASTStatDeclare::eval(Environment<int>& env) {
	Evaluator::push_line(get_line());
	for (auto var : var_list_) {
		var->eval(env);
	}
	return 1;
}

int ASTStatAssign::eval(Environment<int>& env) {
	Evaluator::push_line(get_line());
	env.add_var(var_->get_value(), expr_->eval(env));
	return 1;
}

int ASTStatIf::eval(Environment<int>& env) {
	Evaluator::push_line(get_line());
	int flag = 1;
	if (condition_->eval(env)) {
		flag = if_block_->eval(env);
	}
	else {
		flag = else_block_->eval(env);
	}
	return flag;
}

int ASTStatWhile::eval(Environment<int>& env) {
	Evaluator::push_line(get_line());
	while (condition_->eval(env)) {
		if (!block_->eval(env)) {
			break;
		}
	}
	return 1;
}

int ASTStatDo::eval(Environment<int>& env) {
	Evaluator::push_line(get_line());
	do {
		if (!block_->eval(env)) {
			break;
		}
	} while (condition_->eval(env));
	return 1;
}

int ASTStatFor::eval(Environment<int>& env) {
	Evaluator::push_line(get_line());
	env.getin_new_scope();
	if (init_) {
		init_->eval(env);
	}
	while (condition_ ? condition_->eval(env) : true) {
		if (block_->eval(env)) {//continue or sequence
			if (increase_) {
				increase_->eval(env);
			}
		}
		else {
			break;
		}
	}
	env.getout_a_scope();
	return 1;
}

int ASTStatBreak::eval(Environment<int>& env) {
	Evaluator::push_line(get_line());
	return 0;
}

int ASTExprSingle::eval(Environment<int>& env) {
	Evaluator::push_line(get_line());
	assert(env.is_var_exist(var_->get_value()));
	int temp = env.lookup_var(var_->get_value());
	switch (op_)
	{
	case Token::OP_INCREASE: {
		temp += 1;
		env.add_var(var_->get_value(), temp);
		break;
	}
	case Token::OP_DECREASE: {
		temp -= 1;
		env.add_var(var_->get_value(), temp);
		break;
	}
	case static_cast<Token>('-'): {
		temp = -temp;
		env.add_var(var_->get_value(), temp);
		break;
	}
	default:
		break;
	}
	return temp;
}

int ASTExprComma::eval(Environment<int>& env) {
	Evaluator::push_line(get_line());
	expra_->eval(env);
	return exprb_->eval(env);
}

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