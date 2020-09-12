#ifndef _PARSER_H_
#define _PARSER_H_
#include "number.h"

#define TYPE_TERM     1
#define TYPE_VARIABLE 2

class Term
{
public:
    String letter; /* x, y, MinZ, MaxZ, = */
    int type;
public:
    Term()
    {
        type = TYPE_TERM;
    }
};

class Variable: public Term
{
public:
    Number number; /* 1/3, -7 */
    int id;   /* x1, x2 */
public:    
    Variable()
    {
        type = TYPE_VARIABLE;
    }
};

class Expression
{
private:
    bool minOrmax;
    bool basicRest;
    std::vector<Term*> items;
public:
    Expression();
    Expression(const Expression& expr);
    Expression& operator=(const Expression& rhs);
    ~Expression();
    bool isFunction();
    bool isBasicRestriction();
    void add(Term* term);
    void erase(int index);
    Term* get(int index);
    int count();
    void clear();
};

class Parser
{
private:
    Expression expr;
    int selstart;
    int errorCode;
public:
    Parser();
    int execute(String expression);
    Expression& getExpression();
    int getSelStart();
    String getErrorMsg();
};

#endif
