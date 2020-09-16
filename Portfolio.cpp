
#include <iostream>
#include <string>
#include <vector>

#include "Loan.hpp"
#include "Portfolio.hpp"


void Portfolio::LoansByYear() {
    std::cout << "Loans by years: " << std::endl;
    for (const auto &loan: mLoans) {
        if (loan.purchasedDate.year == 2018) {
            std::cout << loan.purchasedDate.AsString() << " " << loan.title << " " << loan.purchasedAmount << std::endl;
        }
    }
}