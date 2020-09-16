#ifndef GF_PARSER_HPP
#define GF_PARSER_HPP

#include <string>

#include "Loan.hpp"
#include "GfParser.hpp"

class GfParser {
public:
    GfParser() = default;
    GfParser(std::string filePath): mFilePath(filePath) {};
    int LoadFromCsv(const std::string &filePath, std::vector<Loan> &loans);
    int LoadFromCsv(std::vector<Loan> &loans);
    Loan ProcessLoanRow(const std::vector<std::string> &tokens);

private:
    std::string mFilePath;
};

#endif