#include "Decimal.hpp"

#include <sstream>
#include <algorithm>
#include <cctype>

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

void Decimal::validate(const std::string& n) const
{
    if (n.empty())
        throw InvalidFormatException();

    const std::size_t startPos = n[0] == '-' or n[0] == '+';

    if (startPos != 0 and n.size() == 1)
        throw InvalidFormatException();

    bool commaFound = false;
    for (std::size_t i = startPos; i < n.size(); ++i)
    {
        if (n[i] == '.' and not commaFound)
        {
            commaFound = true;
            continue;
        }
        if (not isdigit(n[i]))
            throw InvalidFormatException();
    }
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
    this->validate(n);
    this->normalize();
}

Decimal::Decimal(const Decimal& other):
    val(other.val)
{}

Decimal::~Decimal(void)
{}


// ARITHMETIC OPERATORS
Decimal Decimal::operator + (const Decimal& rhs) const
{
    std::string result;
    std::pair<std::vector<char>, std::vector<char> > lhsDigits;
    std::pair<std::vector<char>, std::vector<char> > rhsDigits;

    bool fractPart = false;
    for (std::size_t i = 0; i < this->val.size(); ++i)
    {
        if (this->val[i] == '.')
        {
            fractPart = true;
            continue;
        }
        else if (!isdigit(this->val[i]))
            continue;
        if (fractPart)
            lhsDigits.second.push_back(this->val[i]);
        else
            lhsDigits.first.push_back(this->val[i]);
    }
    fractPart = false;
    for (std::size_t i = 0; i < rhs.val.size(); ++i)
    {
        if (rhs.val[i] == '.')
        {
            fractPart = true;
            continue;
        }
        else if (!isdigit(rhs.val[i]))
            continue;
        if (fractPart)
            rhsDigits.second.push_back(rhs.val[i]);
        else
            rhsDigits.first.push_back(rhs.val[i]);
    }

    if (lhsDigits.first.size() != rhsDigits.first.size())
    {
        const std::size_t diff = std::max(lhsDigits.first.size(), rhsDigits.first.size()) - std::min(lhsDigits.first.size(), rhsDigits.first.size()); 
        Decimal::front_pad((lhsDigits.first.size() > rhsDigits.size() ? lhsDigits.first : rhsDigits.first), diff);
    }
    if (lhsDigits.second.size() != rhsDigits.second.size())
    {
        const std::size_t diff = std::max(lhsDigits.second.size(), rhsDigits.second.size()) - std::min(lhsDigits.second.size(), rhsDigits.second.size()); 
        Decimal::back_pad((lhsDigits.second.size() > rhsDigits.size() ? lhsDigits.second : rhsDigits.second), diff);
    }




    return Decimal(rhs);
}

// OPERATORS
Decimal& Decimal::operator = (const Decimal& rhs)
{
    this->val = rhs.val;
    return *this;
}
bool Decimal::operator == (const Decimal& rhs) const
{
    return this->val == rhs.val;
}


// EXCEPTIONS
const char* Decimal::InvalidFormatException::what(void) const throw()
{
    return "Decimal: Bad string format";
}
