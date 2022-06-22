/*****************************************************************//**
 * \file   angle_measure.h
 * \brief  Angle measures types implementatino module.
 * 
 * \author Sabitov Kirill
 * \date   19 June 2022
 *********************************************************************/

#pragma once

#include "math_common.h"

namespace scl::math
{
    /* Angle measure types forward declaration. */
    template <typename T> struct radians;
    template <typename T> struct degrees;

    /* Radians angle type class. */
    template <typename T>
    struct radians
    {
    public:
        T Value { 0 }; /* Angle value in radians. */

        /* Radians angle default constructor. */
        radians() = default;

        /**
         * Radians angle constructor by value.
         * 
         * \param Value - angle value.
         */
        radians(T Value) : Value(Value) {}

        /**
         * Radians copy constructor.
         * 
         * \param Other
         * \return 
         */
        const radians &operator=(const radians &Other) {
            if (this != &Other) Value = Other.Value;
            return *this;
        }

        /**
         * Radians angle compare function.
         *
         * \param Other - radians angle to compare with.
         * \return is radians degrees equal flag.
         */
        bool operator==(const radians &Other)
        {
            return Value == Other.Value;
        }

        /**
         * Getting negative radians angle operation.
         *
         * \param None.
         * \return negative radians angle.
         */
        const radians operator-() const
        {
            return radians(-Value);
        }

        /**
         * Radians degrees addition operator overloading.
         *
         * \param Other - radians angle to add.
         * \return radians angle with added coordinates.
         */
        const radians operator+(const radians &Other) const
        {
            return radians(Value + Other.Value);
        }

        /**
         * Radians degrees addition with assigments operator overlaoding.
         *
         * \param Other - radians angle to add.
         * \return self reference
         */
        const radians &operator+=(const radians &Other)
        {
            Value += Other.Value;
            return *this;
        }

        /**
         * Radians degrees subtraction operator overloading.
         *
         * \param Other - radians angle to subtract.
         * \return radians angle with subtract coordinates.
         */
        const radians operator-(const radians &Other) const
        {
            return radians(Value - Other.Value);
        }

        /**
         * Radians degrees subtraction with assigments operator overlaoding.
         *
         * \param Other - radians angle to subtract.
         * \return self reference.
         */
        const radians &operator-=(const radians &Other)
        {
            Value -= Other.Value;
            return *this;
        }

        /**
         * Radians degrees multiplying operator overloading.
         *
         * \param Other - radians angle to multiply.
         * \return radians angle with multiplied coordinates.
         */
        const radians operator*(const radians &Other) const
        {
            return radians(Value * Other.Value);
        }

        /**
         * Radians degrees multiplying with assigments operator overlaoding.
         *
         * \param Other - radians angle to multiply.
         * \return self reference.
         */
        const radians &operator*=(const radians &Other)
        {
            Value *= Other.Value;
            return *this;
        }

        /**
         * Radians degrees dividing operator overloading.
         *
         * \param Other - radians angle to devide.
         * \return radians angle with devided coordinates.
         */
        const radians operator/(const radians &Other) const
        {
            return radians(Value / Other.Value);
        }

        /**
         * Radians degrees dividing with assigments operator overlaoding.
         *
         * \param Other - radians angle to devide.
         * \return self reference.
         */
        const radians &operator/=(const radians &Other)
        {
            Value /= Other.Value;
            return *this;
        }

        /**
         * СCast to degree measure from radians to degrees.
         * 
         * \return angle in degrees.
         */
        operator degrees<T>() { return degrees<T>(Value * 180.0 / PI); }

        /**
         * Cast to scalar type.
         *
         * \return angle value.
         */
        operator T() { return Value; }
    };

    /* Radians angle type class. */
    template <typename T>
    struct degrees
    {
    public:
        T Value { 0 }; /* Angle value in degrees. */

        /* Degrees angle default constructor. */
        degrees() = default;

        /**
         * Degrees angle constructor by value.
         *
         * \param Value - angle value.
         */
        degrees(T Value) : Value(Value) {}

        /**
         * Degrees copy constructor.
         *
         * \param Other
         * \return
         */
        const degrees &operator=(const degrees &Other) {
            if (this != &Other) Value = Other.Value;
            return *this;
        }

        /**
         * Degrees angle compare function.
         *
         * \param Other - degrees angle to compare with.
         * \return is degrees degrees equal flag.
         */
        bool operator==(const degrees &Other)
        {
            return Value == Other.Value;
        }

        /**
         * Getting negative degrees angle operation.
         *
         * \param None.
         * \return negative degrees angle.
         */
        const degrees operator-() const
        {
            return degrees(-Value);
        }

        /**
         * Degrees degrees addition operator overloading.
         *
         * \param Other - degrees angle to add.
         * \return degrees angle with added coordinates.
         */
        const degrees operator+(const degrees &Other) const
        {
            return degrees(Value + Other.Value);
        }

        /**
         * Degrees degrees addition with assigments operator overlaoding.
         *
         * \param Other - degrees angle to add.
         * \return self reference
         */
        const degrees &operator+=(const degrees &Other)
        {
            Value += Other.Value;
            return *this;
        }

        /**
         * Degrees degrees subtraction operator overloading.
         *
         * \param Other - degrees angle to subtract.
         * \return degrees angle with subtract coordinates.
         */
        const degrees operator-(const degrees &Other) const
        {
            return degrees(Value - Other.Value);
        }

        /**
         * Degrees degrees subtraction with assigments operator overlaoding.
         *
         * \param Other - degrees angle to subtract.
         * \return self reference.
         */
        const degrees &operator-=(const degrees &Other)
        {
            Value -= Other.Value;
            return *this;
        }

        /**
         * Degrees degrees multiplying operator overloading.
         *
         * \param Other - degrees angle to multiply.
         * \return degrees angle with multiplied coordinates.
         */
        const degrees operator*(const degrees &Other) const
        {
            return degrees(Value * Other.Value);
        }

        /**
         * Degrees degrees multiplying with assigments operator overlaoding.
         *
         * \param Other - degrees angle to multiply.
         * \return self reference.
         */
        const degrees &operator*=(const degrees &Other)
        {
            Value *= Other.Value;
            return *this;
        }

        /**
         * Degrees degrees dividing operator overloading.
         *
         * \param Other - degrees angle to devide.
         * \return degrees angle with devided coordinates.
         */
        const degrees operator/(const degrees &Other) const
        {
            return degrees(Value / Other.Value);
        }

        /**
         * Degrees degrees dividing with assigments operator overlaoding.
         *
         * \param Other - degrees angle to devide.
         * \return self reference.
         */
        const degrees &operator/=(const degrees &Other)
        {
            Value /= Other.Value;
            return *this;
        }

        /**
         * Cast to degree measure from degrees to degrees.
         *
         * \return angle in degrees.
         */
        operator radians<T>() { return radians<T>(Value * PI / 180); }

        /**
         * Cast to scalar type.
         *
         * \return angle value.
         */
        operator T() { return Value; }
    };
}
