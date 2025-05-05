#ifndef DECIMAL_HPP
#define DECIMAL_HPP

#include <string>
#include <vector>

class Decimal
{
    public:
        Decimal(void);
        Decimal(double n);
        Decimal(const std::string& n);
        Decimal(const Decimal& other);
        ~Decimal(void);

        double num(void) const;
        std::string str(void) const;

        Decimal& operator = (const Decimal& rhs);

        Decimal operator + (const Decimal& rhs) const;
        Decimal operator - (const Decimal& rhs) const;
        Decimal operator / (const Decimal& rhs) const;
        Decimal operator * (const Decimal& rhs) const;

        bool operator == (const Decimal& rhs) const;
        bool operator != (const Decimal& rhs) const;
        bool operator >= (const Decimal& rhs) const;
        bool operator <= (const Decimal& rhs) const;

        bool operator < (const Decimal& rhs) const;
        bool operator > (const Decimal& rhs) const;

    private:
        std::string val;

        ///@brief Removes meaningless signals.
        void normalize(void);

};

#endif
