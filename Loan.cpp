#include <iomanip>

#include "Loan.hpp"

std::ostream &operator<<(std::ostream &os, Date const &d) { 
    return os << std::setw(2) << std::setfill('0') << d.mon << "/" 
              << std::setw(2) << std::setfill('0') << d.day << "/" 
               << d.year;
}
