#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "Loan.hpp"
#include "GfParser.hpp"

// typedef std::pair<std::string, int> ColKey;

// namespace GfCol {
//                                //   {col title,       id for quick comparison later}
//     static const ColKey ID          {"Transaction Id",        0};
//     static const ColKey TITLE       {"Loan",                  5};
//     static const ColKey ADDR1       {"Address 1",            10};
//     static const ColKey ADDR2       {"Address 2",            15};
//     static const ColKey CITY        {"City",                 20};
//     static const ColKey STATE       {"State",                25};
//     static const ColKey ZIP         {"Zipcode",              30};
//     static const ColKey LOAN_AMNT   {"Loan Amount",          35};
//     static const ColKey LOAN_STATE  {"Loan State",           40};
//     static const ColKey PERF_STATE  {"Performance State",    45};
//     static const ColKey RATE        {"Rate",                 50};
//     static const ColKey TERM        {"Term",                 55};
//     static const ColKey GRADE       {"Grade",                60};
//     static const ColKey ARV         {"ARV",                  65};
//     static const ColKey ARV_PCT     {"ARV %",                70};
//     static const ColKey VAL_TYPE    {"Valuation Type",       75};
//     static const ColKey SEC_POS     {"Security Position",    80};
//     static const ColKey PURPOSE     {"Purpose",              85};
//     static const ColKey EXIT_STRAT  {"Exit Strategy",        90};
//     static const ColKey PURCH_AMNT  {"Purchase Amount",      95};
//     static const ColKey PROMO_AMNT  {"Promtions Amount",    100};
//     static const ColKey PURCH_DATE  {"Purchased Date",      105};
//     static const ColKey INT_DATE    {"Interest Start Date", 110};
//     static const ColKey MATURE_DATE {"Maturity Date",       115};
//     static const ColKey REPAID_DATE {"Repaid Date",         120};
//     static const ColKey REPAID_AMNT {"Return",              125}; 
// };

enum GfCol {
    ID = 0,
    TITLE,
    ADDR1,
    ADDR2,
    CITY,
    STATE,
    ZIP,
    LOAN_AMNT,
    LOAN_STATE,
    PERF_STATE,
    RATE,
    TERM,
    GRADE,
    ARV,
    ARV_PCT,
    VAL_TYPE,
    SEC_POS,
    PURPOSE,
    EXIT_STRAT,
    PURCH_AMNT,
    PROMO_AMNT,
    PURCH_DATE,
    INT_DATE,
    MATURE_DATE,
    REPAID_DATE,
    REPAID_AMNT,
};

static Loan::PerfState PerfStateFromString(const std::string &str) {
    if (str.compare("Performing") == 0) {
        return Loan::PerfState::PERFORMING;
    } else if (str.compare("Performed") == 0) {
        return Loan::PerfState::PERFORMED;
    } else if (str.compare("Extended") == 0) {
        return Loan::PerfState::EXTENDED;
    } else if (str.compare("Refunded") == 0) {
        return Loan::PerfState::REFUNDED;
    } else if (str.compare("Default") == 0) {
        return Loan::PerfState::DEFAULT;
    } else {
        throw std::invalid_argument("Invalid Performance State: " + str);
        // return Loan::PerfState::UNKNOWN;
    }
}

static Loan::LoanState LoanStateFromString(const std::string &str) {
    if (str.compare("Funded") == 0) {
        return Loan::LoanState::FUNDED;
    } else if (str.compare("Refunded") == 0) {
        return Loan::LoanState::REFUNDED;
    } else if (str.compare("Repaid") == 0) {
        return Loan::LoanState::REPAID;
    } else {
        throw std::invalid_argument("Invalid Loan State: " + str);
        // return Loan::LoanState::UNKNOWN;
    }
}

static float CurrencyToFloat(std::string str) {
    // remove $ and ,
    str.erase(std::remove(str.begin(), str.end(), '$'), str.end());
    str.erase(std::remove(str.begin(), str.end(), ','), str.end());

    return std::stof(str);
}

Loan GfParser::ProcessLoanRow(const std::vector<std::string> &tokens) {
    Loan loan;

    loan.id = tokens[GfCol::ID];
    loan.title = tokens[GfCol::TITLE];
    loan.loanState = LoanStateFromString(tokens[GfCol::LOAN_STATE]);
    loan.perfState = PerfStateFromString(tokens[GfCol::PERF_STATE]);

    loan.address.street = tokens[GfCol::ADDR1];
    loan.address.city = tokens[GfCol::CITY];
    loan.address.state = tokens[GfCol::STATE];
    loan.address.zipcode = tokens[GfCol::ZIP];

    loan.rate = std::stof(tokens[GfCol::RATE]);
    loan.term = std::stoi(tokens[GfCol::TERM]);
    loan.grade = tokens[GfCol::GRADE];
    loan.arvPct = stof(tokens[GfCol::ARV_PCT]);

    loan.purchasedAmount = CurrencyToFloat(tokens[GfCol::PURCH_AMNT]);

    if (!tokens[GfCol::REPAID_AMNT].empty()) {
        loan.repaidAmount = CurrencyToFloat(tokens[GfCol::REPAID_AMNT]);
    }

    loan.purchasedDate = Date(tokens[GfCol::PURCH_DATE]);
    loan.maturityDate = Date(tokens[GfCol::MATURE_DATE]);

    if (!tokens[GfCol::REPAID_DATE].empty()) {
        loan.repaidDate = Date(tokens[GfCol::REPAID_DATE]);
    }

    return loan;
}

int GfParser::LoadFromCsv(std::vector<Loan> &loans) {
    return LoadFromCsv(mFilePath, loans);
}

int GfParser::LoadFromCsv(const std::string &filePath, std::vector<Loan> &loans) {
    std::ifstream fin(filePath);

    if (!fin.is_open()) {
        throw std::runtime_error("Failed to open file (r): " + filePath);
    }

    std::string line, colName;
    std::vector<std::string> header;

    if (fin.good()) {
        std::getline(fin, line);
        std::stringstream ss(line);

        while(std::getline(ss, colName, ',')) {
            header.push_back(colName);
        }

        // TODO: build header map
    }

    std::vector<std::string> tokens;
    int numLoans = 0;

    while (std::getline(fin, line)) {
        std::stringstream ss(line);
        
        tokens.clear();

        std::string token;

        while (ss.good()) {
            // support for quoted fields (eg: "123,124 Main St",$100): ignore first quote, read until 2nd quote, ignore trailing comma
            if (ss.peek() == '\"') {
                ss.ignore(1);   // ignore first "
                std::getline(ss, token, '\"');
                ss.ignore(1);   // ignore comma after closing "
            } else {
                std::getline(ss, token, ',');
            }
            tokens.push_back(token);
        }

        loans.push_back(ProcessLoanRow(tokens));
        numLoans++;
    }

    fin.close();

    return numLoans;
}