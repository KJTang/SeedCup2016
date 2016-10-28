﻿#include "Evaluator.h"

vector<int> Evaluator::lines;

bool Evaluator::eval_prog() {
	for (auto ast : asts_) {
		ast->eval(env_);
	}
	return true;
}

void Evaluator::push_line(int l) {
	if (lines.empty() || *(lines.end() - 1) != l) {
		lines.push_back(l);
	}
}

std::string Evaluator::output() {
	std::stringstream ss;
	for (auto i : lines) {
		ss << i << " ";
	}
	std::string str = ss.str().substr(0, ss.str().length() - 1);
	return str;
}

void Evaluator::output2file() {
	ofstream file;
	file.open("output.txt");
	file << output();
	file.close();
}