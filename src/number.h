#ifndef _NUMBER_H_
#define _NUMBER_H_
#include "../lib/strings.h"

/*
 * Classe que realiza opera��es sem tornar um n�mero fracion�rio
**/
class Number
{
private:
    int m_num; /* Numerador */
    int m_den; /* Denominador */

    /* fun��es auxiliares */
    void humanNumber(char * number);
    int mdc(int num1, int num2);
    int mmc(int num1, int num2);
    void simplify();
    
public:
    /* construtores */
    Number();
    Number(const Number & rhs);
    Number(double number);
    Number(int numerator, int denominator);
    /* operador divis�o */
    const Number operator/(const Number & rhs) const;
    const Number operator/(int num);
    Number& operator/=(const Number & rhs);
    Number& operator/=(int num);
    /* operador multiplica��o */
    const Number operator*(const Number & rhs) const;
    const Number operator*(int num);
    Number& operator*=(const Number & rhs);
    Number& operator*=(int num);
    /* operador soma */
    const Number operator+(const Number & rhs) const;
    const Number operator+(int num);
    Number& operator+=(const Number & rhs);
    Number& operator+=(int num);
    /* operador subtra��o */
    const Number operator-(const Number & rhs) const;
    const Number operator-(int num);
    Number& operator-=(const Number & rhs);
    Number& operator-=(int num);
    /* operador invers�o de sinal */
    const Number operator-(void) const;
    /* operator assinalar */
    Number& operator=(const Number & rhs);
    /* compara��es */
    bool operator==(const Number & rhs) const;
    bool operator!=(const Number & rhs) const;
    bool operator<(const Number & rhs) const;
    bool operator>(const Number & rhs) const;
    /* convers�es */
    String toString(bool floatMode = false);
    double get() const;
    /* acesso */
    int getUp() const;
    int getDown() const;
};

const Number operator+(int num, const Number& rhs);
const Number operator-(int num, const Number& rhs);
const Number operator*(int num, const Number& rhs);
const Number operator/(int num, const Number& rhs);

#endif

