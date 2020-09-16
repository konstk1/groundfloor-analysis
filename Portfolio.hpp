#ifndef PORTFOLIO_HPP
#define PORTFOLIO_HPP

#include <string>
#include <vector>

#include "GfParser.hpp"
#include "Loan.hpp"

class Portfolio {
public:
    Portfolio() = default;
    Portfolio(std::vector<Loan> &&loans): mLoans(std::move(loans)) {};
    Portfolio(GfParser parser) {
        int numLoans = parser.LoadFromCsv(mLoans);
        std::cout << "Loaded " << numLoans << " loans (total " << mLoans.size() << ")" << std::endl;
    };

    void GenerateXirrFile(const std::string &filePath);

    void CalendarYearSplit();
    void AddLoan(const Loan &loan);
    void RemoveLoan(const std::string &id);

    void LoansByYear();
private:
    std::vector<Loan> mLoans{};
};

#endif