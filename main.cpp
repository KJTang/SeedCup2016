#include <iostream>
#include <sstream>
#include <fstream>

#include "astnode.h"
#include "lexer.h"
#include "parser.h"
#include "Evaluator.h"

int main_t(int argc, char *args[]) {
    Lexer lexer;
    Parser parser;

	std::ifstream fin("input.txt");
    // compiler input & lexer input
    std::string input_str((std::istreambuf_iterator<char>(fin)), std::istreambuf_iterator<char>());
    // lexer output & parser input
    std::vector<TokenStruct*> tokens;
    // parser output & evaluator input
    std::vector<ASTNode*> astnode_list;

    lexer.Input(input_str);
    lexer.Lex();
    lexer.Output(tokens);

    parser.Input(tokens);
    parser.Parse();
    parser.Output(astnode_list);

	Evaluator evaluator(astnode_list);
	evaluator.eval_prog();
	evaluator.output2file();

    return 0;
}