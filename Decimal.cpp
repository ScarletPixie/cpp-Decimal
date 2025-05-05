#include "Decimal.hpp"

#include <sstream>
#include <algorithm>

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

void Decimal::normalize(void)
{
    std::size_t start = 0;
    if (this->val[0] == '-' or this->val[0] == '+')
        start = 1;
    
    std::size_t digitPos = this->val.find_first_not_of("0", start);
    if (digitPos == std::string::npos)
        this->val = "0";
    else if (this->val.at(digitPos) == '.')
        this->val.replace(start, (digitPos - start), "0");
    else
        this->val.replace(start, (digitPos - start), "");

    std::size_t fractPos = this->val.find_last_not_of("0");
    if (fractPos == std::string::npos)
        this->val = "0";
    else if (this->val.at(fractPos) == '.')
        this->val.replace(fractPos, (this->val.size() - fractPos), "");
    else if (this->val.at(fractPos) != '.' && fractPos + 1 != this->val.size() && std::count(this->val.begin(), this->val.end(), '.'))
        this->val.replace(fractPos + 1, (this->val.size() - fractPos), "");

    if (this->val[0] == '+')
        this->val.replace(0, 1, "");
    
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
{
    this->normalize();
}

Decimal::Decimal(const Decimal& other):
    val(other.val)
{}

Decimal::~Decimal(void)
{}
