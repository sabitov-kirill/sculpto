/*****************************************************************//**
 * \file   vec3.h
 * \brief  Euclidean 3D vector implementation module.
 *
 * \author Sabitov Kirill
 * \date   19 June 2022
 *********************************************************************/

#pragma once

#include "math_common.h"

namespace scl::math
{
    // Euclidean 3D vector class.
    template <typename T>
    class vec3
    {
        /* 4x4 Matrix friend class forward declaration. */
        template<typename T> friend class matr4;

    private: /* Vector data. */
        /* Cartesian coordinates. */
        T X { 0 }, Y { 0 }, Z { 0 };

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
        /* Z vector component setter function. */
        T SetZ(T Z) { IsLengthCalculated = false; this->Z = Z; }
        /* X vector component getter function. */
        T GetX() { return X; }
        /* Y vector component getter function. */
        T GetY() { return Y; }
        /* Z vector component getter function. */
        T GetZ() { return Z; }

    public: /* Vector construcotrs. */
        /* Default construcotr. All coodinates would ve set to zero. */
        vec3() = default;

        /**
         * Vector constructor by one scalar.
         * All corinates would be set to its value.
         *
         * \param A - scalar value of all cordinates.
         */
        vec3(T A) : X(A), Y(A), Z(A) {}

        /**
         * Vector constructor by three coordinates.
         *
         * \param X, Y - coordiantes of creating vector.
         */
        vec3(T X, T Y, T Z) : X(X), Y(Y), Z(Z) {}

        /**
         * Vector constructor by 2D vector and additional component.
         * 
         * \param V - vector to get X and Y coordinates
         * \param Z - addition vector component
         */
        vec3(vec2<T> V, T Z) : X(V.X), Y(V.Y), Z(Z) {}

        /**
         * Vector constructor by 2D vector and additional component.
         *
         * \param X - addition vector component
         * \param V - vector to get Y and Z coordinates
         */
        vec3(T X, vec2<T> V) : X(X), Y(V.X), Z(V.Z) {}

        /**
         * Vector copy constructor.
         *
         * \param Other - vector to copy from.
         */
        vec3(const vec3 &Other)
        {
            if (this != &Other)
            {
                X = Other.X;
                Y = Other.Y;
                Z = Other.Z;

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
        vec3 &operator=(const vec3 &Other)
        {
            if (this != &Other)
            {
                X = Other.X;
                Y = Other.Y;
                Z = Other.Z;

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
        static vec3 Zero() { return vec3(0); }

        /**
         * Vector with all components set to random value in range [Min;Max] creation function.
         *
         * \param min - lower bound of random number.
         * \param Max - upper bound of random number.
         * \return random number in range [Min; Max].
         */
        static vec3 Rnd(T Min = 0, T Max = 1) { return vec3(Rnd(Min, Max)); }

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
                length2 = X * X + Y * Y + Z * Z, IsLength2Calculated = true;
            return length2;
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
        T Distacnce(const vec3 &Other) const
        {
            return (*this - Other).Length();
        }

        /**
         * Normalized vector getting function.
         *
         * \param None.
         * \return normalized vector.
         */
        vec3 Normalized() const
        {
            return *this / Length();
        }

        /**
         * Vectors dot product function.
         *
         * \param Other - vector to multiply
         * \return value of vectors dot product.
         */
        T Dot(const vec3 &Other)
        {
            return sqrt(X * Other.X + Y * Other.Y + Z * Other.Z);
        }

        /**
         * Vectors cross product function.
         *
         * \param Other - vector to multiply
         * \return cross product resulting rector.
         */
        vec3 Cross(const vec3 &Other)
        {
            return vec3(Y * Other.Z - Other.Y * Z,
                        Z * Other.X - Other.Z * X,
                        X * Other.Y - Other.X * Y);
        }

    public: /* Operators overloading. */
        /**
         * Vector compare function.
         * 
         * \param Other - vector to compare with.
         * \return is vectors equal flag.
         */
        bool operator==(const vec3 &Other)
        {
            return X == Other.X && Y == Other.Y && Z == Other.Z;
        }

        /**
         * Getting negative vector operation.
         *
         * \param None.
         * \return negative vector.
         */
        const vec3 operator-() const
        {
            return vec3(-X, -Y, -Z);
        }

        /**
         * Vectors addition operator overloading.
         *
         * \param Other - vector to add.
         * \return vector with added coordinates.
         */
        const vec3 operator+(const vec3 &Other) const
        {
            return vec3(X + Other.X, Y + Other.Y, Z + Other.Z);
        }

        /**
         * Vectors addition with assigments operator overlaoding.
         *
         * \param Other - vector to add.
         * \return self reference
         */
        const vec3 &operator+=(const vec3 &Other)
        {
            X += Other.X;
            Y += Other.Y;
            Z += Other.Z;
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
        const vec3 operator-(const vec3 &Other) const
        {
            return vec3(X - Other.X, Y - Other.Y, Z - Other.Z);
        }

        /**
         * Vectors subtraction with assigments operator overlaoding.
         *
         * \param Other - vector to subtract.
         * \return self reference.
         */
        const vec3 &operator-=(const vec3 &Other)
        {
            X -= Other.X;
            Y -= Other.Y;
            Z -= Other.Z;
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
        const vec3 operator*(const vec3 &Other) const
        {
            return vec3(X * Other.X, Y * Other.Y, Z * Other.Z);
        }

        /**
         * Vectors multiplying with assigments operator overlaoding.
         *
         * \param Other - vector to multiply.
         * \return self reference.
         */
        const vec3 &operator*=(const vec3 &Other)
        {
            X *= Other.X;
            Y *= Other.Y;
            Z *= Other.Z;
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
        const vec3 operator/(const vec3 &Other) const
        {
            return vec3(X / Other.X, Y / Other.Y, Z / Other.Z);
        }

        /**
         * Vectors dividing with assigments operator overlaoding.
         *
         * \param Other - vector to devide.
         * \return self reference.
         */
        const vec3 &operator/=(const vec3 &Other)
        {
            X /= Other.X;
            Y /= Other.Y;
            Z /= Other.Z;
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
