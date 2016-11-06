#include "analyzer.h"

#include <algorithm>

const double MERGE_RANGE = 0.9;
const double SAME_STRING_RANGE = 0.75;

int min3(int a, int b, int c) {
    if (a < b) {
        if (a < c) return a;
        else return c;
    }
    if (a > b) {
        if (b < c) return b;
        else return c;
    }
}

double Analyzer::analysis(const Product& p1, const Product& p2) {
    double similarity = 0.0;
    double s_id = analysis_describe(p1.id_, p2.id_);
    double s_brand = analysis_describe(p1.brand_, p2.brand_);
    double s_type = analysis_describe(p1.type_, p2.type_);
    double s_ext = analysis_describe(p1.ext_, p2.ext_);
    double s_title = analysis_describe(p1.title_, p2.title_);
    similarity = s_id * ratio_[0] + s_brand * ratio_[1] + s_type * ratio_[2] + s_ext * ratio_[3] + s_title * ratio_[4];
    return similarity;
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
    double similarity = 0.0;
    // if (merged_d1.size() == 0 || merged_d2.size() == 0) {
    //     similarity = 0.0;
    // } else 
    if (merged_d1[0] == "null" || merged_d2[0] == "null") {
        similarity = 0.0;
    } else {
        if (merged_d1.size() >= merged_d2.size()) {
            for (int i = 0; i != merged_d1.size(); ++i) {
                double max = 0.0;
                for (int j = 0; j != merged_d2.size(); ++j) {
                    double temp = analysis_string(merged_d1[i], merged_d2[j]);
                    if (temp > max) {
                        max = temp;
                    }
                }
                similarity += max;
                // similarity += max > SAME_STRING_RANGE ? 1.0 : 0.0;
            }
            similarity /= (double)merged_d1.size();
        } else {
            for (int i = 0; i != merged_d2.size(); ++i) {
                double max = 0.0;
                for (int j = 0; j != merged_d1.size(); ++j) {
                    double temp = analysis_string(merged_d2[i], merged_d1[j]);
                    if (temp > max) {
                        max = temp;
                    }
                }
                similarity += max;
                // similarity += max > SAME_STRING_RANGE ? 1.0 : 0.0;
            }
            similarity /= (double)merged_d2.size();
        }
    }
    // std::cout<<"analysis_describe: "<<similarity<<std::endl;
    return similarity;
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
