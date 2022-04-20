#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>

#include "Loan.hpp"
#include "Portfolio.hpp"

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

static void UpdateAggregateStats(AggregateStats &stats, const Loan &loan) {
    // only include repaid loans, skip unrepaid
    if (!loan.IsRepaid()) {
        stats.numUnrepaidLoans += 1;
        return;
    }

    stats.returnPctAnnualized = (stats.totalPurchasedAmount * stats.returnPctAnnualized + loan.purchasedAmount * loan.ReturnPctAnnualized()) / (stats.totalPurchasedAmount + loan.purchasedAmount);

    stats.numRepaidLoans += 1;
    stats.totalPurchasedAmount += loan.purchasedAmount;
    stats.totalRepaidAmount += loan.repaidAmount;
    stats.totalDurationDays += loan.DurationDays();
}

void Portfolio::LoansByYear() {
    std::map<int, AggregateStats> yearStats;

    std::cout << "Loans by purchase year (repaid/unrepaid): " << std::endl;
    for (const auto &loan: mLoans) {
        auto year = loan.purchasedDate.year;
        auto &stats = yearStats[year];   // will create blank stats if doesn't exist
        UpdateAggregateStats(stats, loan);
    }

    for (const auto &it: yearStats) {
        auto year = it.first;
        auto stats = it.second;
        std::cout << std::fixed << std::setprecision(2)
                  << year << "\t#: " << std::setw(3) << stats.numRepaidLoans << " / " << std::setw(3) << stats.numUnrepaidLoans << "\t $" << std::setw(5) << static_cast<int>(stats.totalPurchasedAmount)
                  << " ("<< stats.ReturnPct() << "%, annual " << stats.returnPctAnnualized << "%)"
                  << " (" << std::setw(3) << static_cast<int>(stats.DurationAvg()) << " days)" <<  std::endl;
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