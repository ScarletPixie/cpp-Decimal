#include "Decimal.hpp"

#include <cstddef>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <vector>
#include <stack>


static std::size_t getDiff(std::size_t n1, std::size_t n2);


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
        throw InvalidFormatException(n);

    const std::size_t startPos = n[0] == '-' or n[0] == '+';

    if (startPos != 0 and n.size() == 1)
        throw InvalidFormatException(n);

    bool commaFound = false;
    for (std::size_t i = startPos; i < n.size(); ++i)
    {
        if (n[i] == '.' and not commaFound)
        {
            commaFound = true;
            continue;
        }
        if (not isdigit(n[i]))
            throw InvalidFormatException(n);
    }
}
void Decimal::front_pad(std::deque<char>& v, std::size_t size)
{
    for (std::size_t i = 0; i < size; ++i)
        v.push_front('0');
}
void Decimal::back_pad(std::deque<char>& v, std::size_t size)
{
    for (std::size_t i = 0; i < size; ++i)
        v.push_back('0');
}
void Decimal::updateFromVal(void)
{
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
            this->fpart.push_back(this->val[i]);
        else
            this->ipart.push_back(this->val[i]);
    }
}


// CTOR/DTOR
Decimal::Decimal(void):
    val("0")
{
    this->updateFromVal();
}

Decimal::Decimal(double n):
    val("0")
{
    std::stringstream ss;
    ss << n;
    this->val = ss.str();
    this->updateFromVal();
}

Decimal::Decimal(const std::string& n):
    val(n)
{
    this->validate(n);
    this->normalize();
    this->updateFromVal();
}

Decimal::Decimal(const Decimal& other):
    val(other.val),
    ipart(other.ipart),
    fpart(other.fpart)
{}

Decimal::~Decimal(void)
{}


// ARITHMETIC OPERATORS
Decimal Decimal::operator + (const Decimal& rhs) const
{
    bool isNegativeResult = false;
    std::string result;

    if (this->val[0] == '-' and rhs.val[0] == '-')
        isNegativeResult = true;
    else if (this->val[0] == '-')
        return Decimal(this->val.substr(1)) - rhs;
    else if (rhs.val[0] == '-')
        return *this - Decimal(rhs.val.substr(1));

    int carry = 0;
    const std::size_t maxDigitCount = std::max(this->ipart.size(), rhs.ipart.size());
    const std::size_t maxFractDigitCount = std::max(this->fpart.size(), rhs.fpart.size());

    // FRACTIONAL PART
    if (maxFractDigitCount)
    {
        const std::size_t fractSizeDiff = getDiff(this->fpart.size(), rhs.fpart.size());
        const std::size_t minSizeCount = std::min(this->fpart.size(), rhs.fpart.size());

        std::vector<char>::const_reverse_iterator rhsIt = rhs.fpart.rbegin();
        std::vector<char>::const_reverse_iterator lhsIt = this->fpart.rbegin();
        std::vector<char>::const_reverse_iterator* it = (this->fpart.size() > rhs.fpart.size() ? &lhsIt : &rhsIt);
        for (std::size_t i = 0; i < fractSizeDiff; ++i)
        {
            result.push_back(**it);
            ++(*it);
        }

        bool rhsEnd = false;
        bool lhsEnd = false;
        for (std::size_t i = 0; i < minSizeCount; ++i)
        {
            if (rhsIt == rhs.fpart.rend())
                rhsEnd = true;
            if (lhsIt == this->fpart.rend())
                lhsEnd = true;
            
            int rhsVal = (!rhsEnd ? *rhsIt - '0' : 0);
            int lhsVal = (!lhsEnd ? *lhsIt - '0' : 0);
            
            if (carry)
            {
                lhsVal += 10;
                carry = 0;
            }

            int res = lhsVal + rhsVal;
            carry = (res / 10) % 10;

            if (carry)
                res -= 10;
            
            result.push_back(res + '0');

            if (!rhsEnd)
                ++rhsIt;
            if (!lhsEnd)
                ++lhsIt;
        }
        result.push_back('.');
    }

    // INTEGER PART
    bool fractToIntCarryOver = carry;
    bool rhsEnd = false;
    bool lhsEnd = false;
    std::deque<char>::const_reverse_iterator rhsIt = rhs.ipart.rbegin();
    std::deque<char>::const_reverse_iterator lhsIt = this->ipart.rbegin();
    for (std::size_t i = 0; i < maxDigitCount; ++i)
    {
        if (rhsIt == rhs.ipart.rend() or *rhsIt == '-')
            rhsEnd = true;
        if (lhsIt == this->ipart.rend() or *lhsIt == '-')
            lhsEnd = true;

        int rhsVal = (!rhsEnd ? *rhsIt - '0' : 0);
        int lhsVal = (!lhsEnd ? *lhsIt - '0' : 0);
        if (fractToIntCarryOver)
        {
            lhsVal += 1;
            carry = 0;
        }
        else if (carry)
        {
            lhsVal += 10;
            carry = 0;
        }

        int res = lhsVal + rhsVal;
        carry = (res / 10) % 10;
        if (carry)
            res -= 10;
        
        result.push_back(res + '0');

        if (!rhsEnd)
            ++rhsIt;
        if (!lhsEnd)
            ++lhsIt;
    }

    if (carry)
        result.push_back('1');
    if (isNegativeResult)
        result.push_back('-');
    std::reverse(result.begin(), result.end());
    return Decimal(result);
}
Decimal Decimal::operator - (const Decimal& rhs) const
{
    bool negativeResult = false;
    const Decimal* n1 = this;
    const Decimal* n2 = &rhs;

    if (this->val[0] != '-' and rhs.val[0] == '-')
        return *this + Decimal(rhs.val.substr(1));
    else if (this->val[0] == '-' and rhs.val[0] != '-')
        return -(rhs + Decimal(this->val.substr(1)));
    if (rhs > *this)
    {
        n1 = &rhs;
        n2 = this;
        negativeResult = true;
    }

    std::string result;
    const std::size_t maxIntDigitCount = std::max(this->ipart.size(), rhs.ipart.size());
    const std::size_t maxFractDigitCount = std::max(this->fpart.size(), rhs.fpart.size());

    std::stack<int> loan;
    std::stack<int> debt;

    // FRACTIONAL PART
    if (maxFractDigitCount)
    {
        const std::size_t fractSizeDiff = getDiff(this->fpart.size(), rhs.fpart.size());
        const std::size_t minSizeCount = std::min(this->fpart.size(), rhs.fpart.size());

        std::vector<char>::const_reverse_iterator lhsIt = n1->fpart.rbegin();
        std::vector<char>::const_reverse_iterator rhsIt = n2->fpart.rbegin();
        std::vector<char>::const_reverse_iterator* it = (n1->fpart.size() > n2->fpart.size() ? &lhsIt : &rhsIt);
        for (std::size_t i = 0; i < fractSizeDiff; ++i)
        {
            result.push_back(**it);
            ++(*it);
        }

        bool rhsEnd = false;
        bool lhsEnd = false;
        for (std::size_t i = 0; i < minSizeCount; ++i)
        {
            if (rhsIt == rhs.fpart.rend())
                rhsEnd = true;
            if (lhsIt == this->fpart.rend())
                lhsEnd = true;
            
            int lhsVal = (!lhsEnd ? *lhsIt - '0' : 0);
            int rhsVal = (!rhsEnd ? *rhsIt - '0' : 0);

            // apply debt
            if (!debt.empty())
            {
                lhsVal -= 1;
                debt.pop();
            }

            // debt carry over/operation
            if (lhsVal < 0)
            {
                debt.push(1);
                lhsVal = 9;
            }
            else if (lhsVal < rhsVal)
            {
                debt.push(1);
                lhsVal += 10;
            }
        }
        result.push_back('.');
    }

    // INTEGER PART
    bool rhsEnd = false;
    bool lhsEnd = false;
    std::deque<char>::const_reverse_iterator lhsIt = n1->ipart.rbegin();
    std::deque<char>::const_reverse_iterator rhsIt = n2->ipart.rbegin();
    for (std::size_t i = 0; i < maxIntDigitCount; ++i)
    {
        if (rhsIt == rhs.ipart.rend())
            rhsEnd = true;
        if (lhsIt == this->ipart.rend())
            lhsEnd = true;
        
        int lhsVal = (!lhsEnd ? *lhsIt - '0' : 0);
        int rhsVal = (!rhsEnd ? *rhsIt - '0' : 0);

        // apply debt
        if (!debt.empty())
        {
            lhsVal -= 1;
            debt.pop();
        }

        // debt carry over/operation
        if (lhsVal < 0)
        {
            debt.push(1);
            lhsVal = 9;
        }
        else if (lhsVal < rhsVal)
        {
            debt.push(1);
            lhsVal += 10;
        }
    }

    if (negativeResult)
        result.push_back('-');
    std::reverse(result.begin(), result.end());
    return Decimal(result);
}

// OPERATORS
Decimal Decimal::operator - (void) const
{
    if (this->val[0] == '-')
        return Decimal(this->val.substr(1));
    return Decimal("-" + this->val);
}
Decimal& Decimal::operator = (const Decimal& rhs)
{
    this->val = rhs.val;
    this->ipart = rhs.ipart;
    this->fpart = rhs.fpart;
    return *this;
}
bool Decimal::operator == (const Decimal& rhs) const
{
    return this->val == rhs.val;
}
bool Decimal::operator > (const Decimal& rhs) const
{
    if (this->ipart.size() > rhs.ipart.size())
        return true;
    else if (rhs.ipart.size() > this->ipart.size())
        return false;

    for (std::size_t i = 0; i < this->ipart.size(); ++i)
    {
        if (this->ipart[i] != rhs.ipart[i])
            return this->ipart[i] > rhs.ipart[i];
    }

    const std::size_t maxFractDigits = std::max(this->fpart.size(), rhs.fpart.size());
    for (std::size_t i = 0; i < maxFractDigits; ++i)
    {
        const int lhsVal = (i < this->fpart.size() ? this->fpart.size() - '0' : 0);
        const int rhsVal = (i < rhs.fpart.size() ? rhs.fpart.size() - '0' : 0);

        if (lhsVal != rhsVal)
            return lhsVal < rhsVal;
    }

    return false;
}



// EXCEPTIONS
Decimal::InvalidFormatException::InvalidFormatException(const std::string& message):
    message(message)
{}
const char* Decimal::InvalidFormatException::what(void) const throw()
{
    return this->message.c_str();
}
Decimal::InvalidFormatException::~InvalidFormatException(void) throw()
{}


// HELPERS
static std::size_t getDiff(std::size_t n1, std::size_t n2)
{
    if (n1 > n2)
        return n1 - n2;
    return n2 - n1;
}