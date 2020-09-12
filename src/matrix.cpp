#include "matrix.h"

Matrix::Matrix()
{
    int i;

    rows = 1;
    columns = 1;
    matrix = new Number*[rows]();
    for(i = 0; i < rows; i++)
        matrix[i] = new Number[columns]();
}

Matrix::Matrix(const Matrix& other)
{
    int i, j;

    this->rows = other.rows;
    this->columns = other.columns;
    matrix = new Number*[rows]();
    for(i = 0; i < rows; i++)
        matrix[i] = new Number[columns]();
    for(i = 0; i < rows; i++)
    {
        for(j = 0; j < columns; j++)
            matrix[i][j] = other.matrix[i][j];
    }
}

Matrix::Matrix(int rows, int columns)
{
    int i;

    if(rows < 1)
        rows = 1;
    if(columns < 1)
        columns = 1;
    this->rows = rows;
    this->columns = columns;
    matrix = new Number*[rows]();
    for(i = 0; i < rows; i++)
        matrix[i] = new Number[columns]();
}

Matrix::~Matrix()
{
    int i;

    for(i = 0; i < rows; i++)
        delete[] matrix[i];
    delete[] matrix;
}

Matrix& Matrix::operator=(const Matrix& rhs)
{
    int i, j;

    set(rhs.rows, rhs.columns);
    for(i = 0; i < rows; i++)
    {
        for(j = 0; j < columns; j++)
            matrix[i][j] = rhs.matrix[i][j];
    }
    return *this;
}

void Matrix::set(int rows, int columns)
{
    int i;

    for(i = 0; i < this->rows; i++)
        delete[] matrix[i];
    delete[] matrix;
    if(rows < 1)
        rows = 1;
    if(columns < 1)
        columns = 1;
    this->rows = rows;
    this->columns = columns;
    matrix = new Number*[rows]();
    for(i = 0; i < rows; i++)
        matrix[i] = new Number[columns]();
}

Number& Matrix::operator()(int i, int j) const
{
    return matrix[i][j];
}

