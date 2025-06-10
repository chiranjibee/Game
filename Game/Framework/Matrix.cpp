#include "Matrix.h"

void SMatrix3X3::Identity()
{
	Mat.M11 = 1.0f; Mat.M12 = 0.0f; Mat.M13 = 0.0f;
	Mat.M21 = 0.0f; Mat.M22 = 1.0f; Mat.M23 = 0.0f;
	Mat.M31 = 0.0f; Mat.M32 = 0.0f; Mat.M33 = 1.0f;
}

void SMatrix3X3::Translate(float InX, float InY)
{
	Internal::SMatrix MatTemp;

	MatTemp.M11 = 1.0f; MatTemp.M12 = 0.0f; MatTemp.M13 = 0.0f;
	MatTemp.M21 = 0.0f; MatTemp.M22 = 1.0f; MatTemp.M23 = 0.0f;
	MatTemp.M31 = InX; MatTemp.M32 = InY; MatTemp.M33 = 1.0f;

	MatrixMultiply(MatTemp);

}

void SMatrix3X3::Scale(float InX, float InY)
{
	Internal::SMatrix MatTemp;

	MatTemp.M11 = InX; MatTemp.M12 = 0.0f; MatTemp.M13 = 0.0f;
	MatTemp.M21 = 0.0f; MatTemp.M22 = InY; MatTemp.M23 = 0.0f;
	MatTemp.M31 = 0.0f; MatTemp.M32 = 0.0f; MatTemp.M33 = 1.0f;

	MatrixMultiply(MatTemp);
}

void SMatrix3X3::Rotate(float InRot)
{
	Internal::SMatrix MatTemp;

	const float SinVal = sin(InRot);
	const float CosVal = cos(InRot);

	MatTemp.M11 = CosVal; MatTemp.M12 = SinVal; MatTemp.M13 = 0.0f;
	MatTemp.M21 = -SinVal; MatTemp.M22 = CosVal; MatTemp.M23 = 0.0f;
	MatTemp.M31 = 0.0f; MatTemp.M32 = 0.0f; MatTemp.M33 = 1.0f;

	MatrixMultiply(MatTemp);
}

void SMatrix3X3::Rotate(const SVector2D& InFwd, const SVector2D& InSide)
{
	Internal::SMatrix MatTemp;

	MatTemp.M11 = InFwd.X; MatTemp.M12 = InFwd.Y; MatTemp.M13 = 0.0f;
	MatTemp.M21 = InSide.X; MatTemp.M22 = InSide.Y; MatTemp.M23 = 0.0f;
	MatTemp.M31 = 0.0f; MatTemp.M32 = 0.0f; MatTemp.M33 = 1.0f;

	MatrixMultiply(MatTemp);
}

void SMatrix3X3::TransfromVector2D(SVector2D& InPt)
{
	const float XTemp = (Mat.M11 * InPt.X) + (Mat.M21 * InPt.Y) + Mat.M31;
	const float YTemp = (Mat.M12 * InPt.X) + (Mat.M22 * InPt.Y) + Mat.M32;

	InPt.X = XTemp;
	InPt.Y = YTemp;
}

void SMatrix3X3::TransfromVector2DArray(std::vector<SVector2D>& InPts)
{
	for (unsigned int i = 0; i < InPts.size(); ++i)
	{
		const float XTemp = (Mat.M11 * InPts[i].X) + (Mat.M21 * InPts[i].Y) + Mat.M31;
		const float YTemp = (Mat.M12 * InPts[i].X) + (Mat.M22 * InPts[i].Y) + Mat.M32;

		InPts[i].X = XTemp;
		InPts[i].Y = YTemp;
	}
}

void SMatrix3X3::MatrixMultiply(Internal::SMatrix& InMat)
{
	Internal::SMatrix MatTemp;

	// First row
	MatTemp.M11 = (Mat.M11 * InMat.M11) + (Mat.M12 * InMat.M21) + (Mat.M13 * InMat.M31);
	MatTemp.M12 = (Mat.M11 * InMat.M12) + (Mat.M12 * InMat.M22) + (Mat.M13 * InMat.M32);
	MatTemp.M13 = (Mat.M11 * InMat.M13) + (Mat.M12 * InMat.M23) + (Mat.M13 * InMat.M33);

	// Second
	MatTemp.M21 = (Mat.M21 * InMat.M11) + (Mat.M22 * InMat.M21) + (Mat.M23 * InMat.M31);
	MatTemp.M22 = (Mat.M21 * InMat.M12) + (Mat.M22 * InMat.M22) + (Mat.M23 * InMat.M32);
	MatTemp.M23 = (Mat.M21 * InMat.M13) + (Mat.M22 * InMat.M23) + (Mat.M23 * InMat.M33);

	// Third
	MatTemp.M31 = (Mat.M31 * InMat.M11) + (Mat.M32 * InMat.M21) + (Mat.M33 * InMat.M31);
	MatTemp.M32 = (Mat.M31 * InMat.M12) + (Mat.M32 * InMat.M22) + (Mat.M33 * InMat.M32);
	MatTemp.M33 = (Mat.M31 * InMat.M13) + (Mat.M32 * InMat.M23) + (Mat.M33 * InMat.M33);

	Mat = MatTemp;
}

