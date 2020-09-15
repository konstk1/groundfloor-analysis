#ifndef PORTFOLIO_HPP
#define PORTFOLIO_HPP

#include <string>
#include <vector>

#include "Loan.hpp"

class Portfolio {
public:
    Portfolio() = default;

    void LoadFromCsv(const std::string &path);
    void GenerateXirrFile(const std::string &path);

    void CalendarYearSplit();
    void AddLoan(const Loan &loan);
    void RemoveLoan(const std::string &id);
private:
    std::vector<Loan> loans{};
};

#endif