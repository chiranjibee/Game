#include "Vector2D.h"

float SVector2D::GetLength() const
{
	return sqrtf(X * X + Y * Y);
}

float SVector2D::GetLengthSq() const
{
	return (X * X + Y * Y);
}

void SVector2D::Normalize()
{
	const float VecLength = this->GetLength();
	if (VecLength > FLT_EPSILON)
	{
		this->X /= VecLength;
		this->Y /= VecLength;
	}
}

float SVector2D::Dot(const SVector2D& InVec) const
{
	return X * InVec.X + Y * InVec.Y;
}

int SVector2D::Sign(const SVector2D& InVec) const
{
	if (Y * InVec.X > X * InVec.Y)
	{
		return -1;
	}
	else
	{
		return 1;
	}
}

SVector2D SVector2D::GetPerpendicular() const
{
	return SVector2D(-Y, X);
}

void SVector2D::Truncate(float InMax)
{
	if (this->GetLength() > InMax)
	{
		this->Normalize();
		*this *= InMax;
	}
}

float SVector2D::Distance(const SVector2D& InVec) const
{
	const float XSeparation = InVec.X - X;
	const float YSeparation = InVec.Y - Y;

	return sqrtf(XSeparation * XSeparation + YSeparation * YSeparation);
}

float SVector2D::DistanceSq(const SVector2D& InVec) const
{
	const float XSeparation = InVec.X - X;
	const float YSeparation = InVec.Y - Y;

	return (XSeparation * XSeparation + YSeparation * YSeparation);
}

void SVector2D::Reflect(const SVector2D& InNorm)
{
	*this += 2.0f * this->Dot(InNorm) * InNorm.GetReverse();
}

SVector2D SVector2D::GetReverse() const
{
	return SVector2D(-this->X, -this->Y);
}

//////////////////////////////////////////////////////////////////////////
// Operator overloads

const SVector2D& SVector2D::operator+=(const SVector2D& InVec)
{
	X += InVec.X;
	Y += InVec.Y;

	return *this;
}

const SVector2D& SVector2D::operator-=(const SVector2D& InVec)
{
	X -= InVec.X;
	Y -= InVec.Y;

	return *this;
}

const SVector2D& SVector2D::operator*=(const float& InVal)
{
	X *= InVal;
	Y *= InVal;

	return *this;
}

const SVector2D& SVector2D::operator/=(const float& InVal)
{
	X /= InVal;
	Y /= InVal;

	return *this;
}

SVector2D SVector2D::Zero()
{
	return SVector2D(0.0f, 0.0f);
}

SVector2D SVector2D::Identity()
{
	return SVector2D(1.0f, 1.0f);
}

SVector2D SVector2D::Normalize(const SVector2D& InVec)
{
	SVector2D NewVec = InVec;

	const float VecLength = NewVec.GetLength();
	if (VecLength > FLT_EPSILON)
	{
		NewVec.X /= VecLength;
		NewVec.Y /= VecLength;
	}

	return NewVec;
}

float SVector2D::Dot(const SVector2D& InA, const SVector2D& InB)
{
	return InA.X * InB.X + InA.Y * InB.Y;
}

float SVector2D::Distance(const SVector2D& InA, const SVector2D& InB)
{
	const float XSeparation = InB.X - InA.X;
	const float YSeparation = InB.Y - InA.Y;

	return sqrtf(XSeparation * XSeparation + YSeparation * YSeparation);
}

float SVector2D::DistanceSq(const SVector2D& InA, const SVector2D& InB)
{
	const float XSeparation = InB.X - InA.X;
	const float YSeparation = InB.Y - InA.Y;

	return (XSeparation * XSeparation + YSeparation * YSeparation);
}

SVector2D operator*(const SVector2D& InVec, float InVal)
{
	SVector2D Result = InVec;
	Result *= InVal;

	return Result;
}

SVector2D operator*(float InVal, const SVector2D& InVec)
{
	SVector2D Result = InVec;
	Result *= InVal;

	return Result;
}

SVector2D operator-(const SVector2D& InA, const SVector2D& InB)
{
	SVector2D Result = InA;
	Result.X -= InB.X;
	Result.Y -= InB.Y;

	return Result;
}

SVector2D operator+(const SVector2D& InA, const SVector2D& InB)
{
	SVector2D Result = InA;
	Result.X += InB.X;
	Result.Y += InB.Y;

	return Result;
}

SVector2D operator/(const SVector2D& InVec, float InVal)
{
	SVector2D Result = InVec;
	Result.X /= InVal;
	Result.Y /= InVal;

	return Result;
}

std::ostream& operator<<(std::ostream& OStream, const SVector2D& InVec)
{
	OStream << " " << InVec.X << " " << InVec.Y;

	return OStream;
}

std::ifstream& operator>>(std::ifstream& IStream, SVector2D& InVec)
{
	IStream >> InVec.X >> InVec.Y;

	return IStream;
}
