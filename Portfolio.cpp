#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>

#include "Loan.hpp"
#include "Portfolio.hpp"

struct AggregateStats {
    int numLoans = 0;
    float totalPurchasedAmount = 0;
    float totalRepaidAmount = 0;

    float ReturnPct() const {
        return (totalRepaidAmount / totalPurchasedAmount - 1) * 100.0;
    };

};

static void UpdateAggregateStats(AggregateStats &stats, const Loan &loan) {
    // only include repaid loans, skip unrepaid
    if (!loan.IsRepaid()) {
        return;
    }

    stats.numLoans += 1;
    stats.totalPurchasedAmount += loan.purchasedAmount;
    stats.totalRepaidAmount += loan.repaidAmount;
}

void Portfolio::LoansByYear() {
    std::map<int, AggregateStats> yearStats;

    std::cout << "Loans by year: " << std::endl;
    for (const auto &loan: mLoans) {
        auto year = loan.purchasedDate.year;
        auto &stats = yearStats[year];   // will create blank stats if doesn't exist
        UpdateAggregateStats(stats, loan);
    }

    for (const auto &it: yearStats) {
        auto year = it.first;
        auto stats = it.second;
        std::cout << year << "\t#: " << std::setw(2) << stats.numLoans << "\t $" << std::setw(5) << stats.totalPurchasedAmount << " ("<< stats.ReturnPct() << "%)" << std::endl;
    }
}

void Portfolio::GenerateXirrFile(const std::string &filePath) {
    std::ofstream fout(filePath);

    if (!fout.is_open()) {
        throw std::runtime_error("Failed to open file (w): " + filePath);
    }

    // write headers
    fout << "id,loan,amount,date" << std::endl;

    // break up loans into transactions
    std::vector<Transaction> trans;

    for (const auto &loan: mLoans) {
        // only count repaid loans
        if (loan.repaidDate.IsValid()) {
            auto ts = loan.GetTransactions();
            trans.insert(trans.end(), ts.begin(), ts.end());
        }
    }

    // sort transactions by date
    std::sort(trans.begin(), trans.end(), [] (const Transaction &t1, const Transaction &t2) {
        return t1.date < t2.date;
    });

    for (const auto &t: trans) {
        fout << t.id << ",\"" << t.loan << "\"," << t.amount << "," << t.date << std::endl;
    }

    fout.close();
}