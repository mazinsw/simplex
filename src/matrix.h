#ifndef _MATRIX_H_
#define _MATRIX_H_
#include "number.h"

class Matrix
{
private:
    Number ** matrix;
    int rows, columns;
public:
    Matrix();
    Matrix(const Matrix& other);
    Matrix(int rows, int columns);
    ~Matrix();
    
    Matrix& operator=(const Matrix& rhs);
    Number& operator()(int i, int j) const;
    void set(int rows, int columns);

    inline int getRows() const
    {
        return rows;
    }

    inline int getColumns() const
    {
        return columns;
    }
};

#endif
