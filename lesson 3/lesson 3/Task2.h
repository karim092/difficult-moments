pragma once

#include <cstddef>

/*
 * Т.к. определитель (детрминант) вычисляется только для квадратной матрицы,
 * то класс инициализируется одним положительным целочисленным значением.
 */
    class Matrix
{
private:
    const size_t matrixSize;
    const size_t matrixCapacity;
    double* array;
    double matrixDeterminant{ 0.0 };

    Matrix getModifiedMatrix(Matrix& arr, const size_t item);
    double calculateDeterminant(Matrix& arr, const size_t size);
public:
    Matrix(const int size);
    void fillRandom();                 // заполнить массив случайными значениями
    void print() const;                // распечатать массив
    size_t size() const;               // получить размер массива
    double determinant() const;        // получить определитель матрицы

    class Iterator
    {
    private:
        double* currentRow;
    public:
        Iterator(double* row);
        double& operator[](size_t i);
    };

    Iterator operator[](size_t i);

    ~Matrix();
};

void task_2();