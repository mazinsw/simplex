#include "parser.h"
#include <string.h>
#include <stdlib.h>

Expression::Expression()
{
    minOrmax = false;
    basicRest = false;
}

Expression::Expression(const Expression& expr)
{
    std::vector<Term*>::const_iterator it;

    minOrmax = expr.minOrmax;
    basicRest = expr.basicRest;
    for(it = expr.items.begin(); it != expr.items.end(); it++)
        add(*it);
}

Expression& Expression::operator=(const Expression& rhs)
{
    std::vector<Term*>::const_iterator it;

    clear();
    minOrmax = rhs.minOrmax;
    basicRest = rhs.basicRest;
    for(it = rhs.items.begin(); it != rhs.items.end(); it++)
        add(*it);
    return *this;
}

Expression::~Expression()
{
    std::vector<Term*>::iterator it;

    for(it = items.begin(); it != items.end(); it++)
        delete *it;
}

bool Expression::isFunction()
{
    return minOrmax;
}

void Expression::add(Term* term)
{
    Term *n;
    Variable *vt, *vn;

    if(term->type == TYPE_TERM)
    {
        n = new Term();
        n->letter = term->letter;
        n->type = term->type;
        if(count() == 0 && (n->letter == "MINZ" || n->letter == "MAXZ"))
        {
            minOrmax = true;
            basicRest = false;
        }
        else if(n->letter == ",")
        {
            basicRest = true;
            minOrmax = false;
        }
        items.push_back(n);
    }
    else
    {
        vt = reinterpret_cast<Variable*>(term);
        vn = new Variable();
        vn->letter = vt->letter;
        vn->type = vt->type;
        vn->number = vt->number;
        vn->id = vt->id;
        items.push_back(vn);
    }
}

bool Expression::isBasicRestriction()
{
    return basicRest;    
}

void Expression::erase(int index)
{
    Term *n = items[index];

    if(index == 0 && (n->letter == "MINZ" || n->letter == "MAXZ"))
        minOrmax = false;
    delete n;
    items.erase(items.begin() + index);
}

Term* Expression::get(int index)
{
    return items[index];
}

int Expression::count()
{
    return items.size();
}

void Expression::clear()
{
    std::vector<Term*>::iterator it;

    for(it = items.begin(); it != items.end(); it++)
        delete *it;
    items.clear();
}

Parser::Parser()
{
    selstart = 0;
}

int Parser::execute(String expression)
{
    const char * ptr = expression.c_str();
    char tmp[20], tmp2[20];
    int tmp_count = 0, tmp_count2 = 0;
    int frac_num, int_num, den_num, frac_tmp;
    Number n, d;
    Number num = 1;
    Term * term;
    Variable * v;
    String last, var;
    bool is_func = false;
    bool has_comp = false;
    bool is_restr = false;
    bool contains_sig = false;
    char last_sig = 0, ch, den_sig = 0;

    // Min Z = 2x1 - 3x5 + 25/4x5
    //
    expr.clear();
    while(*ptr)
    {
        switch(ptr[0])
        {
        case '0'...'9':
            tmp_count = 0;
            den_sig = 0;
            while(true)
            {
                tmp[tmp_count] = ptr[0];
                tmp_count++;
                tmp[tmp_count] = 0;
                ptr++;
                if(ptr[0] < '0' || ptr[0] > '9')
                    break;
            }
            tmp2[0] = '0';
            tmp2[1] = 0;
            tmp_count2 = 0;
            if((ptr[0] == ',' && last != "icog") || ptr[0] == '.')
            {
                ptr++;
                if(ptr[0] >= '0' && ptr[0] <= '9')
                {
                    while(true)
                    {
                        tmp2[tmp_count2] = ptr[0];
                        tmp_count2++;
                        tmp2[tmp_count2] = 0;
                        ptr++;
                        if(ptr[0] < '0' || ptr[0] > '9')
                            break;
                    }
                }
                else
                {
                    // incomplete float number
                    expr.clear();
                    return errorCode = 14;
                }
            }
            den_num = 1;
            int_num = atoi(tmp);
            frac_num = atoi(tmp2);
            frac_tmp = frac_num;
            while(frac_tmp > 0)
            {
                frac_tmp /= 10;
                den_num *= 10;
                int_num *= 10;
            }
            int_num += frac_num;
            n = Number(int_num, den_num);
            d = 1;
            while(ptr[0] == ' ')
                ptr++;
            if(ptr[0] == '/')
            {
                ptr++;
                while(ptr[0] == ' ')
                    ptr++;
                if(ptr[0] == '-')
                {
                    den_sig = '-';
                    ptr++;
                    while(ptr[0] == ' ')
                        ptr++;
                }
                if(ptr[0] >= '0' && ptr[0] <= '9')
                {
                    tmp_count = 0;
                    while(true)
                    {
                        tmp[tmp_count] = ptr[0];
                        tmp_count++;
                        tmp[tmp_count] = 0;
                        ptr++;
                        if(ptr[0] < '0' || ptr[0] > '9')
                            break;
                    }
                    tmp2[0] = '0';
                    tmp2[1] = 0;
                    tmp_count2 = 0;
                    if(ptr[0] == ',' || ptr[0] == '.')
                    {
                        ptr++;
                        if(ptr[0] >= '0' && ptr[0] <= '9')
                        {

                            while(true)
                            {
                                tmp2[tmp_count2] = ptr[0];
                                tmp_count2++;
                                tmp2[tmp_count2] = 0;
                                ptr++;
                                if(ptr[0] < '0' || ptr[0] > '9')
                                    break;
                            }
                        }
                        else
                        {
                            // incomplete float number
                            expr.clear();
                            return errorCode = 14;
                        }
                    }
                    den_num = 1;
                    int_num  = atoi(tmp);
                    frac_num = atoi(tmp2);
                    frac_tmp = frac_num;
                    while(frac_tmp > 0)
                    {
                        frac_tmp /= 10;
                        den_num *= 10;
                        int_num *= 10;
                    }
                    int_num += frac_num;
                    d = Number(int_num, den_num);
                    //division by zero
                    if(d == 0)
                    {
                        expr.clear();
                        return errorCode = 2;
                    }
                }
                else
                {
                    //invalid constant      10/?
                    expr.clear();
                    return errorCode = 3;
                }
            }
            if(last == "icog")
            {
                if(n.getDown() != 1)
                {
                    //invalid variable index
                    expr.clear();
                    return errorCode = 15;
                }
                v = new Variable();
                v->number = num;
                v->letter = var;
                v->id = n.getUp();
                v->type = TYPE_VARIABLE;
                expr.add(v);
                delete v;
                last = "varid";

            }
            else if(last == "sig" || last == "comp" || last == "")
            {
                if(last_sig == '-')
                    n = -n;
                if(den_sig == '-')
                    d = -d;
                last_sig = 0;
                num = n / d;
                last = "num";
            }
            continue;
            break;
        case 'a'...'z':
        case 'A'...'Z':
            if(last == "sig" || last == "comp")
            {
                if(last_sig == '-')
                    num = -1;
                else
                    num = 1;
                last_sig = 0;
            }
            tmp_count = 0;
            ch = ptr[0];
            if(ch >= 'a' && ch <= 'z')
                ch -= 'a' - 'A';
            while(ch >= 'A' && ch <= 'Z')
            {
                if(tmp_count == 20)
                {
                    //var or function too large
                    expr.clear();
                    return errorCode = 4;
                }
                tmp[tmp_count] = ch;
                tmp_count++;
                tmp[tmp_count] = 0;
                ptr++;
                ch = ptr[0];
                if(ch >= 'a' && ch <= 'z')
                    ch -= 'a' - 'A';
            }
            if(String(tmp) == "MIN" || String(tmp) == "MAX")
            {
                while(ptr[0] == ' ')
                    ptr++;
                if(ptr[0] != 'z' && ptr[0] != 'Z')
                {
                    //incomplete function
                    expr.clear();
                    return errorCode = 5;
                }
                tmp[tmp_count] = 'Z';
                tmp_count++;
                tmp[tmp_count] = 0;
                ptr++;
            }

            if(String(tmp) == "MINZ" || String(tmp) == "MAXZ")
            {
                if(is_func)
                {
                    // double function
                    return errorCode = 6;
                }
                if(has_comp)
                {
                    // function after equal
                    return errorCode = 7;
                }
                if(expr.count() > 0)
                {
                    // function after variable
                    return errorCode = 8;
                }
                is_func = true;
                term = new Term;
                term->letter = tmp;
                term->type = TYPE_TERM;
                expr.add(term);
                delete term;
                last = "func";
            }
            else if(tmp[1] == 0)
            {
                last = "icog";
                var = tmp;
            }
            else
            {
                //invalid variable, only one charactere allowed
                expr.clear();
                return errorCode = 13;
            }
            continue;
            break;
        case '-':
        case '+':
            if(is_restr)
            {
                // invalid base restriction
                expr.clear();
                return errorCode = 20;
            }
            if(last == "num")
            {
                v = new Variable();
                v->number = num;
                v->letter = "";
                v->id = 0;
                v->type = TYPE_VARIABLE;
                expr.add(v);
                delete v;
            }
            else if(last == "icog")
            {
                // var index not found
                expr.clear();
                return errorCode = 17;
            }
            if(last_sig != 0)
            {
                //error - double sign
                return errorCode = 18;
            }
            last_sig = ptr[0];
            contains_sig = true;
            last = "sig";
            num = 1;
            break;
        case '=':
        case '<':
        case '>':
            if(last == ",")
            {
                // var or constant not found
                expr.clear();
                return errorCode = 19;
            }
            else if(last == "num")
            {
                v = new Variable();
                v->number = num;
                v->letter = "";
                v->id = 0;
                v->type = TYPE_VARIABLE;
                expr.add(v);
                delete v;
            }
            else if(last == "icog")
            {
                // var index not found
                expr.clear();
                return errorCode = 17;
            }
            else if(last == "sig")
            {
                // variable not found
                expr.clear();
                return errorCode = 19;
            }
            tmp_count = 0;
            tmp[tmp_count] = ptr[0];
            tmp_count++;
            tmp[tmp_count] = 0;
            ptr++;
            while(ptr[0] == ' ')
                ptr++;
            if(ptr[0] == '<' || ptr[0] == '>' || (tmp[0] == '=' && ptr[0] == '='))
            {
                //invalid comparation =<  =>  ==
                return errorCode = 9;
            }
            if(ptr[0] == '=')
            {
                tmp[tmp_count] = '=';
                tmp_count++;
                tmp[tmp_count] = 0;
                ptr++;
            }
            //erro - double equal
            if(has_comp || expr.count() == 0)
            {
                expr.clear();
                return errorCode = 1;
            }
            if((is_func && tmp[1] != 0) || (is_func && tmp[0] != '='))
            {
                //invalid function comparation, only = allowed
                return errorCode = 10;
            }
            term = new Term;
            term->letter = tmp;
            term->type = TYPE_TERM;
            expr.add(term);
            delete term;
            has_comp = true;
            last_sig = 0;
            last = "comp";
            continue;
            break;
        case ' ':
            break;
        case ',':
            if(contains_sig)
            {
                // invalid basic restriction
                expr.clear();
                return errorCode = 20;
            }
            if(last == ",")
            {
                // variable not found
                expr.clear();
                return errorCode = 19;
            }
            last = ",";
            term = new Term;
            term->letter = ",";
            term->type = TYPE_TERM;
            expr.add(term);
            delete term;
            is_restr = true;
            break;
        default:
            //invalid charactere
            expr.clear();
            return errorCode = 12;
        }
        ptr++;
    }
    if(last == "num" && has_comp)
    {
        v = new Variable();
        v->number = num;
        v->letter = "";
        v->id = 0;
        v->type = TYPE_VARIABLE;
        expr.add(v);
        delete v;
    }
    else if(last == "sig" || last == "comp" || last == "icog")
    {
        //incomplete expression
        expr.clear();
        return errorCode = 11;
    }
    else if(!has_comp)
    {
        //expression without restriction
        expr.clear();
        return errorCode = 16;
    }
    return errorCode = 0;
}

String Parser::getErrorMsg()
{
    switch(errorCode)
    {
    case 1:
        return "Somente permitido um sinal de comparação";
    case 2:
        return "Divisão por zero não permitida";
    case 3:
        return "Constante inválida faltou o denominador";
    case 4:
        return "Variável ou função objetivo inválida, nome muito grande";
    case 5:
        return "Função objetivo incompleta tente: Max Z, Min Z";
    case 6:
        return "Duas funções objetivo";
    case 7:
        return "A função objetivo não pode ficar depois da igualdade";
    case 8:
        return "A função objetivo não pode ficar depois de uma variável";
    case 9:
        return "Sinal de comparação inválido, tente: <=, >=, <, >, =";
    case 10:
        return "Sinal de restrição inválido para a função objetivo";
    case 11:
        return "Expressão incompleta";
    case 12:
        return "Caractere inválido";
    case 13:
        return "Variável inválida, somente um caractere é permitido";
    case 14:
        return "Número flutuante incompleto";
    case 15:
        return "Índice da variável inválida, permitido somente números inteiros";
    case 16:
        return "Expressão sem restrição";
    case 17:
        return "Índice da variável não informado tente assim: x1, x2";
    case 18:
        return "Falta uma varíavel ou uma constante entre os sinais";
    case 19:
        return "Falta uma varíavel ou uma constante entre o sinal e a comparação";
    case 20:
        return "A restrição das variáveis não pode conter sinal";
    default:
        return "Erro desconhecido";
    }
}

Expression& Parser::getExpression()
{
    return expr;
}

int Parser::getSelStart()
{
    return selstart;
}

