#ifndef GE_APP_H
#define GE_APP_H

#include <SDKDDKVer.h>

#include <stdio.h>
#include <tchar.h>

#include <windows.h>

#include <GL/freeglut.h>
#include "AppSettings.h"
#include "Controller.h"
#include "Sprite.h"

#define APP_VIRTUAL_TO_NATIVE_COORDS(_x_,_y_)	_x_ = ((_x_ / APP_VIRTUAL_WIDTH )*2.0f) - 1.0f; _y_ = ((_y_ / APP_VIRTUAL_HEIGHT)*2.0f) - 1.0f;
#define APP_NATIVE_TO_VIRTUAL_COORDS(_x_,_y_)	_x_ = ((_x_ + 1.0f) * APP_VIRTUAL_WIDTH) / 2.0f; _y_ = ((_y_ + 1.0f) * APP_VIRTUAL_HEIGHT) / 2.0f;

namespace App
{
	/**
	 * RenderLine - Draw a 2D Line from Start X, Y to End X, Y with color R, G, B 
	 */
	void RenderLine(float StartX, float StartY, float EndX, float EndY, float R = 1.0f, float G = 1.0f, float B = 1.0f);
	
	/**
	 * RenderText - Prints text to screen at X, Y with color R, G, B using the GLUT font
	 *
	 * Fonts:
	 * GLUT_BITMAP_9_BY_15
	 * GLUT_BITMAP_8_BY_13
	 * GLUT_BITMAP_TIMES_ROMAN_10
	 * GLUT_BITMAP_TIMES_ROMAN_24
	 * GLUT_BITMAP_HELVETICA_10
	 * GLUT_BITMAP_HELVETICA_12
	 * GLUT_BITMAP_HELVETICA_18
	 */
	void RenderText(float X, float Y, const char* Text, float R = 1.0f, float G = 1.0f, float B = 1.0f, void* Font = GLUT_BITMAP_HELVETICA_18);
	
	/**
	 * CreateSprite - Creates a sprite from the provided file (32-bit BMP format). Cols and Rows
	 * define the number of columns and rows of the sprite animation frames in the given image
	 */
	CSprite* CreateSprite(const char* Filename, int Cols, int Rows);
	
	/**
	 * PlaySound - Plays the passed in .wav file
	 */
	void PlaySound(const char* Filename, bool bLoop = false);

	/**
	 * StopSound - Stops the give .wav file
	 */
	void StopSound(const char* Filename);
	
	/**
	 * IsSoundPlaying - Returns true if the given .wav file is playing
	 */
	bool IsSoundPlaying(const char* Filename);
	
	/**
	 * IsKeyPressed - Returns true if the give key is currently being pressed
	 *
	 * E.g. IsKeyPressed('A')
	 */
	bool IsKeyPressed(int Key);
	
	/**
	 * GetMousePos - Sets the value of the floats to the current mouse pointer position
	 */
	void GetMousePos(float &XCoord, float &YCoord);
	
	/**
	 * GetController - Returns the reference to the game controller
	 *
	 * PadId - 0 to 4
	 */
	const CController& GetController(int PadId = 0);
};
#endif