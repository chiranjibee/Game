#include "Table.h"

CTable::CTable()
{
	LineDefinitions[ELineType::Hard] = CLineDefinition("Hard", .8f, .6f, .2f);
	LineDefinitions[ELineType::Soft] = CLineDefinition("Soft", .4f, .3f, .1f);
	LineDefinitions[ELineType::Wall] = CLineDefinition("Wall", .2f, .2f, .6f);
	LineDefinitions[ELineType::Wall2] = CLineDefinition("Wall2", .5f, .9f, .1f);
	LineDefinitions[ELineType::Wall3] = CLineDefinition("Wall3", .8f, .3f, .9f);
	LineDefinitions[ELineType::Wall4] = CLineDefinition("Wall4", .7f, .4f, .4f);
}

CLineSegment::CLineSegment() : CLineSegment(0.0f, 0.0f, 0.0f, 0.0f, ELineType::Wall3)
{
}

CLineSegment::CLineSegment(float InX1, float InY1, float InX2, float InY2, ELineType InLineType)
{
	StartPoint.X = InX1;
	StartPoint.Y = InY1;
	StartPoint.bSelected = false;
	EndPoint.X = InX2;
	EndPoint.Y = InY2;
	EndPoint.bSelected = false;
	Type = InLineType;
	bSelected = false;
}


float CLineSegment::DistanceToLine(float InX, float InY)
{
	const float x0 = StartPoint.X;
	const float x1 = EndPoint.X;
	const float y0 = StartPoint.Y;
	const float y1 = EndPoint.Y;

	const float dx = x1 - x0;
	const float dy = y1 - y0;

	const float linelengthSquared = dx*dx + dy*dy;
	float param = ((InX - x0)*dx + (InY - y0)*dy) / linelengthSquared;
	if (param > 1)
		param = 1;
	if (param < 0)
		param = 0;

	const float nearestX = x0 + param*dx;
	const float nearestY = y0 + param*dy;

	const float distX = InX - nearestX;
	const float distY = InY - nearestY;
	const float distance = static_cast<float>(sqrt((distX*distX) + (distY*distY)));
	return distance;
}

/* float CLineSegment::DistanceToInfiniteLine(float x, float y)
{
	// See https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line
	const float x0 = m_start.m_x;
	const float x1 = m_end.m_x;
	const float y0 = m_start.m_y;
	const float y1 = m_end.m_y;

	float numerator = abs((y1 - y0)*x - (x1 - x0)*y + x1*y0 - y1*x0);
	float denomenator = sqrt((y1 - y0)*(y1 - y0) + (x1 - x0)*(x1 - x0));
	float distance = numerator / denomenator;
	
	return distance;
} */

bool CLineSegment::IsOnLine(float InX, float InY, float InTolerance)
{
	return DistanceToLine(InX,InY) < InTolerance;
}

float CPoint::DistanceToPoint(float InX, float InY)
{
	const float dX = InX - X;
	const float dY = InY - Y;
	const float distance = static_cast<float>(sqrt(dX*dX + dY*dY));

	return distance;
}

bool CPoint::IsOnPoint(float InX, float InY, float InTolerance)
{
	return DistanceToPoint(InX, InY) < InTolerance;
}

CLineDefinition::CLineDefinition(const char * InName, float InR, float InG, float InB)
{
	Name = InName;
	Red = InR;
	Green = InG;
	//Red = InB;
	Blue = InB;
}

