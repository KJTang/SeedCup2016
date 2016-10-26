#ifndef __EVALUATOR_H__
#define __EVALUATOR_H__

#include <vector>
#include <unordered_map>
#include "astnode.h"

using namespace std;

class Evaluator {
public:
	Evaluator(ASTNode* root);

private:
	Environment<int> env_;
};

template<typename T>
class Environment {
public:
	Environment() : cur_scope(0);

	bool is_var_exist(string var_name);
	T lookup_var(string var_name);
	bool add_var(string var_name, T value);

	bool getin_new_scope();
	bool getout_a_scope();
private:
	vector<unordered_map<string, T>> scopes_;
	bool is_var_exist(string var_name, int scope_layer);
	T lookup_var(string var_name, int scope_layer);
	int cur_scope_;
};

#endif // !__EVALUATOR_H__
