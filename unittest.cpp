
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

TEST(CC, t6_1) {
	EXPECT_STREQ(
		cctest("input6-1.txt").c_str(),
		"2 3 4 5 6 7 5 6 7 8 10 11 4 5 6 7 8 10 11 4 5 6 7 8 10 11 12"
	);
}

TEST(CC, t6_2) {
	EXPECT_STREQ(
		cctest("input6-2.txt").c_str(),
		"2 3 4 3 4 3 4 5"
	);
}

TEST(CC, t6_3) {
	EXPECT_STREQ(
		cctest("input6-3.txt").c_str(),
		"1 3 6 7 3 6 7 3 4"
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
		"5 6 7 10 11 13 14 16 25"
	);
}

TEST(CC, t9) {
	EXPECT_STREQ(
		cctest("input9.txt").c_str(),
		"2 3 4 6 7 8 9 10 11 12 13 15 16 17 18"
	);
}

TEST(CC, t10) {
	EXPECT_STREQ(
		cctest("input10.txt").c_str(),
		"1 2 3 4 3 7 8 9 8 9 8"
	);
}

TEST(CC, t11) {
	EXPECT_STREQ(
		cctest("input11.txt").c_str(),
		"1 2 3 4 5 13 14 23 26 27"
	);
}

TEST(CC, t12) {
	EXPECT_STREQ(
		cctest("input12.txt").c_str(),
		"1 2 4 5 6 5 6 5 8 9"
	);
}

TEST(CC, t13) {
	EXPECT_STREQ(
		cctest("input13.txt").c_str(),
		"1 2 3 4 8 9 1 2 3 4 8 9 1 12 13 12 13 12 13 12 13 12 13 12"
	);
}

TEST(CC, t14) {
	EXPECT_STREQ(
		cctest("input14.txt").c_str(),
		"1 2 3 5 6 7 5 6 7 5 6 7 5 6 7 5 10 11 12 10 15 16 15 19 20 19 20 19 20 19"
	);
}

TEST(CC, t15) {
	EXPECT_STREQ(
		cctest("input15.txt").c_str(),
		"1 2 4 5 8 9"
	);
}

TEST(CC, tX1) {
	EXPECT_STREQ(
		cctest("inputX1.txt").c_str(),
		"1 2 3 7 8 9"
	);
}

TEST(DY, control1) {
	EXPECT_STREQ(
		cctest("input/control_1.txt").c_str(),
		"1 3 6"
	);
}

TEST(DY, control2) {
	EXPECT_STREQ(
		cctest("input/control_2.txt").c_str(),
		"2 4 5 6 7"
	);
}

TEST(DY, control4) {
	EXPECT_STREQ(
		cctest("input/control_4.txt").c_str(),
		"1 3 4 5"
	);
}

TEST(DY, test0) {
	EXPECT_STREQ(
		cctest("input/dy-debug.c").c_str(),
		"1 5 6 7 8 10 12 13 14 15 16 17 18 20 28 29 30 38 39 40 44"
	);
}

TEST(DY, test3) {
	EXPECT_STREQ(
		cctest("input/dy-test-3.c").c_str(),
		"2 3 4 6 9 10 11 12 13 14 15 16 17 18 20 21 22 23 24 25 26 27 28 30 31 33 35 41"
	);
}

TEST(DY, test4) {
	EXPECT_STREQ(
		cctest("input/dy-test-4.c").c_str(),
		"1 4 5 7 8 11 13 15 16 19 22 23 24 25 31 32 34"
	);
}

TEST(DY, loop1) {
	EXPECT_STREQ(
		cctest("input/loop_1.txt").c_str(),
		"2 3"
	);
}

TEST(DY, loop2) {
	EXPECT_STREQ(
		cctest("input/loop_2.txt").c_str(),
		"1 2 3 2 3 2"
	);
}

TEST(DY, loop3) {
	EXPECT_STREQ(
		cctest("input/loop_3.txt").c_str(),
		"1 4 5 6 7"
	);
}

TEST(DY, loop4) {
	EXPECT_STREQ(
		cctest("input/loop_4.txt").c_str(),
		"3 4 5 6 5 6 5 6 5 3 4 5 6 5 6 5 6 5 3"
	);
}

TEST(DY, test6) {
	EXPECT_STREQ(
		cctest("input/sample6.txt").c_str(),
		"1 2 4"
	);
}

TEST(DY, test7) {
	EXPECT_STREQ(
		cctest("input/sample7.txt").c_str(),
		"1 3 4"
	);
}

TEST(DY, test8) {
	EXPECT_STREQ(
		cctest("input/sample8.txt").c_str(),
		"6 12"
	);
}

TEST(DY, systest1) {
	EXPECT_STREQ(
		cctest("systest_in/calc.c").c_str(),
		"5 7 8 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10 11 10"
	);
}

TEST(DY, fibonacci) {
	EXPECT_STREQ(
		cctest("systest_in/fibonacci.c").c_str(),
		"3 7 8 9 11 12 13 14 9 11 12 13 14 9 11 12 13 14 9 11 12 13 14 9 16"
	);
}

TEST(DY, level) {
	EXPECT_STREQ(
		cctest("systest_in/level.c").c_str(),
		"3 4 5 8 9 11 12 13 17 18 20 23"
	);
}

TEST(DY, loop5) {
	EXPECT_STREQ(
		cctest("systest_in/loop_5.c").c_str(),
		"3 5 7 8 9 10 11 13 7 8 9 10 11 13 7 8 9 18 21 5 7 8 9 18 21 5 7 8 9 18 21 5 7 8 9 18 21 5 7 8 9 18 21 22"
	);
}

TEST(DY, loop6) {
	EXPECT_STREQ(
		cctest("systest_in/loop_6.c").c_str(),
		"3 5 6 7 8 5 6 7 8 5 6 7 8 5 6 7 8 5 6 7 8 5 6 7 8"
	);
}

TEST(DY, print_tri) {
	EXPECT_STREQ(
		cctest("systest_in/print_tri.c").c_str(),
		"3 4 5 13 14 16 17 16 17 16 17 16 17 16 17 16 17 16 19 20 19 22 23 14 16 17 16 17 16 17 16 17 16 17 16 19 20 19 20 19 20 19 22 23 14 16 17 16 17 16 17 16 17 16 19 20 19 20 19 20 19 20 19 20 19 22 23 14 16 17 16 17 16 17 16 19 20 19 20 19 20 19 20 19 20 19 20 19 20 19 22 23 14 16 17 16 17 16 19 20 19 20 19 20 19 20 19 20 19 20 19 20 19 20 19 20 19 22 23 14 16 17 16 19 20 19 20 19 20 19 20 19 20 19 20 19 20 19 20 19 20 19 20 19 20 19 22 23 14 16 19 20 19 20 19 20 19 20 19 20 19 20 19 20 19 20 19 20 19 20 19 20 19 20 19 20 19 22 23 14 25 26 28 29 28 31 32 31 32 31 32 31 32 31 32 31 32 31 32 31 32 31 32 31 32 31 32 31 34 35 26 28 29 28 29 28 31 32 31 32 31 32 31 32 31 32 31 32 31 32 31 32 31 32 31 34 35 26 28 29 28 29 28 29 28 31 32 31 32 31 32 31 32 31 32 31 32 31 32 31 34 35 26 28 29 28 29 28 29 28 29 28 31 32 31 32 31 32 31 32 31 32 31 34 35 26 28 29 28 29 28 29 28 29 28 29 28 31 32 31 32 31 32 31 34 35 26 28 29 28 29 28 29 28 29 28 29 28 29 28 31 32 31 34 35 26"
	);
}

int main_test(int argc, char *args[]) {
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
