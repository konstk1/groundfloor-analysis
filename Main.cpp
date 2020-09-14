#include <iostream>
#include <stdexcept>

#include <chrono>
#include <ctime>
#include <string>
#include <sstream>
#include <vector>

const std::vector<int> leapYears = { 2000, 2004, 2008, 2012, 2016, 2020, 2024, 2028 };

struct Date {
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
        struct tm start = {0, 0, 0, date.day, date.mon, date.year - 1900};
        struct tm end = {0, 0, 0, day, mon, year - 1900};

        auto s = std::mktime(&start);
        auto e = std::mktime(&end);

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

struct Loan {
    Date start;
};

class Portfolio {

};

int main() {
    std::string d = "9/14/2020";

    Date d1(9, 14, 2020);
    Date d2("9/15/2021");

    std::cout << "D1: " << d1.AsString() << std::endl;
    std::cout << "D2: " << d2.AsString() << std::endl;
    std::cout << "Diff: " << d2.DaysSince(d1) << " days" << std::endl;
    return 0;
}
