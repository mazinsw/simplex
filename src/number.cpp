#include "number.h"
#include <math.h>
#include <stdio.h>
#include <string.h>
//#define TEST_NUMBER

const Number operator+(int num, const Number& rhs)
{
    return Number(num) + rhs;
}

const Number operator-(int num, const Number& rhs)
{
    return Number(num) - rhs;
}

const Number operator*(int num, const Number& rhs)
{
    return Number(num) * rhs;
}

const Number operator/(int num, const Number& rhs)
{
    return Number(num) / rhs;
}

Number::Number()
{
    m_num = 0;
    m_den = 1;
}

Number::Number(const Number & rhs)
{
    m_num = rhs.m_num;
    m_den = rhs.m_den;
}

Number::Number(double number)
{
    double num, den;
    int i;
    
    // bug #INF
    num = number;
    den = 1;
    i = 0;
    while(num != (int)num)
    {
        i++;
        num *= 10;
        den *= 10;
        if(i == 6)
            break;
    }
    m_num = (int)num;
    m_den = (int)den;
    simplify();
}

Number::Number(int numerator, int denominator)
{
    m_num = numerator;
    if(denominator == 0)
        m_den = 1;
    else
        m_den = denominator;
    simplify();
}

void Number::humanNumber(char * number)
{
    char * end = number + strlen(number) - 1;
    char erase = 1;

    while(end >= number)
    {
        if(end[0] == '.')
        {
            end[0] = ',';
            break;
        }
        else if(end[0] == '0')
        {
            if(erase == 1)
                end[0] = 0;
        }
        else
            erase = 0;
        end--;
    }
    if(end >= number && end[0] == ',')
    {
        if(end[1] == 0)
        {
            end[0] = 0;
            end--;
            erase = 1;
            while(end >= number)
            {
                if(end[0] != '0')
                {
                    erase = 0;
                    break;
                }
                end--;
            }
            if(erase == 1)
                number[1] = 0;
        }
    }
}

int Number::mdc(int num1, int num2)
{
    int rest;

    while((num1 % num2) != 0)
    {
        rest = num1 % num2;
        num1 = num2;
        num2 = rest;
    }

    return num2;
}

int Number::mmc(int num1, int num2)
{
    return (num1 * num2) / mdc(num1, num2);
}

void Number::simplify()
{
    int c;

    c = mdc(m_num, m_den);
    m_num /= c;
    m_den /= c;
    if(m_den < 0)
    {
        m_num *= -1;
        m_den *= -1;
    }
}

const Number Number::operator/(const Number & rhs) const
{
    return Number(*this) /= rhs;
}

const Number Number::operator/(int num)
{
    return Number(*this) /= num;
}

Number& Number::operator/=(const Number & rhs)
{
    (*this) *= Number(rhs.m_den, rhs.m_num);
    return *this;
}

Number& Number::operator/=(int num)
{
    (*this) *= Number(1, num);
    return *this;
}

const Number Number::operator*(const Number & rhs) const
{
    return Number(*this) *= rhs;
}

const Number Number::operator*(int num)
{
    return Number(*this) *= num;
}

Number& Number::operator*=(const Number & rhs)
{
    int c;

    m_num = m_num * rhs.m_num;
    m_den = m_den * rhs.m_den;
    c = mdc(m_num, m_den);
    m_num = m_num / c;
    m_den = m_den / c;
    simplify();
    return *this;
}

Number& Number::operator*=(int num)
{
    (*this) *= Number(num);
    return *this;
}

const Number Number::operator+(const Number & rhs) const
{
    return Number(*this) += rhs;
}

const Number Number::operator+(int num)
{
    return Number(*this) += num;
}

Number& Number::operator+=(const Number & rhs)
{
    int den;

    den = mmc(rhs.m_den, m_den);
    m_num = (m_num * den / m_den) + (rhs.m_num * den / rhs.m_den);
    m_den = den;
    simplify();
    return *this;
}

Number& Number::operator+=(int num)
{
    (*this) += Number(num);
    return *this;
}

const Number Number::operator-(const Number & rhs) const
{
    return Number(*this) -= rhs;
}

const Number Number::operator-(int num)
{
    return Number(*this) -= num;
}

Number& Number::operator-=(const Number & rhs)
{
    (*this) += (-rhs);
    return *this;
}

Number& Number::operator-=(int num)
{
    (*this) -= Number(num);
    return *this;
}

const Number Number::operator-(void) const
{
    return Number(-m_num, m_den);
}

Number& Number::operator=(const Number & rhs)
{
    m_num = rhs.m_num;
    m_den = rhs.m_den;
    return *this;
}

bool Number::operator==(const Number & rhs) const
{
    return (m_num == rhs.m_num) && (m_den == rhs.m_den);
}

bool Number::operator!=(const Number & rhs) const
{
    return !((*this) == rhs);
}

bool Number::operator<(const Number & rhs) const
{
    return get() < rhs.get();
}

bool Number::operator>(const Number & rhs) const
{
    return get() > rhs.get();
}

String Number::toString(bool floatMode)
{
    char tmp[16];
    String result;

    if(floatMode)
    {
        sprintf(tmp, "%.6lf", get());
        humanNumber(tmp);
        result = tmp;
        return result;
    }
    if(m_den != 1)
        result.format("%d/%d", m_num, m_den);
    else
        result.format("%d", m_num);
    return result;
}

double Number::get() const
{
    return (double) m_num / m_den;
}

int Number::getUp() const
{
    return m_num;
}

int Number::getDown() const
{
    return m_den;
}

#ifdef TEST_NUMBER
int main(int argc, char *argv[])
{
    Number a(2.5), b(4), c(4, 3), d(3, 2);
    
    printf("a: %s, a: %s, a: %s, a: %s\n", 
        a.toString().c_str(), b.toString().c_str(), 
        c.toString().c_str(), d.toString().c_str());
    
    a += Number(1, 2);
    b = c + d;
    c /= 2;
    d *= 4;
    printf("a: %s, a: %s, a: %s, a: %s\n", 
        a.toString().c_str(), b.toString().c_str(), 
        c.toString().c_str(), d.toString().c_str());

    if( 2 * a == d)
        printf("2 * %s eh igual a %s\n", a.toString().c_str(), d.toString().c_str());
    else
        printf("2 * %s eh diferent de %s\n", a.toString().c_str(), d.toString().c_str());

    if( 6 * c != b)
        printf("6 * %s eh diferent de %s\n", c.toString().c_str(), b.toString().c_str());
    else
        printf("6 * %s eh igual a %s\n", c.toString().c_str(), b.toString().c_str());

    if( d < a)
        printf("%s eh menor que %s\n", d.toString().c_str(), a.toString().c_str());
    else
        printf("%s nao eh menor que %s\n", d.toString().c_str(), a.toString().c_str());

    if( b > c)
        printf("%s eh menor que %s\n", b.toString().c_str(), c.toString().c_str());
    else
        printf("%s nao eh menor que %s\n", b.toString().c_str(), c.toString().c_str());
    return 0;
}

#endif

