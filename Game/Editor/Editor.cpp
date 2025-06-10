#include "Editor.h"

#include <cassert>
#include <fstream>

#include "../App/App.h"
#include "Table.h"

CTable* MyTable;
ELineType DefaultLineType = ELineType::Hard;
const char* ControlsMessage = "Controls\n"
	"Left-Click - Select Line, drag points\n"
	"Shift - Hold to disable snap to point\n"
	"Shift - Hold to drag just this point\n"
	"Ctrl - Create new line with each click\n"
	"Ctrl-S Save data\n"
	"Delete - delete selected line\n"
	"1-6 change type of selected line";
const char* StatusMessage = ControlsMessage;
const char* DefaultFileName = "table.txt";
const int VERSION = 1;
const float PICK_TOLERANCE = 5.0f;
const float SNAP_TOLERANCE = 8.0f;
const float POINT_EPSILON = 0.01f;

//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void EditorInit()
{
	MyTable = new CTable;
	Editor::Load(DefaultFileName);


}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void EditorUpdate(float DeltaTime)
{
	static EEditorState EdState = EEditorState::Edit;

	// Controls
	// Left-Click - Select Line, drag points
	// Shift - Hold to disable snap to point
	// Shift - Hold to drag just this point
	// Ctrl - Create new line with each click
	// Ctrl-S Save data
	// Delete - delete selected line
	// 1-6 change type of selected line

	float XCoord, YCoord;
	App::GetMousePos(XCoord, YCoord);

	switch (EdState)
	{
	case EEditorState::Edit:
		EdState = Editor::UpdateMoveLine(XCoord, YCoord);
		break;
	case EEditorState::Add:
		EdState = Editor::UpdateAddLine(XCoord, YCoord);
		break;
	case EEditorState::Save:
		EdState = Editor::Save(DefaultFileName);
		break;
	default:
		assert(false);
	}
}

EEditorState Editor::UpdateMoveLine(float InX, float InY)
{
	if (App::IsKeyPressed(VK_CONTROL))
	{
		if (App::IsKeyPressed('S'))
		{
			return EEditorState::Save;
		}
		return EEditorState::Add;
	}

	CLineSegment* SelectedLine = nullptr;
	for (auto& Line : MyTable->Lines)
	{
		if (Line.bSelected)
		{
			SelectedLine = &Line;
		}
	}

	if (App::IsKeyPressed(VK_DELETE))
	{
		MyTable->Lines.erase(
			std::remove_if(
				MyTable->Lines.begin(), MyTable->Lines.end(),
				[](CLineSegment x) {return x.bSelected;}),
			MyTable->Lines.end());
		StatusMessage = ControlsMessage;
		return EEditorState::Edit;
	}

	if (SelectedLine)
	{
		for (int i = 0; i < ELineType::MAX; ++i)
		{
			if (App::IsKeyPressed('0' + i))
			{
				SelectedLine->Type = static_cast<ELineType>(i);
				SelectedLine->bSelected = false;
				SelectedLine = nullptr;
				DefaultLineType = static_cast<ELineType>(i);
			}
		}
	}

	if (App::IsKeyPressed(VK_LBUTTON))
	{
		const CPoint* SelectedPoint = nullptr;
		if (SelectedLine)
		{
			if (SelectedLine->StartPoint.bSelected)
			{
				SelectedPoint = &SelectedLine->StartPoint;
			}
			else if (SelectedLine->EndPoint.bSelected)
			{
				SelectedPoint = &SelectedLine->EndPoint;
			}
		}

		// We have a selected point while we are dragging
		if (SelectedLine && SelectedPoint)
		{
			float SnappedX = InX;
			float SnappedY = InY;

			// Snap moving point to close points (on any other line)
			if (!App::IsKeyPressed(VK_LSHIFT))
			{
				for (auto& TestLine : MyTable->Lines)
				{
					if (!TestLine.bSelected)
					{
						if (!TestLine.StartPoint.bSelected && TestLine.StartPoint.IsOnPoint(InX, InY, SNAP_TOLERANCE))
						{
							SnappedX = TestLine.StartPoint.X;
							SnappedY = TestLine.StartPoint.Y;
							continue;
						}
						if (!TestLine.EndPoint.bSelected &&TestLine.EndPoint.IsOnPoint(InX, InY, SNAP_TOLERANCE))
						{
							SnappedX = TestLine.EndPoint.X;
							SnappedY = TestLine.EndPoint.Y;
							continue;
						}
					}
				}
			}

			// move all selected points
			for (auto& Line : MyTable->Lines)
			{
				if (Line.StartPoint.bSelected)
				{
					Line.StartPoint.X = SnappedX;
					Line.StartPoint.Y = SnappedY;
				}
				if (Line.EndPoint.bSelected)
				{
					Line.EndPoint.X = SnappedX;
					Line.EndPoint.Y = SnappedY;
				}
				StatusMessage = ControlsMessage;
			}
		}
		else if (SelectedLine)
		{
			// no point selected, select a point close to the cursor
			if (!App::IsKeyPressed(VK_LSHIFT))
			{
				// select all points that are close
				for (auto& Line : MyTable->Lines)
				{
					if (Line.StartPoint.IsOnPoint(InX, InY, PICK_TOLERANCE))
					{
						Line.StartPoint.bSelected = true;
						if (&Line == SelectedLine)
						{
							SelectedPoint = &SelectedLine->StartPoint;
						}
					}
					else if (Line.EndPoint.IsOnPoint(InX, InY, PICK_TOLERANCE))
					{
						Line.EndPoint.bSelected = true;
						if (&Line == SelectedLine)
						{
							SelectedPoint = &SelectedLine->EndPoint;
						}
					}
				}
			}
			else
			{
				// Shift stops dragging both points
				if (SelectedLine->StartPoint.IsOnPoint(InX, InY, PICK_TOLERANCE))
				{
					SelectedLine->StartPoint.bSelected = true;
					SelectedPoint = &SelectedLine->StartPoint;
				}
				else if (SelectedLine->EndPoint.IsOnPoint(InX, InY, PICK_TOLERANCE))
				{
					SelectedLine->EndPoint.bSelected = true;
					SelectedPoint = &SelectedLine->EndPoint;
				}
			}
		}

		if (!SelectedPoint)
		{
			// Select closest line
			CLineSegment* ClosestLine = nullptr;
			float ClosestDist = 10.0f;
			for (auto& Line : MyTable->Lines)
			{
				const float Distance = Line.DistanceToLine(InX, InY);
				if (Distance < ClosestDist)
				{
					ClosestDist = Distance;
					ClosestLine = &Line;
				}
			}
			if (ClosestLine)
			{
				for (auto& Line : MyTable->Lines)
				{
					Line.bSelected = false;
				}
				ClosestLine->bSelected = true;
			}
		}
	}
	else
	{
		// Remove all point selection on mouse-up
		for (auto& Line : MyTable->Lines)
		{
			if (Line.StartPoint.bSelected || Line.EndPoint.bSelected)
			{
				Line.StartPoint.bSelected = false;
				Line.EndPoint.bSelected = false;
			}
		}
	}
	return EEditorState::Edit;
}

//------------------------------------------------

EEditorState Editor::UpdateAddLine(float InX, float InR)
{
	CLineSegment* SelectedLine = nullptr;
	CPoint* SelectedPoint = nullptr;
	for (auto& Line : MyTable->Lines)
	{
		if (Line.bSelected)
		{
			SelectedLine = &Line;
		}

		if (Line.StartPoint.bSelected)
		{
			SelectedPoint = &Line.StartPoint;
		}
		else if (Line.EndPoint.bSelected)
		{
			SelectedPoint = &Line.EndPoint;
		}
	}

	// Find a snapped point
	float SnappedX = InX;
	float SnappedY = InR;
	for (auto& TestLine : MyTable->Lines)
	{
		if (!TestLine.bSelected)
		{
			if (!TestLine.StartPoint.bSelected && TestLine.StartPoint.IsOnPoint(InX, InR, SNAP_TOLERANCE))
			{
				SnappedX = TestLine.StartPoint.X;
				SnappedY = TestLine.StartPoint.Y;
				continue;
			}
			if (!TestLine.EndPoint.bSelected &&TestLine.EndPoint.IsOnPoint(InX, InR, SNAP_TOLERANCE))
			{
				SnappedX = TestLine.EndPoint.X;
				SnappedY = TestLine.EndPoint.Y;
				continue;
			}
		}
	}

	if (!App::IsKeyPressed(VK_CONTROL) || App::IsKeyPressed('S'))
	{
		// When releasing CTRL, the user may have a 'dangling' zero length line
		if (SelectedPoint)
		{
			CLineSegment* ZeroLengthLine = &MyTable->Lines.back();
			if (ZeroLengthLine->StartPoint.IsOnPoint(ZeroLengthLine->EndPoint.X, ZeroLengthLine->EndPoint.Y, POINT_EPSILON))
			{
				MyTable->Lines.pop_back();
			}
		}
		if (App::IsKeyPressed('S'))
		{
			return EEditorState::Save;
		}
		else
		{
			return EEditorState::Edit;
		}
	}

	for (int Idx = 0; Idx < ELineType::MAX; ++Idx)
	{
		if (App::IsKeyPressed('0' + Idx))
		{
			DefaultLineType = static_cast<ELineType>(Idx);
		}
	}


	if (App::IsKeyPressed(VK_LBUTTON))
	{
		if (!SelectedLine)
		{
			// add new line
			if (SelectedPoint)
			{
				SelectedPoint->bSelected = false;
				MyTable->Lines.push_back(CLineSegment(SelectedPoint->X, SelectedPoint->Y, InX, InR, DefaultLineType));
			}
			else
			{
				MyTable->Lines.push_back(CLineSegment(SnappedX, SnappedY, InX, InR, DefaultLineType));
			}
			SelectedLine = &(MyTable->Lines.back());
			SelectedLine->bSelected = true;
			SelectedLine->EndPoint.bSelected = true;
		}
	}
	else
	{
		if (SelectedLine)
		{
			SelectedLine->bSelected = false;
		}
	}

	if (SelectedPoint)
	{
		// Snap moving point to close points (on any other line)
		if (!App::IsKeyPressed(VK_LSHIFT))
		{
			SelectedPoint->X = SnappedX;
			SelectedPoint->Y = SnappedY;
		}
		else
		{
			SelectedPoint->X = InX;
			SelectedPoint->Y = InR;
		}
		StatusMessage = ControlsMessage;
	}

	return EEditorState::Add;
}

EEditorState Editor::Save(const char* InFilename)
{
	if (!App::IsKeyPressed('S'))
	{
		std::fstream File;
		File.open(InFilename, std::fstream::out);
		if (File.is_open())
		{
			File << VERSION;
			for (const auto& Line : MyTable->Lines)
			{
				File << '\n';
				File << Line.Type << ' ';
				File << Line.StartPoint.X << ' ';
				File << Line.StartPoint.Y << ' ';
				File << Line.EndPoint.X << ' ';
				File << Line.EndPoint.Y;
			}

			File.close();
			StatusMessage = "SAVED";
		}
		else
		{
			// error message!
			StatusMessage = "SAVE FAILED";
		}

		return EEditorState::Edit;
	}
	return EEditorState::Save;
}


void Editor::Load(const char* InFilename)
{
	std::fstream File;
	File.open(InFilename, std::fstream::in);
	if (File.is_open())
	{
		int Ver = 0;
		File >> Ver;
		if (Ver <= 1)
		{
			MyTable->Lines.clear();
			while (!File.eof())
			{
				CLineSegment Line;
				int Type;
				File >> Type;
				Line.Type = static_cast<ELineType>(Type);
				File >> Line.StartPoint.X;
				File >> Line.StartPoint.Y;
				File >> Line.EndPoint.X;
				File >> Line.EndPoint.Y;
				MyTable->Lines.push_back(Line);
			}

			StatusMessage = "LOADED OK";
		}
		else
		{
			StatusMessage = "LOAD FAILED: UNHANDLED VERSION";
		}

		File.close();
	}
	else
	{
		// error message!
		StatusMessage = "LOAD FAILED";
	}

}

//------------------------------------------------------------------------
// Add your display calls here (DrawLine or Print) 
// See App.h 
//------------------------------------------------------------------------
void EditorRender()
{
	float YPos = 740.0f;
	const float Dy = -12.0f;
	char Buffer[256];
	const char* DisplayStr = Buffer;
	char* Test = Buffer;
	strcpy_s(Buffer, 255, StatusMessage);
	const int Length = static_cast<int>(strlen(StatusMessage));
	while (Test - Buffer <= Length)
	{
		if (*Test == '\n' || *Test == 0)
		{
			*Test = 0;
			App::RenderText(10.0f, YPos, DisplayStr, 1.0f, 1.0f, 1.0f, GLUT_BITMAP_8_BY_13);
			DisplayStr = Test + 1;
			YPos += Dy;
		}
		++Test;
	}
	


//	y = 700.0f;

	for (auto& Line : MyTable->Lines)
	{
		if (Line.bSelected)
		{
			App::RenderLine(Line.StartPoint.X, Line.StartPoint.Y, Line.EndPoint.X, Line.EndPoint.Y, 0.0f, 0.0f);
			Editor::DrawBoxAroundPoint(Line.StartPoint, 6.0f);
			Editor::DrawBoxAroundPoint(Line.EndPoint, 6.0f);

//			char textBuffer[256];
//			sprintf(textBuffer, "%0.1f, %0.1f - %0.1f, %0.1f", line.m_start.m_x, line.m_start.m_y, line.m_end.m_x, line.m_start.m_y);
//			App::Print(10.0f, y, textBuffer, 0.0f, 0.0f, 1.0f, GLUT_BITMAP_8_BY_13);
		}
		else
		{
			CLineDefinition& LineDef = MyTable->LineDefinitions[Line.Type];
			App::RenderLine(Line.StartPoint.X, Line.StartPoint.Y, Line.EndPoint.X, Line.EndPoint.Y, LineDef.Red, LineDef.Green, LineDef.Blue);
//			char textBuffer[256];
//			sprintf(textBuffer, "%0.1f, %0.1f - %0.1f, %0.1f", line.m_start.m_x, line.m_start.m_y, line.m_end.m_x, line.m_start.m_y);
//			App::Print(10.0f, y, textBuffer, 1.0f, 1.0f, 1.0f, GLUT_BITMAP_8_BY_13);
		}
		YPos += Dy;
	}
}



//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void EditorShutdown()
{
	if (strcmp(StatusMessage, "SAVED") != 0)
	{
		Editor::Save("backup.txt");
	}
	delete MyTable;
}

void Editor::DrawBoxAroundPoint(const CPoint& InPoint, float InSize)
{
	const float BlueGreen = InPoint.bSelected ? 0.0f : 1.0f;
	const float Radius = InSize / 2.0f;
	const float XPos = InPoint.X;
	const float YPos = InPoint.Y;
	App::RenderLine(XPos - Radius, YPos - Radius, XPos + Radius, YPos - Radius, 1.0f, BlueGreen, BlueGreen);
	App::RenderLine(XPos + Radius, YPos - Radius, XPos + Radius, YPos + Radius, 1.0f, BlueGreen, BlueGreen);
	App::RenderLine(XPos + Radius, YPos + Radius, XPos - Radius, YPos + Radius, 1.0f, BlueGreen, BlueGreen);
	App::RenderLine(XPos - Radius, YPos + Radius, XPos - Radius, YPos - Radius, 1.0f, BlueGreen, BlueGreen);
}
