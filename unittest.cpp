
#include <iostream>
#include <sstream>
#include <fstream>

#include "astnode.h"
#include "lexer.h"
#include "parser.h"
#include "Evaluator.h"
#include <gtest/gtest.h>

std::string cctest(std::string filename);

TEST(test, t1) {
	EXPECT_EQ(1, 1);
}

TEST(CC, t1) {
	std::string a = cctest("input1.txt");
	std::string b = "5 6 7 9 10 12 13 15";
	EXPECT_STREQ(a.c_str(), b.c_str());
}

TEST(CC, t2) {
	EXPECT_STREQ(
		cctest("input2.txt").c_str(),
		"3 4 5 6 7 10 14 16"
	);
}

TEST(CC, t3) {
	EXPECT_STREQ(
		cctest("input3.txt").c_str(),
		"1 2 3 4 5 6 7 5 6 7 5 6 7 5 3 4 5 3"
	);
}

TEST(CC, t4) {
	EXPECT_STREQ(
		cctest("input4.txt").c_str(),
		"1 2 3 4 3 4 3 4 3 4 3 4 3"
	);
}

TEST(CC, t5) {
	EXPECT_STREQ(
		cctest("input5.txt").c_str(),
		"2 3 4 6 7 6 7 6 7"
	);
}

TEST(CC, t6) {
	EXPECT_STREQ(
		cctest("input6.txt").c_str(),
		"2 3 6 7 6 7 8 10 11 6 7 8 10 11 6 7 8 10 11 12"
	);
}

TEST(CC, t7) {
	EXPECT_STREQ(
		cctest("input7.txt").c_str(),
		"1 2 3 4 3 4 3"
	);
}

TEST(CC, t8) {
	EXPECT_STREQ(
		cctest("input8.txt").c_str(),
		"5 6 7 9 10 12 13 15"
	);
}

//TEST(CC, t9) {
//	EXPECT_STREQ(
//		cctest("input9.txt").c_str(),
//		"2 3 4 6 7 8 9 10 11 12 13 15 16 17 18"
//	);
//}

TEST(CC, t10) {
	EXPECT_STREQ(
		cctest("input10.txt").c_str(),
		"1 2"
	);
}

int main(int argc, char *args[]) {
	::testing::InitGoogleTest(&argc, args);
	RUN_ALL_TESTS();

	system("pause");
	return 0;
}

std::string cctest(std::string filename) {
    Lexer lexer;
    Parser parser;

	std::ifstream fin(filename);
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
	return evaluator.output();
}
