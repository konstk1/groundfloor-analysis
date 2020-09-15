#ifndef LOAN_HPP
#define LOAN_HPP

#include <iostream>
#include <iomanip>
#include <stdexcept>

#include <chrono>
#include <ctime>
#include <string>
#include <sstream>
#include <vector>

const std::vector<int> leapYears = { 2000, 2004, 2008, 2012, 2016, 2020, 2024, 2028 };

struct Date {
    Date(): Date(0, 0, 1900) {};

    Date(int mon, int day, int year): mon(mon), day(day), year(year) {};

    Date(std::string date) {
        std::istringstream ss(date);
        char slash1, slash2;
        ss >> mon >> slash1 >> day >> slash2 >> year;

        if (slash1 != '/' || slash2 != '/' || 
            mon < 1 || mon > 12 ||
            day < 1 || day > 31 ||
            year < 2000) {
            std::cerr << "Invalid date: " << date << std::endl;
            throw std::runtime_error("Invalid date format");
        }
    }

    int DaysSince(Date &date) {
        struct tm start = {0, 0, 0, date.day, date.mon - 1, date.year - 1900};
        struct tm end = {0, 0, 0, day, mon - 1, year - 1900};

        auto s = std::mktime(&start);
        auto e = std::mktime(&end);

        if (s == static_cast<time_t>(-1) || e == static_cast<time_t>(-1)) {
            std::cerr << "Invalid date";
            throw std::runtime_error("Invalid date");
        }

        double diffSec = std::difftime(e, s);
        return diffSec / (60 * 60 * 24);
    }

    std::string AsString() {
        std::ostringstream os;

        os << mon << "/" << day << "/" << year;
        return os.str();
    }

    int mon = 0;    // month
    int day = 0;    // day
    int year = 0;   // year

private:
    static constexpr int YEAR_0 = 2000;     // TODO count leap years

};

struct Address {
    Address() = default;

    std::string street;
    std::string city;
    std::string state;
    std::string zipcode;
};

struct Loan {
    Loan() = default;

    enum class LoanState {
        UNKNOWN,
        FUNDED,
        REPAID,
        REFUNDED,
    };

    enum class PerfState {
        UNKNOWN,
        PERFORMING,
        PERFORMED,
        EXTENDED,
        DEFAULT,
    };

    std::string id;
    std::string title;
    LoanState loanState{LoanState::UNKNOWN};
    PerfState perfState{PerfState::UNKNOWN};

    Address address;

    float rate{0};
    int term{0};
    std::string grade;
    float arv{0};

    float purchasedAmount{0};
    float repaidAmount{0};

    Date purchasedDate;
    Date maturityDate;
    Date repaidDate;

    void Print() {
        std::cout << "Loan: " << title << std::endl;
        std::cout << address.street << ", " << address.city << ", " << address.state << ", " << address.zipcode << std::endl;
        std::cout << "Grade: " << grade << ", Term: " << term << ", Rate: " << rate << "%" << std::endl;
        std::cout << "Purchased: " << purchasedDate.AsString() << ", Maturity: " << maturityDate.AsString() << ", Repaid: " << repaidDate.AsString() << std::endl;
        std::cout << std::fixed << std::setprecision(2);
        std::cout << "Amount: $" << purchasedAmount << ", Repaid: $" << repaidAmount << std::endl;
    }

    std::vector<Loan> CalendarYearSplit() {

    };
};


#endif