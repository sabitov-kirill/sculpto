/*****************************************************************//**
 * \file   common.h
 * \brief  Math common functions, types implementation module.
 * 
 * \author Sabitov Kirill
 * \date   19 June 2022
 *********************************************************************/

 #pragma once

#include <math.h>

namespace scl
{
    /* Atomic types. */
    using i8 = int8_t;
    using u8 = uint8_t;
    using i16 = int16_t;
    using u16 = uint16_t;
    using i32 = int32_t;
    using u32 = uint32_t;
    using i64 = int64_t;
    using u64 = uint64_t;

    namespace math
    {
        /* Math common constants. */
        const float E = 2.7182818284590f;  // e
        const float LOG2E = 1.4426950408889f;  // log2(e)
        const float LOG10E = 0.4342944819032f;  // log10(e)
        const float LN2 = 0.6931471805599f;  // ln(2)
        const float LN10 = 2.3025850929940f;  // ln(10)
        const float PI = 3.1415926535897f;  // pi
        const float PI_2 = 1.5707963267948f;  // pi/2
        const float PI_4 = 0.7853981633974f;  // pi/4
        const float REV_1_PI = 0.3183098861837f;  // 1/pi
        const float REV_2_PI = 0.6366197723675f;  // 2/pi
        const float REV_2_SQRTPI = 1.1283791670955f;  // 2/sqrt(pi)
        const float SQRT2 = 1.4142135623730f;  // sqrt(2)
        const float SQRT1_2 = 0.7071067811865f;  // 1/sqrt(2)

        /**
         * Getting smallest number function.
         *
         * \param Num1, Num2 - numbers to smallest one be chosen from.
         * \return smallest number from pair.
         */
        template <typename T>
        T Min(T Num1, T Num2)
        {
            return Num1 < Num2 ? Num1 : Num2;
        }

        /**
         * Getting biggest number function.
         *
         * \param Num1, Num2 - numbers to biggest one be chosen from.
         * \return smallest number from pair.
         */
        template <typename T>
        T Max(T Num1, T Num2)
        {
            return Num1 > Num2 ? Num1 : Num2;
        }

        /**
         * Number clamping function.
         * If number is higher then upper bound value returned.
         * If number is lower then lower bound value returned.
         *
         * \param Num - number to be clamped.
         * \param Min - lower clipping limit.
         * \param Max - upper clipping limit.
         * \return clamped value.
         */
        template <typename T>
        T Clamp(T Num, T Min = 0, T Max = 1)
        {
            return Num < Min ? Min : Num > Max ? Max : Num;
        }

        /**
         * Linear interpolation between two points function.
         *
         * \param Start - first point value.
         * \param End - second point value.
         * \param Current - interpolation value [0;1].
         * \return value.
         */
        template <typename T>
        T Lerp(T Start, T End, T Current)
        {
            return Start + (End - Start) * Current;
        }

        /**
         * Getting random number function.
         *
         * \param None.
         * \return random number in range [0;1].
         */
        template <typename T>
        T Rnd0()
        {
            return static_cast<T>(rand()) / RAND_MAX;
        }

        /**
         * Getting randow number function.
         *
         * \param min - lower bound of random number.
         * \param Max - upper bound of random number.
         * \return random number in range [Min; Max].
         */
        template <typename T>
        T Rnd(T Min = 0, T Max = 1)
        {
            return Min + Rnd0<T>() * (Max - Min + 1);
        }
    }
}
