#include <iomanip>

#include "Loan.hpp"

void Loan::Print() const {
    std::cout << "Loan: " << title << std::endl;
    std::cout << address.street << ", " << address.city << ", " << address.state << ", " << address.zipcode << std::endl;
    std::cout << "Grade: " << grade << ", Term: " << term << ", Rate: " << rate << "%, ARV: " << arvPct << "%" << std::endl;
    std::cout << "Purchased: " << purchasedDate.AsString() << ", Maturity: " << maturityDate.AsString() << ", Repaid: " << repaidDate.AsString() << std::endl;
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Amount: $" << purchasedAmount << ", Repaid: $" << repaidAmount
              << " (" << ReturnPct() << "%, Annual: " << ReturnPctAnnualized() << "%)" << std::endl;
}

std::vector<Transaction> Loan::GetTransactions() const {
    std::vector<Transaction> transactions;

    transactions.push_back(Transaction{id, title, purchasedDate, 0 - purchasedAmount});  // purchase is negative transaction

    if (repaidDate.IsValid()) {
        transactions.push_back(Transaction{id, title, repaidDate, repaidAmount});  // repaid is positive transaction
    }

    return transactions;
};

static const std::vector<std::string> PerfStateNames = {
    "Unknown",
    "Performing",
    "Performed",
    "Extended",
    "Refunded",
    "Defaulted",
};

std::string PerfStateToString(Loan::PerfState state) {
    return PerfStateNames[static_cast<int>(state)];
}