#include <iostream>
#include <iomanip>
#include <random>
#include <math.h>
#include "Task2.h"

using namespace std;

Matrix::Matrix(const int size) : matrixSize(size), matrixCapacity(size* size)
{
    array = new double[matrixCapacity];
}

/*
 * Перегрузка операторов [][]
 */
Matrix::Iterator::Iterator(double* row) : currentRow(row)
{
}

double& Matrix::Iterator::operator[](size_t i)
{
    return *(currentRow + i);
}

Matrix::Iterator Matrix::operator[](size_t i)
{
    return Iterator(array + i * matrixSize);
}
/*
 * Расчет определителя
 */
Matrix Matrix::getModifiedMatrix(Matrix& arr, const size_t item)
{
    Matrix newArray(arr.size() - 1);
    size_t currentRow = 0;

    for (size_t i = 0; i < arr.size(); ++i)
    {
        if (item == i)
        {
            currentRow = i;
            continue;
        }

        for (size_t j = 0; j < newArray.size(); ++j)
        {
            newArray[currentRow][j] = arr[i][j + 1];
        }

        ++currentRow;
    }

    return newArray;
}

double Matrix::calculateDeterminant(Matrix& arr, const size_t size)
{
    double determinant = 0;

    if (size == 1)
    {
        determinant = arr[0][0];
    }
    else if (size == 2)
    {
        determinant = arr[0][0] * arr[1][1] + arr[1][0] * arr[0][1];
    }
    else
    {
        int sign = 1;

        for (size_t i = 0; i < size; ++i)
        {
            Matrix newArray = getModifiedMatrix(arr, i);
            determinant += sign * arr[i][0] * calculateDeterminant(newArray, size - 1);
            sign = -sign;
        }
    }

    return determinant;
}

void Matrix::fillRandom()
{
    random_device rd;
    mt19937 mersenne(rd());                                   // x32-битное число
    uniform_real_distribution<double> urd(-10.0, 10.0);       // диапазон вещественных чисел от -10 до 10

    for (size_t i = 0; i < matrixCapacity; ++i)
    {
        *(array + i) = round(urd(mersenne) * 10) / 10;
    }

    matrixDeterminant = calculateDeterminant((*this), matrixSize);
}

void Matrix::print() const
{
    cout << '[';

    for (size_t i = 0; i < matrixCapacity;)
    {
        cout << setw(5) << *(array + i++);

        if (!(i % matrixSize))
        {
            cout << ']' << endl;

            if (i < matrixCapacity)
            {
                cout << '[';
            }
        }
        else
        {
            cout << ',';
        }
    }
}

size_t Matrix::size() const
{
    return matrixSize;
}

double Matrix::determinant() const
{
    return matrixDeterminant;
}

Matrix::~Matrix()
{
    delete[] array;
}

void task_2()
{
    Matrix m(7);
    m.fillRandom();
    m.print();
    cout << "Определитель матрицы: " << setprecision(10) << m.determinant() << endl;    // манипулятор для вывода без экспоненты
}