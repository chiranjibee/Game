#pragma once

class CPoint;

enum EEditorState
{
    Edit,
    Add,
    Save,
};

namespace Editor
{
    void DrawBoxAroundPoint(const CPoint& InPoint, float InSize);

    EEditorState UpdateMoveLine(float InX, float InY);
    EEditorState UpdateAddLine(float InX, float InR);
    EEditorState Save(const char* InFilename);
    void Load(const char* InFilename);
}
