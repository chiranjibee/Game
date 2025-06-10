#pragma once

#include <cassert>
#include <random>

#include "..\Framework\Matrix.h"

//////////////////////////////////////////////////////////////////////////
// Utility

// Clamps the first argument between the second two
template <class T, class U, class V>
inline void Clamp(T& Arg, const U& MinVal, const V& MaxVal)
{
    assert((static_cast<float>(MinVal) < static_cast<float>(MaxVal)) && "<Clamp>MaxVal < MinVal!");

    if (Arg < static_cast<T>(MinVal))
    {
        Arg = static_cast<T>(MinVal);
    }

    if (Arg > static_cast<T>(MaxVal))
    {
        Arg = static_cast<T>(MaxVal);
    }
}

// Safe delete
template<class T>
void SafeDelete(T** ppT)
{
    if (*ppT)
    {
        delete (*ppT);
        *ppT = nullptr;
    }
}

// Local to World Space transformation
inline std::vector<SVector2D> GetWorldTransform(std::vector<SVector2D>& InPts, const SVector2D& InPos, float InRot, const SVector2D& InScl)
{
	std::vector<SVector2D> TransVec2Ds = InPts;

	SMatrix3X3 TransMat;
	if (InScl.X != 1.0f || InScl.Y != 1.0f)
	{
		TransMat.Scale(InScl.X, InScl.Y);
	}

	TransMat.Rotate(InRot);

	TransMat.Translate(InPos.X, InPos.Y);

	TransMat.TransfromVector2DArray(TransVec2Ds);

	return TransVec2Ds;
}

inline std::vector<SVector2D> GetWorldTransform(std::vector<SVector2D>& InPts, const SVector2D& InPos, const SVector2D& InHeading, const SVector2D& InSide, const SVector2D& InScl)
{
	std::vector<SVector2D> TransVec2Ds = InPts;

	SMatrix3X3 TransMat;
	if (InScl.X != 1.0f || InScl.Y != 1.0f)
	{
		TransMat.Scale(InScl.X, InScl.Y);
	}

	TransMat.Rotate(InHeading, InSide);

	TransMat.Translate(InPos.X, InPos.Y);

	TransMat.TransfromVector2DArray(TransVec2Ds);

	return TransVec2Ds;
}

inline float GetRandomValue(float InMinVal, float InMaxVal)
{
	std::random_device RandDevice;
	std::mt19937 SeedVal(RandDevice());
	std::uniform_real_distribution<> ValueDistribution(InMinVal, InMaxVal);

	const float RandomValue = static_cast<float>(ValueDistribution(SeedVal));
	
	return RandomValue;
}

inline bool LineSegIntersection(SVector2D InA, SVector2D InB, SVector2D InC, SVector2D InD)
{
	const float RTop = ((InA.Y - InC.Y) * (InD.X - InC.X)) - ((InA.X - InC.X) *  (InD.Y - InC.Y));
	const float RBot = ((InB.X - InA.X) * (InD.Y - InC.Y)) - ((InB.Y - InA.Y) *  (InD.X - InC.X));

	const float STop = ((InA.Y - InC.Y) * (InB.X - InA.X)) - ((InA.X - InC.X) *  (InB.Y - InA.Y));
	const float SBot = ((InB.X - InA.X) * (InD.Y - InC.Y)) - ((InB.Y - InA.Y) *  (InD.X - InC.X));

	// Lines are parallel
	if ((RBot == 0.0f) || (SBot == 0.0f))
	{
		return false;
	}

	const float R = RTop / RBot;
	const float S = STop / SBot;

	if ((R > 0.0f) && (R < 1.0f) && (S > 0.0f) && (S < 1.0f))
	{
		return true;
	}
	else
	{
		return false;
	}
}

inline bool ObjectIntersection(const std::vector<SVector2D>& InObjA, const std::vector<SVector2D>& InObjB)
{
	for (unsigned int i = 0; i < InObjA.size() - 1; ++i)
	{
		for (unsigned int j = 0; j < InObjB.size() - 1; ++j)
		{
			if (LineSegIntersection(InObjB[j], InObjB[j + 1], InObjA[i], InObjA[i + 1]))
			{
				return true;
			}
		}
	}

	return false;
}
