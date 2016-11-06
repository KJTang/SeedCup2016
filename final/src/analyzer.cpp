#include "analyzer.h"

double Analyzer::analysis(const Product& p1, const Product& p2) {
    double similarity = 0.0;
    double s_id = analysis_describe(p1.id_, p2.id_);
    similarity = s_id;
    return 0;
}

double Analyzer::analysis_describe(const std::vector<std::string>& d1, const std::vector<std::string>& d2) {
    // merge d1
    // if (d1.size() >= 2) {
    //     for (int i = 0; i != d1.size(); ++i) {
    //         double similarity = analysis_string(d1[i], d1[i+1]);
    //     }
    // }
    return 0;
}

double Analyzer::analysis_string(const std::string& str1, const std::string& str2) {
    return 0;
}
