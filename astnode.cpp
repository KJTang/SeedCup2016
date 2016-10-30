#include "astnode.h"

bool Evaluator::FLAG_BREAK;

int ASTBlock::eval(Environment<int>& env) {
	env.getin_new_scope();
	for (auto stat : statements_) {
		stat->eval(env);
		if (Evaluator::FLAG_BREAK) {
			break;
		}
	}
	env.getout_a_scope();
	return 1;
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
	for (auto var : var_list_) {
		env.add_var(var->get_value());
		var->eval(env);
	}
	return 1;
}

int ASTStatAssign::eval(Environment<int>& env) {
	Evaluator::push_line(get_line());
	env.update_var(var_->get_value(), expr_->eval(env));
	return 1;
}

int ASTStatIf::eval(Environment<int>& env) {
	Evaluator::push_line(get_line());
	int flag = 1;
	if (condition_->eval(env)) {
		flag = if_block_->eval(env);
	}
	else {
		if (else_block_) {
			flag = else_block_->eval(env);
		}
	}
	return flag;
}

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

int ASTStatFor::eval(Environment<int>& env) {
	Evaluator::push_line(get_line());
	env.getin_new_scope();
	if (init_) {
		init_->eval(env);
	}
	while (condition_ ? condition_->eval(env) : true) {
		Evaluator::push_line(get_line());
		block_->eval(env);
		if (Evaluator::FLAG_BREAK) {//continue or sequence
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

int ASTStatBreak::eval(Environment<int>& env) {
	Evaluator::push_line(get_line());
	Evaluator::FLAG_BREAK = true;
	return 0;
}

int ASTExprSingle::eval(Environment<int>& env) {
	Evaluator::push_line(get_line());
	char first = var_->get_value().at(0);
	int temp;
	if (first <= '9' && first >= '0') {
		temp = stoi(var_->get_value());
	}
	else {
		assert(env.is_var_exist(var_->get_value()));
		temp = env.lookup_var(var_->get_value());
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