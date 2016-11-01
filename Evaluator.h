#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include "Environment.h"
#include "astnode.h"

using namespace std;
class ASTNode;

/* Evaluator:
	a virtual machine to evaluates the abstract syntax tree
*/
class Evaluator {
public:
	Evaluator(vector<ASTNode*> roots) : asts_(roots) {
		
	}

	// Evaluate the input
	bool eval_prog();
	// Output the result string
	std::string output();
	// Output to file
	void output2file();

	// Record the line
	static void push_line(int l);
	static vector<int> lines;

	// break flag
	static bool FLAG_BREAK;
	static void reset_break() { FLAG_BREAK = false; }

private:
	Environment<int> env_;
	vector<ASTNode*> asts_;
};

