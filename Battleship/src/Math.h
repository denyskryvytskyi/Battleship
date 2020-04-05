#ifndef _MATH_H
#define _MATH_H

namespace math
{
    template<class T>
    class Matrix
    {
    private:
        int m_rows;
        int m_cols;
        T** m_matrix;

    public:
        Matrix();
        Matrix(int rows, int cols);
        ~Matrix();
        //
        T& at(int row, int col);
        //
        int rows();
        int cols();
    };

    template<class T>
    Matrix<T>::Matrix()
    {
    }

    template<class T>
    Matrix<T>::Matrix(int rows, int cols)
        :m_rows(rows), m_cols(cols)
    {
        m_matrix = new T * [m_rows];
        for (int i = 0; i < m_rows; i++)
            m_matrix[i] = new T[m_cols];
    }

    template<class T>
    Matrix<T>::~Matrix()
    {
        for (int i = 0; i < m_rows; i++)
            delete[] m_matrix[i];

        delete[] m_matrix;
    }

    template<class T>
    T& Matrix<T>::at(int row, int col)
    {
        return m_matrix[row][col];
    }

    template<class T>
    int Matrix<T>::rows()
    {
        return m_rows;
    }

    template<class T>
    int Matrix<T>::cols()
    {
        return m_cols;
    }

}
#endif // !_MATH_H