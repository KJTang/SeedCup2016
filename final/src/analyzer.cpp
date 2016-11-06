#include "analyzer.h"

#include <algorithm>

const double MERGE_RANGE = 0.9;

int min3(int a, int b, int c){
    if (a < b) {
        if (a < c) return a;
        else return c;
    }
    if(a > b) {
        if (b < c) return b;
        else return c;
    }
}

double Analyzer::analysis(const Product& p1, const Product& p2) {
    std::cout<<analysis_string("ashello", "hello")<<std::endl;
    std::cout<<analysis_string("壳氏唯", "壳氏唯 ")<<std::endl;
    std::cout<<analysis_string("HUSKS WARE", "Husk’Sware")<<std::endl;

    // // test
    // std::vector<std::string> t1, t2;
    // t1.push_back("hello");
    // t1.push_back("world");
    // t1.push_back("aha");
    // t1.push_back("cpp");
    // t2 = t1;
    // analysis_describe(t1, t2);

    // double similarity = 0.0;
    // double s_id = analysis_describe(p1.id_, p2.id_);
    // similarity = s_id;
    return 0;
}

double Analyzer::analysis_describe(const std::vector<std::string>& d1, const std::vector<std::string>& d2) {
    std::vector<std::string> merged_d1 = d1, merged_d2 = d2;
    // merge d1
    for (int i = 0; i < merged_d1.size(); ++i) {
        for (int j = i + 1; j < merged_d1.size(); ) {
            if (analysis_string(merged_d1[i], merged_d1[j]) >= MERGE_RANGE) {
                merged_d1.erase(merged_d1.begin()+j);
            } else {
                ++j;
            }
        }
    }
    // merge d2
    for (int i = 0; i < merged_d2.size(); ++i) {
        for (int j = i + 1; j < merged_d2.size(); ) {
            if (analysis_string(merged_d2[i], merged_d2[j]) >= MERGE_RANGE) {
                merged_d2.erase(merged_d2.begin()+j);
            } else {
                ++j;
            }
        }
    }
    // analysis
    
    return 0;
}

double Analyzer::analysis_string(const std::string& s1, const std::string& s2) {
    std::string str1 = s1;
    std::string str2 = s2;
    int len1 = str1.size();
    int len2 = str2.size();

    // trans to low-case
    for (int i = 0; i != len1; ++i) {
        if (str1[i] >= 'A' && str1[i] <= 'Z') {
            str1[i] = str1[i] + 'a' - 'A';
        }
    }
    for (int i = 0; i != len2; ++i) {
        if (str2[i] >= 'A' && str2[i] <= 'Z') {
            str2[i] = str2[i] + 'a' - 'A';
        }
    }

    // init matrix
    int Vec[len1+1][len2+1];
    for (int i = 0; i <= len1; ++i) {
        Vec[i][0] = i;
    }
    for (int j = 0; j <= len2; ++j) {
        Vec[0][j] = j;
    }

    // compute matrix
    for (int j = 1; j <= len2; ++j) {
        for (int i = 1; i <= len1; ++i) {
            if (str1[i-1] == str2[j-1]){
                Vec[i][j] = min3(Vec[i-1][j-1], Vec[i-1][j]+1, Vec[i][j-1]+1);
            } else {
                Vec[i][j] = min3(Vec[i-1][j-1], Vec[i-1][j], Vec[i][j-1])+1;
            }
        }
    }

    return 1.0 - (double)Vec[len1][len2] / (double)std::max(str1.size(), str2.size());
}
