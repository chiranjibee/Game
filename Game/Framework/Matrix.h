#pragma once

#include <vector>

#include "Vector2D.h"

namespace Internal
{
    struct SMatrix
    {
        float M11, M12, M13;
        float M21, M22, M23;
        float M31, M32, M33;

        SMatrix()
        {
            M11 = 0.0f; M12 = 0.0f; M13 = 0.0f;
            M21 = 0.0f; M22 = 0.0f; M23 = 0.0f;
            M31 = 0.0f; M32 = 0.0f; M33 = 0.0f;
        }
    };

}

struct SMatrix3X3
{
    SMatrix3X3()
    {
        Identity();
    }

    // Creates the identity matrix
    void Identity();

    // Creates the translation matrix
    void Translate(float InX, float InY);

    // Creates the scale matrix
    void Scale(float InX, float InY);

    // Creates the rotation matrix, InRot in radians
    void Rotate(float InRot);

    // Creates the rotation matrix, given the forward and the side vectors
    void Rotate(const SVector2D& InFwd, const SVector2D& InSide);

    // Apply transformation matrix to a point
    void TransfromVector2D(SVector2D& InPt);

    void TransfromVector2DArray(std::vector<SVector2D>& InPts);

    void SetM11(float InVal) { Mat.M11 = InVal; }
    void SetM12(float InVal) { Mat.M12 = InVal; }
    void SetM13(float InVal) { Mat.M13 = InVal; }
    void SetM21(float InVal) { Mat.M21 = InVal; }
    void SetM22(float InVal) { Mat.M22 = InVal; }
    void SetM23(float InVal) { Mat.M23 = InVal; }
    void SetM31(float InVal) { Mat.M31 = InVal; }
    void SetM32(float InVal) { Mat.M32 = InVal; }
    void SetM33(float InVal) { Mat.M33 = InVal; }

private:

    Internal::SMatrix Mat;

    // Multiplies this matrix with InMat
    void MatrixMultiply(Internal::SMatrix& InMat);
};
