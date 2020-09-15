#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <utility>

#include "Portfolio.hpp"

typedef std::pair<std::string, int> ColKey;

namespace GfCol {
                                                    //   {col title,       id for quick comparison later}
    static const ColKey ID          {"Transaction Id",        0};
    static const ColKey TITLE       {"Loan",                  5};
    static const ColKey ADDR1       {"Address 1",            10};
    static const ColKey ADDR2       {"Address 2",            15};
    static const ColKey CITY        {"City",                 20};
    static const ColKey STATE       {"State",                25};
    static const ColKey ZIP         {"Zipcode",              30};
    static const ColKey LOAN_AMNT   {"Loan Amount",          35};
    static const ColKey LOAN_STATE  {"Loan State",           40};
    static const ColKey PERF_STATE  {"Performance State",    45};
    static const ColKey RATE        {"Rate",                 50};
    static const ColKey TERM        {"Term",                 55};
    static const ColKey GRADE       {"Grade",                60};
    static const ColKey ARV         {"ARV",                  65};
    static const ColKey ARV_PCT     {"ARV %",                70};
    static const ColKey VAL_TYPE    {"Valuation Type",       75};
    static const ColKey SEC_POS     {"Security Position",    80};
    static const ColKey PURPOSE     {"Purpose",              85};
    static const ColKey EXIT_STRAT  {"Exit Strategy",        90};
    static const ColKey PURCH_AMNT  {"Purchase Amount",      95};
    static const ColKey PROMO_AMNT  {"Promtions Amount",    100};
    static const ColKey PURCH_DATE  {"Purchased Date",      105};
    static const ColKey INT_DATE    {"Interest Start Date", 110};
    static const ColKey MATURE_DATE {"Maturity Date",       115};
    static const ColKey REPAID_DATE {"Repaid Date",         120};
    static const ColKey REPAID_AMNT {"Return",              125};
};

void buildColumnMap(const std::vector<std::string> &header) {
    std::map<int, std::string> columns;

    for (auto i = 0; i < header.size(); ++i) {
        
    }

}

void Portfolio::LoadFromCsv(const std::string &path) {
    std::ifstream fin(path);

    if (!fin.is_open()) {
        throw std::runtime_error("Failed to open file: " + path);
    }

    std::string line, colName;
    std::vector<std::string> header;

    if (fin.good()) {
        std::getline(fin, line);
        std::stringstream ss(line);

        while(std::getline(ss, colName, ',')) {
            header.push_back(colName);
        }

        buildColumnMap(header);
    }

    fin.close();
}
