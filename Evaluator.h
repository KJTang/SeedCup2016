#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include "Environment.h"
#include "astnode.h"

using namespace std;
class ASTNode;

class Evaluator {
public:
	Evaluator(vector<ASTNode*> roots) : asts_(roots) {
		
	}

	bool eval_prog();
	std::string output();
	void output2file();

	static void push_line(int l);
	static vector<int> lines;

	static bool FLAG_BREAK;
	static void reset_break() { FLAG_BREAK = false; }

private:
	Environment<int> env_;
	vector<ASTNode*> asts_;
};

