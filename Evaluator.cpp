#include "Evaluator.h"

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

void Evaluator::output() {
	for (auto i : lines) {
		std::cout << i << " ";
	}
}