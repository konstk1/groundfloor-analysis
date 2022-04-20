#include <algorithm>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>

#include "Loan.hpp"
#include "Portfolio.hpp"

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
    std::map<int, AggregateStats> aggrStats;

    std::cout << std::endl << "Loans by purchase year (repaid/unrepaid): " << std::endl;
    for (const auto &loan: mLoans) {
        auto year = loan.purchasedDate.year;
        auto &stats = aggrStats[year];   // will create blank stats if doesn't exist
        UpdateAggregateStats(stats, loan);
    }

    PrintStats(aggrStats);
}

void Portfolio::LoansByGrade() {
    std::map<std::string, AggregateStats> aggrStats;

    std::cout << std::endl << "Loans by Grade (repaid/unrepaid): " << std::endl;
    for (const auto &loan: mLoans) {
        auto grade = loan.grade;
        auto &stats = aggrStats[grade];   // will create blank stats if doesn't exist
        UpdateAggregateStats(stats, loan);
    }

    PrintStats(aggrStats);
}

void Portfolio::LoansByPerfState() {
    std::map<std::string, AggregateStats> aggrStats;

    std::cout << std::endl << "Loans by Performance State (repaid/unrepaid): " << std::endl;
    for (const auto &loan: mLoans) {
        auto perfState = PerfStateToString(loan.perfState);
        auto &stats = aggrStats[perfState];   // will create blank stats if doesn't exist
        UpdateAggregateStats(stats, loan);
    }

    PrintStats(aggrStats);
}

template <typename T>
void Portfolio::PrintStats(std::map<T, AggregateStats> &aggregateStats) {
    for (const auto &it: aggregateStats) {
        auto key = it.first;
        auto stats = it.second;
        std::cout << std::fixed << std::setprecision(2)
                  << key << "\t#: " << std::setw(3) << stats.numRepaidLoans << " / " << std::setw(3) << stats.numUnrepaidLoans << "\t $" << std::setw(5) << static_cast<int>(stats.totalPurchasedAmount)
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