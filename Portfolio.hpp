#ifndef PORTFOLIO_HPP
#define PORTFOLIO_HPP

#include <string>
#include <vector>
#include <map>


#include "GfParser.hpp"
#include "Loan.hpp"

struct AggregateStats {
    int numRepaidLoans = 0;
    int numUnrepaidLoans = 0;
    float totalPurchasedAmount = 0;
    float totalRepaidAmount = 0;
    int totalDurationDays = 0;

    float returnPctAnnualized = 0.0;

    float ReturnPct() const {
        return (totalRepaidAmount / totalPurchasedAmount - 1) * 100.0;
    };

    float DurationAvg() const {
        return static_cast<float>(totalDurationDays) / numRepaidLoans;
    }
};

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
    void LoansByGrade();
    void LoansByPerfState();

private:
    std::vector<Loan> mLoans{};

    template <typename T>
    void PrintStats(std::map<T, AggregateStats> &aggregateStats);

};

#endif