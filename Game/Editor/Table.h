#pragma once

#include <vector>

enum ELineType
{
    Hard = 0,
    Soft,
    Wall,
    Wall2,
    Wall3,
    Wall4,

    MAX
};

class CPoint
{
public:
    float DistanceToPoint(float InX, float InY);
    bool IsOnPoint(float InX, float InY, float InTolerance);

public:
    float X;
    float Y;
    bool bSelected;
};

class CLineSegment
{
public:
    CLineSegment();
    CLineSegment(float InX1, float InY1, float InX2, float InY2, ELineType InLineType);
    float DistanceToLine(float InX, float InY);
    bool IsOnLine(float InX, float InY, float InTolerance = 5.0f);
public:
    CPoint StartPoint;
    CPoint EndPoint;
    ELineType Type;
    bool bSelected;
};

class CLineDefinition
{
public:
    CLineDefinition(): Red(0.0f), Green(0.0f), Blue(0.0f), Name(nullptr)
    {
    } ;
    CLineDefinition(const char* InName, float InR, float InG, float InB);
public:
    float Red;
    float Green;
    float Blue;
    const char* Name;
};

// To hold a set of lines describing the background for the game
class CTable
{
public:
    CTable();
public:
    std::vector<CLineSegment> Lines;
    CLineDefinition LineDefinitions[ELineType::MAX];
};
