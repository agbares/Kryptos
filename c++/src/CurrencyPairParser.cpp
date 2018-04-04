// CurrencyPairParser.cpp
// CurrencyPairParser Class Implementation
// Author: Antonio G. Bares Jr.

#include "CurrencyPairParser.h"

// Utilities
CurrencyPair CurrencyPairParser::parseLine(const std::string& line)
{
    std::string from;
    std::string to;
    std::string tempStr;
    double price;

    std::stringstream lineStream;
    lineStream << line;

    std::getline(lineStream, from, ',');
    std::getline(lineStream, to, ',');
    std::getline(lineStream, tempStr, '\n');

    price = std::stod(tempStr);

    return CurrencyPair(from, to, price);
}

std::list<CurrencyPair> CurrencyPairParser::parseCurrenciesString(const std::string& data)
{
    
}

std::list<CurrencyPair> CurrencyPairParser::parseFileAndGetListOfCurrencies(const std::string& fileName)
{

}