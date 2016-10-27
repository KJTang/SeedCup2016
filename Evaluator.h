#pragma once

#include "Environment.h"
#include "astnode.h"

using namespace std;
class ASTNode;

class Evaluator {
public:
	Evaluator(vector<ASTNode*> roots) : asts_(roots) {
		
	}

	bool eval_prog();
	void output();

	static void push_line(int l);
	static vector<int> lines;

private:
	Environment<int> env_;
	vector<ASTNode*> asts_;
};

