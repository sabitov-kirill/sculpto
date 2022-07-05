/*****************************************************************//**
 * \file   matr4.h
 * \brief  Math 3x3 matrix implementation module.
 *
 * \author Sabitov Kirill
 * \date   19 June 2022
 *********************************************************************/

#pragma once

#include "angle_measure.h"

namespace scl::math
{
    /* 4x4 matrix declaration. */
    template<class T> class matr4;

    /* 3x3 matrix data type class. Used to pass onlt neccecery data to shaders. */
    template<class T>
    class matr3_data
    {
    public:
        /* Matrix data */
        T A[3][3];

        /* Default matrix data constructor. */
        matr3_data() :
            A { {1, 0, 0},
                {0, 1, 0},
                {0, 0, 1} } {}

        /**
         * Matrix data constructor by 9 values.
         *
         * \param A00-A22 - matrx valeus.
         */
        matr3_data(T A00, T A01, T A02,
                   T A10, T A11, T A12,
                   T A20, T A21, T A22) :
            A { {A00, A01, A02},
                {A10, A11, A12},
                {A20, A21, A22} } {}

        /**
         * Matrix data constructor by 1 value.
         *
         * \param A00 - calue to set to all matrix cells.
         */
        matr3_data(T A00) :
            A { {A00, A00, A00 },
                {A00, A00, A00 },
                {A00, A00, A00 } } {}

        /**
         * Getting pointer to first component of natrix operator.
         * Need to pass vector to shader.
         *
         * \return pointer to first component of matrix.
         */
        operator T *()
        {
            return A[0];
        }
    };

    /* 3x3 matrix class. */
    template <typename T>
    class matr3: public matr3_data<T>
    {
    public:
        /**
         * Default matrix constructor.
         * Set identity matrix.
         *
         * \param None.
         */
        matr3() : matr4_data<T>() {}

        /**
         * Matrix constructor bt 16 values.
         *
         * \param A00-A33 - matrx valeus.
         */
        matr3(T A00, T A01, T A02,
              T A10, T A11, T A12,
              T A20, T A21, T A22) :
            matr3_data<T>(A00, A01, A02,
                          A10, A11, A12,
                          A20, A21, A22) {}

        /**
         * Matrix contructor by array of values.
         * 
         * \param A - array of values to set in matrix
         */
        matr3(T A[3][3]) { memcpy(this->A, A, 3 * 3 * sizeof(T)); }

        /**
         * Matrix constructor by 1 value.
         *
         * \param A00 - calue to set to all matrix cells.
         */
        matr3(T A00) : matr3_data<T>(A00) {}

        matr3(const matr4<T> Matr4x4) :
            matr3_data<T>(Matr4x4.A[0][0], Matr4x4.A[0][1], Matr4x4.A[0][2],
                          Matr4x4.A[1][0], Matr4x4.A[1][1], Matr4x4.A[1][2],
                          Matr4x4.A[2][0], Matr4x4.A[2][1], Matr4x4.A[2][2]) {}

        /**
         * Calculate determinant of 3x3 Matrix.
         *
         * \param A11-A33 - matrix components.
         * \return matrix determinant.
         */
        T Det()
        {
            return (this->A[0][0] * this->A[1][1] * this->A[2][2] -
                    this->A[0][0] * this->A[1][2] * this->A[2][1] -
                    this->A[0][1] * this->A[1][0] * this->A[2][2] +
                    this->A[0][1] * this->A[1][2] * this->A[2][0] +
                    this->A[0][2] * this->A[1][0] * this->A[2][1] -
                    this->A[0][2] * this->A[1][1] * this->A[2][0]);
        }

        /**
         * Calculate determinant of 3x3 Matrix.
         *
         * \param A11-A33 - matrix components.
         * \return matrix determinant.
         */
        static T Det(T A11, T A12, T A13,
                     T A21, T A22, T A23,
                     T A31, T A32, T A33)
        {
            return (A11 * A22 * A33 - A11 * A23 * A32 - A12 * A21 * A33 +
                    A12 * A23 * A31 + A13 * A21 * A32 - A13 * A22 * A31);
        }
    };
}
