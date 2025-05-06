#ifndef DECIMAL_HPP
#define DECIMAL_HPP

#include <string>
#include <deque>
#include <exception>


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

        class InvalidFormatException: public std::exception
        {
            public:
                const char* what(void) const throw();
        };

    private:
        std::string val;

        ///@brief Removes plus sign, also removes leading zeroes and trailing zeroes after dot.
        void normalize(void);
        void validate(const std::string& n) const;

        static void front_pad(std::deque<char>& v, std::size_t size);
        static void back_pad(std::deque<char>& v, std::size_t size);

};

#endif
