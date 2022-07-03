/*****************************************************************//**
 * \file   matr4.h
 * \brief  Math 4x4 matrix implementation module.
 * 
 * \author Sabitov Kirill
 * \date   19 June 2022
 *********************************************************************/

#pragma once

#include "angle_measure.h"

namespace scl::math
{
    namespace matr3
    {
        /**
         * Calculate determinant of 3x3 Matrix.
         *
         * \param A11-A33 - matrix components.
         * \return matrix determinant.
         */
        template <typename T>
        T Det(T A11, T A12, T A13,
              T A21, T A22, T A23,
              T A31, T A32, T A33)
        {
            return (A11 * A22 * A33 - A11 * A23 * A32 - A12 * A21 * A33 +
                    A12 * A23 * A31 + A13 * A21 * A32 - A13 * A22 * A31);
        }
    };

    /* 4x4 matrix data type class. Used to pass onlt neccecery data to shaders. */
    template<class T>
    class matr4_data
    {
    public:
        /* Matrix data */
        T A[4][4];

        /* Default matrix data constructor. */
        matr4_data() :
            A { {1, 0, 0, 0},
                {0, 1, 0, 0},
                {0, 0, 1, 0},
                {0, 0, 0, 1} } {}

        /**
         * Matrix data constructor by 16 values.
         * 
         * \param A00-A33 - matrx valeus.
         */
        matr4_data(T A00, T A01, T A02, T A03,
                   T A10, T A11, T A12, T A13,
                   T A20, T A21, T A22, T A23,
                   T A30, T A31, T A32, T A33) :
            A { {A00, A01, A02, A03},
                {A10, A11, A12, A13},
                {A20, A21, A22, A23},
                {A30, A31, A32, A33} } {}

        /**
         * Matrix data constructor by 1 value.
         *
         * \param A00 - calue to set to all matrix cells.
         */
        matr4_data(T A00 ) :
            A { {A00, A00, A00, A00},
                {A00, A00, A00, A00},
                {A00, A00, A00, A00},
                {A00, A00, A00, A00} } {}

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

    /* 4x4 matrix class. */
    template <typename T>
    class matr4: public matr4_data<T>
    {
    private:
        mutable T InvA[4][4];            /* Inversed matrix */
        mutable bool IsInverseEvaluated; /* Inverse matrix evaluated flag */

        /**
         * Evaluate inversed matrix function.
         *
         * \param None.
         * \return None.
         */
        void EvaluateInverseMatrix() const
        {
            if (IsInverseEvaluated) return;
            IsInverseEvaluated = true;

            /* build adjoint matrix */
            InvA[0][0] =
                matr3::Det(this->A[1][1], this->A[1][2], this->A[1][3],
                           this->A[2][1], this->A[2][2], this->A[2][3],
                           this->A[3][1], this->A[3][2], this->A[3][3]);
            InvA[1][0] =
                -matr3::Det(this->A[1][0], this->A[1][2], this->A[1][3],
                            this->A[2][0], this->A[2][2], this->A[2][3],
                            this->A[3][0], this->A[3][2], this->A[3][3]);
            InvA[2][0] =
                matr3::Det(this->A[1][0], this->A[1][1], this->A[1][3],
                           this->A[2][0], this->A[2][1], this->A[2][3],
                           this->A[3][0], this->A[3][1], this->A[3][3]);
            InvA[3][0] =
                -matr3::Det(this->A[1][0], this->A[1][1], this->A[1][2],
                            this->A[2][0], this->A[2][1], this->A[2][2],
                            this->A[3][0], this->A[3][1], this->A[3][2]);

            InvA[0][1] =
                -matr3::Det(this->A[0][1], this->A[0][2], this->A[0][3],
                            this->A[2][1], this->A[2][2], this->A[2][3],
                            this->A[3][1], this->A[3][2], this->A[3][3]);
            InvA[1][1] =
                matr3::Det(this->A[0][0], this->A[0][2], this->A[0][3],
                           this->A[2][0], this->A[2][2], this->A[2][3],
                           this->A[3][0], this->A[3][2], this->A[3][3]);
            InvA[2][1] =
                -matr3::Det(this->A[0][0], this->A[0][1], this->A[0][3],
                            this->A[2][0], this->A[2][1], this->A[2][3],
                            this->A[3][0], this->A[3][1], this->A[3][3]);
            InvA[3][1] =
                matr3::Det(this->A[0][0], this->A[0][1], this->A[0][2],
                           this->A[2][0], this->A[2][1], this->A[2][2],
                           this->A[3][0], this->A[3][1], this->A[3][2]);

            InvA[0][2] =
                matr3::Det(this->A[0][1], this->A[0][2], this->A[0][3],
                           this->A[1][1], this->A[1][2], this->A[1][3],
                           this->A[3][1], this->A[3][2], this->A[3][3]);
            InvA[1][2] =
                -matr3::Det(this->A[0][0], this->A[0][2], this->A[0][3],
                            this->A[1][0], this->A[1][2], this->A[1][3],
                            this->A[3][0], this->A[3][2], this->A[3][3]);
            InvA[2][2] =
                matr3::Det(this->A[0][0], this->A[0][1], this->A[0][3],
                           this->A[1][0], this->A[1][1], this->A[1][3],
                           this->A[3][0], this->A[3][1], this->A[3][3]);
            InvA[3][2] =
                -matr3::Det(this->A[0][0], this->A[0][1], this->A[0][2],
                            this->A[1][0], this->A[1][1], this->A[1][2],
                            this->A[3][0], this->A[3][1], this->A[3][2]);

            InvA[0][3] =
                -matr3::Det(this->A[0][1], this->A[0][2], this->A[0][3],
                            this->A[1][1], this->A[1][2], this->A[1][3],
                            this->A[2][1], this->A[2][2], this->A[2][3]);
            InvA[1][3] =
                matr3::Det(this->A[0][0], this->A[0][2], this->A[0][3],
                           this->A[1][0], this->A[1][2], this->A[1][3],
                           this->A[2][0], this->A[2][2], this->A[2][3]);
            InvA[2][3] =
                -matr3::Det(this->A[0][0], this->A[0][1], this->A[0][3],
                            this->A[1][0], this->A[1][1], this->A[1][3],
                            this->A[2][0], this->A[2][1], this->A[2][3]);
            InvA[3][3] =
                matr3::Det(this->A[0][0], this->A[0][1], this->A[0][2],
                           this->A[1][0], this->A[1][1], this->A[1][2],
                           this->A[2][0], this->A[2][1], this->A[2][2]);

            /* divide by determinant */
            T det = Det();
            InvA[0][0] /= det; InvA[1][0] /= det; InvA[2][0] /= det; InvA[3][0] /= det;
            InvA[0][1] /= det; InvA[1][1] /= det; InvA[2][1] /= det; InvA[3][1] /= det;
            InvA[0][2] /= det; InvA[1][2] /= det; InvA[2][2] /= det; InvA[3][2] /= det;
            InvA[0][3] /= det; InvA[1][3] /= det; InvA[2][3] /= det; InvA[3][3] /= det;
        } /* End of 'EvaluateInverseMatrix' function */

    public:
        /**
         * Default matrix constructor.
         * Set identity matrix.
         *
         * \param None.
         */
        matr4() :
            matr4_data<T>(),
            InvA { {0, 0, 0, 0},
                   {0, 0, 0, 0},
                   {0, 0, 0, 0},
                   {0, 0, 0, 0} },
            IsInverseEvaluated(false) {}

        /**
         * Matrix constructor bt 16 values.
         *
         * \param A00-A33 - matrx valeus.
         */
        matr4(T A00, T A01, T A02, T A03,
              T A10, T A11, T A12, T A13,
              T A20, T A21, T A22, T A23,
              T A30, T A31, T A32, T A33) :
            matr4_data<T>(A00, A01, A02, A03,
                          A10, A11, A12, A13,
                          A20, A21, A22, A23,
                          A30, A31, A32, A33),
            InvA { {0, 0, 0, 0},
                   {0, 0, 0, 0},
                   {0, 0, 0, 0},
                   {0, 0, 0, 0} },
            IsInverseEvaluated(false) {}

        matr4(T A[4][4]) :
            InvA { {0, 0, 0, 0},
                   {0, 0, 0, 0},
                   {0, 0, 0, 0},
                   {0, 0, 0, 0} },
            IsInverseEvaluated(false)
        {
            memcpy(this->A, A, 4 * 4 * sizeof(T));
        }

        /**
         * Matrix data constructor by 1 value.
         *
         * \param A00 - calue to set to all matrix cells.
         */
        matr4(T A00) :
            matr4_data<T>(A00),
            InvA { {0, 0, 0, 0},
                   {0, 0, 0, 0},
                   {0, 0, 0, 0},
                   {0, 0, 0, 0} },
            IsInverseEvaluated(false) {}

    public: /* Common matrices creation functinos. */
        /**
         * Identity creation function.
         *
         * \return identity matrix.
         */
        static matr4 Identity()
        {
            return matr4(1, 0, 0, 0,
                         0, 1, 0, 0,
                         0, 0, 1, 0,
                         0, 0, 0, 1);
        }

        /**
         * Translate matrix creation function.
         *
         * \param Transform - translatino vector.
         * \return translation matrix.
         */
        static matr4 Translate(const vec3<T> &Transform)
        {
            T x = Transform.GetX(), y = Transform.GetY(), z = Transform.GetZ();
            return matr4(1, 0, 0, 0,
                         0, 1, 0, 0,
                         0, 0, 1, 0,
                         x, y, z, 1);
        }

        /**
         * Scale matrix creation function.
         *
         * \param S - scale factor.
         * \return scale matrix.
         */
        static matr4 Scale(vec3<T> S)
        {
            T x = S.X, y = S.Y, z = S.Z;
            return matr4(x, 0, 0, 0,
                         0, y, 0, 0,
                         0, 0, z, 0,
                         0, 0, 0, 1);
        }

        /**
         * Rotation matrix creation function.
         *
         * \param Axis - axis to rotate around of.
         * \param Angle - angle in degrees to roate on.
         * \return rotation matrix.
         */
        static matr4 Rotate(vec3<T> Axis, degrees<T> Angle)
        {
            T c = cos((radians<T>)Angle);
            T s = sin((radians<T>)Angle);
            vec3<T> v = Axis.Normalized();

            return matr4(c + v.X * v.X * (1 - c), v.X * v.Y * (1 - c) + v.Z * s, v.X * v.Z * (1 - c) - v.Y * s, 0,
                         v.Y * v.X * (1 - c) - v.Z * s, c + v.Y * v.Y * (1 - c), v.Y * v.Z * (1 - c) + v.X * s, 0,
                         v.Z * v.X * (1 - c) + v.Y * s, v.Z * v.Y * (1 - c) - v.X * s, c + v.Z * v.Z * (1 - c), 0,
                         0, 0, 0, 1);
        }

        /**
         * Rotation around X axis matrix creation function.
         *
         * \param Angle - angle in degrees to roate on.
         * \return rotation matrix.
         */
        static matr4 RotateX(degrees<T> Angle)
        {
            T c = cos((radians<T>)Angle);
            T s = sin((radians<T>)Angle);

            return matr4(1, 0, 0, 0,
                         0, c, s, 0,
                         0, -s, c, 0,
                         0, 0, 0, 1);
        }

        /**
         * Rotation around Y axis matrix creation function.
         *
         * \param Angle - angle in degrees to roate on.
         * \return rotation matrix.
         */
        static matr4 RotateY(degrees<T> Angle)
        {
            T c = cos((radians<T>)Angle);
            T s = sin((radians<T>)Angle);

            return matr4(c, 0, -s, 0,
                         0, 1, 0, 0,
                         s, 0, c, 0,
                         0, 0, 0, 1);
        }

        /**
         * Rotation around Z axis matrix creation function.
         *
         * \param Angle - angle in degrees to roate on.
         * \return rotation matrix.
         */
        static matr4 RotateZ(degrees<T> Angle)
        {
            T c = cos((radians<T>)Angle);
            T s = sin((radians<T>)Angle);

            return matr4(c, s, 0, 0,
                         -s, c, 0, 0,
                         0, 0, 1, 0,
                         0, 0, 0, 1);
        }

        /**
         * Frusum projection matrix creation function.
         *
         * \param Left, Right - frustrum view cone left and right sides position
         * \param Bottom, Top - frustrum view cone bottom and top sides position
         * \param Near, Far - frustrum view cone near and far planes distance.
         * \return None.
         */
        static matr4 Frustum(T Left, T Right, T Bottom, T Top, T Near, T Far)
        {
            return matr4(2 * Near / (Right - Left), 0, 0, 0,
                         0, 2 * Near / (Top - Bottom), 0, 0,
                         (Right + Left) / (Right - Left), (Top + Bottom) / (Top - Bottom), -(Far + Near) / (Far - Near), -1,
                         0, 0, -2 * Near * Far / (Far - Near), 0);
        }

        /**
         * Ortho projection matrix creation function.
         *
         * \param Left, Right - view cube left and right sides position
         * \param Bottom, Top - view cube bottom and top sides position
         * \param Near, Far - view cube near and far planes distance.
         * \return ortho matrix.
         */
        static matr4 Ortho(T Left, T Right, T Bottom, T Top, T Near, T Far)
        {
            return matr4(2 / (Right - Left), 0, 0, 0,
                         0, 2 / (Top - Bottom), 0, 0,
                         0, 0, -2 / (Far - Near), 0,
                         -(Right + Left) / (Right - Left), -(Top + Bottom) / (Top - Bottom), -(Near + Far) / (Far - Near), 1);
        }

        /**
         * Viewer coordinate system transformation.
         *
         * \param Location - viewer position in global coordinate system.
         * \param At - viewer looking at point.
         * \param Up - viewer up direction point.
         * \return view matrix.
         */
        static matr4 View(vec3<T> Location, vec3<T> At, vec3<T> Up)
        {
            vec3<T> d = (At - Location).Normalized();
            vec3<T> r = d.Cross(Up).Normalized();
            vec3<T> u = r.Cross(d);

            return matr4(r.X, u.X, -d.X, 0,
                         r.Y, u.Y, -d.Y, 0,
                         r.Z, u.Z, -d.Z, 0,
                         -Location.Dot(r), -Location.Dot(u), Location.Dot(d), 1);
        }

    public: /* Matrices operations. */
        /**
         * Matrix lerp function.
         *
         * \param Start, End - matrices to interpolate.
         * \param Current - interpolation value [0;1].
         * \return interpolated matrix.
         */
        static matr4 Lerp(const matr4 &Start, const matr4 &End, T Current)
        {
            matr4 M;

            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    M.A[i][j] = ::scl::math::Lerp(Start.A[i][j],
                                                  End.A[i][j],
                                                  Current);

            return M;
        }

        /**
         * Transpose matrix function.
         *
         * \return result matrix.
         */
        matr4 Transpose() const
        {
            return matr4(this->A[0][0], this->A[1][0], this->A[2][0], this->A[3][0],
                         this->A[0][1], this->A[1][1], this->A[2][1], this->A[3][1],
                         this->A[0][2], this->A[1][2], this->A[2][2], this->A[3][2],
                         this->A[0][3], this->A[1][3], this->A[2][3], this->A[3][3]);
        }

        /**
         * Mulpiply matrices function.
         *
         * \param M - matrix to multyply.
         * \return result matrix.
         */
        matr4 operator*(const matr4 &M) const
        {
            matr4 r;
            int k;

            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 4; j++)
                    for (k = 0, r.A[i][j] = 0; k < 4; k++)
                        r.A[i][j] += this->A[i][k] * M.A[k][j];
            return r;
        }

        /**
         * Calculate matrix determinant.
         *
         * \param None.
         * \return matrix determinant.
         */
        T Det() const
        {
            return
                this->A[0][0] * matr3::Det(this->A[1][1], this->A[1][2], this->A[1][3],
                                     this->A[2][1], this->A[2][2], this->A[2][3],
                                     this->A[3][1], this->A[3][2], this->A[3][3]) +
                -this->A[0][1] * matr3::Det(this->A[1][0], this->A[1][2], this->A[1][3],
                                      this->A[2][0], this->A[2][2], this->A[2][3],
                                      this->A[3][0], this->A[3][2], this->A[3][3]) +
                this->A[0][2] * matr3::Det(this->A[1][0], this->A[1][1], this->A[1][3],
                                     this->A[2][0], this->A[2][1], this->A[2][3],
                                     this->A[3][0], this->A[3][1], this->A[3][3]) +
                -this->A[0][3] * matr3::Det(this->A[1][0], this->A[1][1], this->A[1][2],
                                      this->A[2][0], this->A[2][1], this->A[2][2],
                                      this->A[3][0], this->A[3][1], this->A[3][2]);
        }

        /**
         * Evaluate inverse matrix.
         *
         * \param None.
         * \return None.
         */
        matr4 Inverse() const
        {
            EvaluateInverseMatrix();
            return matr4(InvA);
        }

        /**
         * Transform point position.
         *
         * \param V - vectors to transform.
         * \return None.
         */
        vec3<T> TransformPoint(const vec3<T> &V) const
        {
            return vec3<T>(V.X * this->A[0][0] + V.Y * this->A[1][0] + V.Z * this->A[2][0] + this->A[3][0],
                           V.X * this->A[0][1] + V.Y * this->A[1][1] + V.Z * this->A[2][1] + this->A[3][1],
                           V.X * this->A[0][2] + V.Y * this->A[1][2] + V.Z * this->A[2][2] + this->A[3][2]);
        }

        /**
         * Vector linear transformation.
         *
         * \param V - vectors to transform.
         * \return None.
         */
        vec3<T> TransformVector(const vec3<T> &V) const
        {
            return vec3<T>(V.X * this->A[0][0] + V.Y * this->A[1][0] + V.Z * this->A[2][0],
                              V.X * this->A[0][1] + V.Y * this->A[1][1] + V.Z * this->A[2][1],
                              V.X * this->A[0][2] + V.Y * this->A[1][2] + V.Z * this->A[2][2]);
        }

        /**
         * Multiply matrix and vector.
         *
         * \param V - vectors to transform.
         * \return None.
         */
        vec3<T> Transform4x4(const vec3<T> &V) const
        {
            float w = V.X * this->A[0][3] + V.Y * this->A[1][3] + V.Z * this->A[2][3] + this->A[3][3];

            return vec3<T>((V.X * this->A[0][0] + V.Y * this->A[1][0] + V.Z * this->A[2][0] + this->A[3][0]) / w,
                           (V.X * this->A[0][1] + V.Y * this->A[1][1] + V.Z * this->A[2][1] + this->A[3][1]) / w,
                           (V.X * this->A[0][2] + V.Y * this->A[1][2] + V.Z * this->A[2][2] + this->A[3][2]) / w);
        }
    };
}