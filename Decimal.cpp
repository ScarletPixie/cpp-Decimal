#include "Decimal.hpp"

#include <sstream>

// METHODS
double Decimal::num(void) const
{
    std::stringstream ss(this->val);
    double n = 0;
    ss >> n;
    return n;
}

std::string Decimal::str(void) const
{
    return this->val;
}


// CTOR/DTOR
Decimal::Decimal(void):
    val("0")
{}

Decimal::Decimal(double n):
    val("0")
{
    std::stringstream ss;
    ss << n;
    this->val = ss.str();
}

Decimal::Decimal(const std::string& n):
    val(n)
{}

Decimal::Decimal(const Decimal& other):
    val(other.val)
{}

Decimal::~Decimal(void)
{}
