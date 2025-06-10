#include <string>
#include "main.h"
#include "App.h"
#include "Sound.h"
#include "Controller.h"
#include "Sprite.h"

namespace App
{	
	void RenderLine(float StartX, float StartY, float EndX, float EndY, float InR, float InG, float InB)
	{
#if APP_USE_VIRTUAL_RES		
		APP_VIRTUAL_TO_NATIVE_COORDS(StartX, StartY);
		APP_VIRTUAL_TO_NATIVE_COORDS(EndX, EndY);
#endif
		glBegin(GL_LINES);
		glColor3f(InR, InG, InB);
		glVertex2f(StartX, StartY);
		glVertex2f(EndX, EndY);
		glEnd();
	}
	
	CSprite* CreateSprite(const char* Filename, int Cols, int Rows)
	{
		return new CSprite(Filename, Cols, Rows);
	}

	bool IsKeyPressed(int Key)
	{
		return ((GetAsyncKeyState(Key) & 0x8000) != 0);
	}

	void GetMousePos(float &X, float &Y)
	{
		POINT MousePos;
		GetCursorPos(&MousePos);		
		ScreenToClient(MAIN_WINDOW_HANDLE, &MousePos);
		X = static_cast<float>(MousePos.x);
		Y = static_cast<float>(MousePos.y);
		X = (X * (2.0f / WINDOW_WIDTH) - 1.0f);
		Y = -(Y * (2.0f / WINDOW_HEIGHT) - 1.0f);

#if APP_USE_VIRTUAL_RES		
		APP_NATIVE_TO_VIRTUAL_COORDS(X, Y);
#endif
	}
	void PlaySound(const char* Filename, bool bLoop)
	{
		DWORD Flags = (bLoop) ? DSBPLAY_LOOPING : 0;
		CSound::GetInstance().PlaySound(Filename, Flags);
	}
	void StopSound(const char* Filename)
	{
		CSound::GetInstance().StopSound(Filename);
	}
	bool IsSoundPlaying(const char* Filename)
	{
		return CSound::GetInstance().IsPlaying(Filename);
	}

	// This prints a string to the screen
	void RenderText(float X, float Y, const char* Text, float InR, float InG, float InB, void* Font)
	{
#if APP_USE_VIRTUAL_RES		
		APP_VIRTUAL_TO_NATIVE_COORDS(X, Y);
#endif		
		// Set location to start printing text
		glColor3f(InR, InG, InB);
		glRasterPos2f(X, Y);
		const int Len = static_cast<int>(strlen(Text));
		for (int i = 0; i < Len; i++)
		{
			glutBitmapCharacter(Font, Text[i]);
		}
	}

	const CController& GetController(int PadId)
	{
		return CControllerManager::GetInstance().GetController(PadId);
	}
}