/*****************************************************************//**
 * \file   vec4.h
 * \brief  Euclidean 4D vector implementation module.
 *
 * \author Sabitov Kirill
 * \date   19 June 2022
 *********************************************************************/

#pragma once

#include "math_common.h"

namespace scl::math
{
    // Euclidean 4D vector class.
    template <typename T>
    class vec4
    {
    private: /* Vector data. */
        /* Cartesian coordinates. */
        T X { 0 }, Y { 0 }, Z { 0 }, W { 0 };

    public: /* Public coordinates setters and getters.
               On coordinate change vector's length calculation state set to false. */
               /* X vector component setter function. */
        T SetX(T X) { this->X = X; }
        /* Y vector component setter function. */
        T SetY(T Y) { this->Y = Y; }
        /* Z vector component setter function. */
        T SetZ(T Z) { this->Z = Z; }
        /* W vector component setter function. */
        T SetW(T W) { this->W = W; }
        /* X vector component getter function. */
        T GetX() { return X; }
        /* Y vector component getter function. */
        T GetY() { return Y; }
        /* Z vector component getter function. */
        T GetZ() { return Z; }
        /* W vector component getter function. */
        T GetW() { return W; }

    public: /* Vector construcotrs. */
        /* Default construcotr. All coodinates would ve set to zero. */
        vec4() = default;

        /**
         * Vector constructor by one scalar.
         * All corinates would be set to its value.
         *
         * \param A - scalar value of all cordinates.
         */
        vec4(T A) : X(A), Y(A), Z(A), W(A) {}

        /**
         * Vector constructor by three coordinates.
         *
         * \param X, Y - coordiantes of creating vector.
         */
        vec4(T X, T Y, T Z, T W) : X(X), Y(Y), Z(Z), W(W) {}

        /**
         * Vector constructor by 3D vector and additional component.
         *
         * \param V - vector to get X, Y and Z coordinates
         * \param W - addition vector component
         */
        vec4(const vec3<T> &V, T W) : X(V.GetX()), Y(V.GetY()), Z(V.GetZ()), W(W)
        {
        } /* End of 'vec4' function */

        /**
         * Vector constructor by 3D vector and additional component.
         *
         * \param X - addition vector component
         * \param V - vector to get X, Y and Z coordinates
         */
        vec4(T X, const vec3<T> &V) : X(X), Y(V.GetX()), Z(V.GetY()), W(V.GetZ())
        {
        } /* End of 'vec4' function */

        /**
         * Vector copy constructor.
         *
         * \param Other - vector to copy from.
         */
        vec4(const vec4 &Other)
        {
            if (this != &Other)
            {
                X = Other.X;
                Y = Other.Y;
                Z = Other.Z;
                W = Other.W;
            }
            return *this;
        }

        /**
         * Vector assigments operator overloading.
         *
         * \param Other - vector to copy from.
         * \return self reference.
         */
        vec4 &operator=(const vec4 &Other)
        {
            if (this != &Other)
            {
                X = Other.X;
                Y = Other.Y;
                Z = Other.Z;
                W = Other.W;
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
        static vec4 Zero() { return vec4(0); }

        /**
         * Vector with all components set to random value in range [Min;Max] creation function.
         *
         * \param min - lower bound of random number.
         * \param Max - upper bound of random number.
         * \return random number in range [Min; Max].
         */
        static vec4 Rnd(T Min = 0, T Max = 1) { return vec4(Rnd(Min, Max)); }

    public: /* Operators overloading. */
        /**
         * Vector compare function.
         *
         * \param Other - vector to compare with.
         * \return is vectors equal flag.
         */
        bool operator==(const vec4 &Other)
        {
            return X == Other.X && Y == Other.Y && Z == Other.Z && W == Other.W;
        }

        /**
         * Getting negative vector operation.
         *
         * \param None.
         * \return negative vector.
         */
        const vec4 operator-() const
        {
            return vec4(-X, -Y, -Z, -W);
        }

        /**
         * Vectors addition operator overloading.
         *
         * \param Other - vector to add.
         * \return vector with added coordinates.
         */
        const vec4 operator+(const vec4 &Other) const
        {
            return vec4(X + Other.X, Y + Other.Y, Z + Other.Z, W + Other.W);
        }

        /**
         * Vectors addition with assigments operator overlaoding.
         *
         * \param Other - vector to add.
         * \return self reference
         */
        const vec4 &operator+=(const vec4 &Other)
        {
            X += Other.X;
            Y += Other.Y;
            Z += Other.Z;
            W += Other.W;
            return *this;
        }

        /**
         * Vectors subtraction operator overloading.
         *
         * \param Other - vector to subtract.
         * \return vector with subtract coordinates.
         */
        const vec4 operator-(const vec4 &Other) const
        {
            return vec4(X - Other.X, Y - Other.Y, Z - Other.Z, W - Other.W);
        }

        /**
         * Vectors subtraction with assigments operator overlaoding.
         *
         * \param Other - vector to subtract.
         * \return self reference.
         */
        const vec4 &operator-=(const vec4 &Other)
        {
            X -= Other.X;
            Y -= Other.Y;
            Z -= Other.Z;
            W -= Other.W;
            return *this;
        }

        /**
         * Vectors multiplying operator overloading.
         *
         * \param Other - vector to multiply.
         * \return vector with multiplied coordinates.
         */
        const vec4 operator*(const vec4 &Other) const
        {
            return vec4(X * Other.X, Y * Other.Y, Z * Other.Z, W * Other.W);
        }

        /**
         * Vectors multiplying with assigments operator overlaoding.
         *
         * \param Other - vector to multiply.
         * \return self reference.
         */
        const vec4 &operator*=(const vec4 &Other)
        {
            X *= Other.X;
            Y *= Other.Y;
            Z *= Other.Z;
            W *= Other.W;
            return *this;
        }

        /**
         * Vectors dividing operator overloading.
         *
         * \param Other - vector to devide.
         * \return vector with devided coordinates.
         */
        const vec4 operator/(const vec4 &Other) const
        {
            return vec4(X / Other.X, Y / Other.Y, Z / Other.Z, W / Other.W);
        }

        /**
         * Vectors dividing with assigments operator overlaoding.
         *
         * \param Other - vector to devide.
         * \return self reference.
         */
        const vec4 &operator/=(const vec4 &Other)
        {
            X /= Other.X;
            Y /= Other.Y;
            Z /= Other.Z;
            W /= Other.W;
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
