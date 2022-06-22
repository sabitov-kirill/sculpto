/*****************************************************************//**
 * \file   vec2.h
 * \brief  Euclidean 2D vector implementation module.
 * 
 * \author Sabitov Kirill
 * \date   19 June 2022
 *********************************************************************/

#pragma once

#include "math_common.h"

namespace scl::math
{
    // Euclidean 2D vector class.
    template <typename T>
    class vec2
    {
    private: /* Vector data. */
        /* Cartesian coordinates. */
        T X { 0 }, Y { 0 };

        mutable T length { 0 };  /* Vector length. */
        mutable T length2 { 0 }; /* Vector length squared. */
        /* Vector length calculation state.
           Need to memoization and prevention of new calculations. */
        mutable bool IsLength2Calculated { false }, IsLengthCalculated { false };

    public: /* Public coordinates setters and getters.
               On coordinate change vector's length calculation state set to false. */
        /* X vector component setter function. */
        T SetX(T X) { IsLengthCalculated = false; this->X = X; }
        /* Y vector component setter function. */
        T SetY(T Y) { IsLengthCalculated = false; this->Y = Y; }
        /* X vector component getter function. */
        T GetX() { return X; }
        /* Y vector component getter function. */
        T GetY() { return Y; }

    public: /* Vector construcotrs. */
        /* Default construcotr. All coodinates would ve set to zero. */
        vec2() = default;

        /**
         * Vector constructor by one scalar.
         * All corinates would be set to its value.
         * 
         * \param A - scalar value of all cordinates.
         */
        vec2(T A) : X(A), Y(A) {}

        /**
         * Vector constructor by three coordinates.
         * 
         * \param X, Y - coordiantes of creating vector.
         */
        vec2(T X, T Y) : X(X), Y(Y) {}

        /**
         * Vector copy constructor.
         * 
         * \param Other - vector to copy from.
         */
        vec2(const vec2 &Other)
        {
            if (this != &Other)
            {
                X = Other.X;
                Y = Other.Y;

                IsLength2Calculated = Other.IsLength2Calculated;
                if (IsLength2Calculated) length2 = Other.length2;
                IsLengthCalculated = Other.IsLengthCalculated;
                if (IsLengthCalculated) length = Other.length;
            }
            return *this;
        }

        /**
         * Vector assigments operator overloading.
         * 
         * \param Other - vector to copy from.
         * \return self reference.
         */
        vec2 &operator=(const vec2 &Other)
        {
            if (this != &Other)
            {
                X = Other.X;
                Y = Other.Y;

                IsLength2Calculated = Other.IsLength2Calculated;
                if (IsLength2Calculated) length2 = Other.length2;
                IsLengthCalculated = Other.IsLengthCalculated;
                if (IsLengthCalculated) length = Other.length;
            }
            return *this;
        }

    public: /* Common vectors creation functinos. */
        /**
         * Vector with all components set to 0 creation function.
         * 
         * \param None.
         * \return zero vector.
         */
        static vec2 Zero() { return vec2(0); }

        /**
         * Vector with all components set to random value in range [Min;Max] creation function.
         *
         * \param min - lower bound of random number.
         * \param Max - upper bound of random number.
         * \return random number in range [Min; Max].
         */
        static vec2 Rnd(T Min = 0, T Max = 1) { return vec2(Rnd(Min, Max)); }

    public: /* Vector methods. */
        /**
         * Vector squared length getting function.
         *
         * \param None.
         * \return length of vector.
         */
        T Length2() const
        {
            if (!IsLength2Calculated)
                length2 = X * X + Y * Y, IsLength2Calculated = true;
            return Length2;
        }

        /**
         * Vector length getting function.
         * 
         * \param None.
         * \return length of vector.
         */
        T Length() const
        {
            if (!IsLengthCalculated)
                length = sqrt(Length2()), IsLengthCalculated = true;
            return length;
        }

        /**
         * Distance between two vectors getting function.
         * 
         * \param Other - vector to get distance to.
         * \return distance between vectors.
         */
        T Distacnce(const vec2 &Other) const
        {
            return (*this - Other).Length();
        }

        /**
         * Normalized vector getting function.
         *
         * \param None.
         * \return normalized vector.
         */
        vec2 Normalized() const
        {
            return *this / Length();
        }

        /**
         * Vectors dot product function.
         * 
         * \param Other - vector to multiply
         * \return value of vectors dot product.
         */
        T Dot(const vec2 &Other)
        {
            return sqrt(X * Other.X + Y * Other.Y);
        }

    public: /* Operators overloading. */
        /**
         * Vector compare function.
         *
         * \param Other - vector to compare with.
         * \return is vectors equal flag.
         */
        bool operator==(const vec2 &Other)
        {
            return X == Other.X && Y == Other.Y;
        }

        /**
         * Getting negative vector operation.
         * 
         * \param None.
         * \return negative vector.
         */
        const vec2 operator-() const
        {
            return vec2(-X, -Y);
        }

        /**
         * Vectors addition operator overloading.
         * 
         * \param Other - vector to add.
         * \return vector with added coordinates.
         */
        const vec2 operator+(const vec2 &Other) const
        {
            return vec2(X + Other.X, Y + Other.Y);
        }

        /**
         * Vectors addition with assigments operator overlaoding.
         * 
         * \param Other - vector to add.
         * \return self reference
         */
        const vec2 &operator+=(const vec2 &Other)
        {
            X += Other.X;
            Y += Other.Y;
            IsLengthCalculated = Other.IsLengthCalculated;
            IsLength2Calculated = Other.IsLength2Calculated;
            return *this;
        }

        /**
         * Vectors subtraction operator overloading.
         *
         * \param Other - vector to subtract.
         * \return vector with subtract coordinates.
         */
        const vec2 operator-(const vec2 &Other) const
        {
            return vec2(X - Other.X, Y - Other.Y);
        }

        /**
         * Vectors subtraction with assigments operator overlaoding.
         *
         * \param Other - vector to subtract.
         * \return self reference.
         */
        const vec2 &operator-=(const vec2 &Other)
        {
            X -= Other.X;
            Y -= Other.Y;
            IsLengthCalculated = Other.IsLengthCalculated;
            IsLength2Calculated = Other.IsLength2Calculated;
            return *this;
        }

        /**
         * Vectors multiplying operator overloading.
         *
         * \param Other - vector to multiply.
         * \return vector with multiplied coordinates.
         */
        const vec2 operator*(const vec2 &Other) const
        {
            return vec2(X * Other.X, Y * Other.Y);
        }

        /**
         * Vectors multiplying with assigments operator overlaoding.
         *
         * \param Other - vector to multiply.
         * \return self reference.
         */
        const vec2 &operator*=(const vec2 &Other)
        {
            X *= Other.X;
            Y *= Other.Y;
            IsLengthCalculated = Other.IsLengthCalculated;
            IsLength2Calculated = Other.IsLength2Calculated;
            return *this;
        }

        /**
         * Vectors dividing operator overloading.
         *
         * \param Other - vector to devide.
         * \return vector with devided coordinates.
         */
        const vec2 operator/(const vec2 &Other) const
        {
            return vec2(X / Other.X, Y / Other.Y);
        }

        /**
         * Vectors dividing with assigments operator overlaoding.
         *
         * \param Other - vector to devide.
         * \return self reference.
         */
        const vec2 &operator/=(const vec2 &Other)
        {
            X /= Other.X;
            Y /= Other.Y;
            IsLengthCalculated = Other.IsLengthCalculated;
            IsLength2Calculated = Other.IsLength2Calculated;
            return *this;
        }

        /**
         * Getting pointer to first component of vector operator.
         * Need to pass vector to shader.
         * 
         * \return pointer to first component of vector.
         */
        operator T *()
        {
            return &X;
        }
    };
}
