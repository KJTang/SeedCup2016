#include "analyzer.h"

const double MERGE_RANGE = 0.9;

int min3(int a, int b, int c){
    if(a<b){
        if(a<c) return a;
        else return c;
    }
    if(a>b){
        if(b<c) return b;
        else return c;
    }
}

double Analyzer::analysis(const Product& p1, const Product& p2) {
    // test
    std::vector<std::string> t1, t2;
    t1.push_back("hello");
    t1.push_back("world");
    t1.push_back("aha");
    t1.push_back("cpp");
    t2 = t1;
    analysis_describe(t1, t2);

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

double Analyzer::analysis_string(const std::string& str1, const std::string& str2) {
    int i, j;
    int k1 = str1.size();
    int k2 = str2.size();
    int Vec[k1+1][k2+1];
    for(i=0; i<=k1; i++){
        Vec[i][0] = i;
    }
    for(j=0; j<=k2; j++){
        Vec[0][j] = j;
    }

    for(j=1; j<=k2; j++){
        for(i=1; i<=k1; i++){
            if(str1[i-1] == str2[j-1]){
                Vec[i][j] = min3(Vec[i-1][j-1], Vec[i-1][j]+1, Vec[i][j-1]+1);
            }
            else{
                Vec[i][j] = min3(Vec[i-1][j-1], Vec[i-1][j], Vec[i][j-1])+1;
            }
        }
    }

    return Vec[k1][k2];
}
