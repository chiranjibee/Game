#pragma once

#include <cfloat>
#include <fstream>

struct SVector2D
{
	float X;
	float Y;

	SVector2D() : X(0.0f), Y(0.0f)
	{}

	SVector2D(float InX, float InY) : X(InX), Y(InY)
	{}

	// Zeros out the X and Y values
	void SetZero()
	{
		X = 0.0f;
		Y = 0.0f;
	}

	// Returns true if both X and Y are zero
	bool IsZero() const
	{
		return (X * X + Y * Y) < FLT_MIN;
	}

	// Returns length of vector
	float GetLength() const;

	// Returns squared length of vector
	float GetLengthSq() const;

	// Normalizes the vector
	void Normalize();

	// Calculates the dot product
	float Dot(const SVector2D& InVec) const;

	// Returns +1 if InVec is clockwise of this vector or -1 if anticlockwise
	int Sign(const SVector2D& InVec) const;

	// Returns the vector perpendicular to this
	SVector2D GetPerpendicular() const;

	// Adjusts X and Y so that the length of the vector doesn't exceed max
	void Truncate(float InMax);

	// Returns the distance between this vector and the one passed in
	float Distance(const SVector2D& InVec) const;

	// Returns the squared distance between the vectors
	float DistanceSq(const SVector2D& InVec) const;

	// Given the normalized vector, this vector is reflected across the normal
	void Reflect(const SVector2D& InNorm);

	// Returns the reverse of this vector
	SVector2D GetReverse() const;

	/////////////////////////////////////////////////////////////////////////
	// Operator overloads

	const SVector2D& operator+=(const SVector2D& InVec);

	const SVector2D& operator-=(const SVector2D& InVec);

	const SVector2D& operator*=(const float& InVal);

	const SVector2D& operator/=(const float& InVal);

	bool operator==(const SVector2D& InVec) const
	{
		return fabsf(X - InVec.X) < FLT_EPSILON && fabsf(Y - InVec.Y) < FLT_EPSILON;
	}

	bool operator!=(const SVector2D& InVec) const
	{
		return (X != InVec.X) || (Y != InVec.Y);
	}

	/////////////////////////////////////////////////////////////////////////
	// Static functions

	// Returns a zero vector
	static SVector2D Zero();

	// Returns a vector with each component 1.0f
	static SVector2D Identity();

	static SVector2D Normalize(const SVector2D& InVec);

	static float Dot(const SVector2D& InA, const SVector2D& InB);

	static float Distance(const SVector2D& InA, const SVector2D& InB);

	static float DistanceSq(const SVector2D& InA, const SVector2D& InB);
};

SVector2D operator*(const SVector2D& InVec, float InVal);

SVector2D operator*(float InVal, const SVector2D& InVec);

SVector2D operator-(const SVector2D& InA, const SVector2D& InB);

SVector2D operator+(const SVector2D& InA, const SVector2D& InB);

SVector2D operator/(const SVector2D& InVec, float InVal);

std::ostream& operator<<(std::ostream& OStream, const SVector2D& InVec);
std::ifstream& operator>>(std::ifstream& IStream, SVector2D& InVec);

