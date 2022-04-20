#include <iostream>
#include <vector>

#include "Loan.hpp"
#include "Portfolio.hpp"

void loanTest() {
    Loan loan;
    loan.address.street = "123 Main Street";
    loan.address.city = "Boston";
    loan.address.state = "MA";
    loan.address.zipcode = "02138";

    loan.grade = "A";
    loan.term = 12;
    loan.rate = 7.5;

    loan.purchasedDate = Date("1/1/2020");
    loan.maturityDate = Date("7/6/2020");
    loan.repaidDate = Date("7/1/2020");

    loan.purchasedAmount = 100;
    loan.repaidAmount = 115;

    loan.Print();
}

void dateTest() {
    Date d1(2, 29, 2020);
    Date d2("3/1/2020");

    std::cout << "D1: " << d1.AsString() << std::endl;
    std::cout << "D2: " << d2.AsString() << std::endl;
    std::cout << "Diff: " << d2.DaysSince(d1) << " days" << std::endl;
}

int main() {
    GfParser parser("groundfloor_investments.csv");
    Portfolio p(parser);

    p.LoansByYear();
    p.LoansByGrade();
    p.LoansByPerfState();

    // p.GenerateXirrFile("xirr.csv");
    // loanTest();

    return 0;
}