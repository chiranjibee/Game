#include <windows.h>

#include "Controller.h"
#include "app.h"


#if (_WIN32_WINNT >= 0x0602 /*_WIN32_WINNT_WIN8*/)
#include <XInput.h>
#pragma comment(lib,"xinput.lib")
#else
#include <XInput.h>
#pragma comment(lib,"xinput9_1_0.lib")
#endif

// Define if you want a dead zone on the thumb stick analog inputs
#define UPDATE_DZONE

// Default to 10% of the +/- 32767 range. This is a reasonable default value but can be altered if needed
#define INPUT_DEADZONE  (0.10f * FLOAT(0x7FFF))

CControllerManager &CControllerManager::GetInstance()
{
	static CControllerManager Manager;
	return Manager;
}

void CControllerManager::Update()
{
    DWORD DwResult;
	int NumControllers = 0;
    for(DWORD Idx = 0; Idx < MAX_CONTROLLERS; Idx++ )
    {
        // Simply get the state of the controller from XInput
        DwResult = XInputGetState(Idx, &Controllers[Idx].InputState );

		if (DwResult == ERROR_SUCCESS)
		{
			Controllers[Idx].bConnected = true;
			NumControllers++;
		}
        else
		{
			Controllers[Idx].bConnected = false;
		}
    }
	
	// No controllers so lets fake one using keyboard defines
	if (NumControllers == 0 )
	{
		Controllers[0].bConnected = true;
		WORD Btns = 0;
		Controllers[0].InputState.Gamepad.sThumbLX = 0;
		Controllers[0].InputState.Gamepad.sThumbLY = 0;
		Controllers[0].InputState.Gamepad.sThumbRX = 0;
		Controllers[0].InputState.Gamepad.sThumbRY = 0;
		Controllers[0].InputState.Gamepad.bLeftTrigger = 0;
		Controllers[0].InputState.Gamepad.bRightTrigger = 0;		

		if (App::IsKeyPressed(APP_PAD_EMUL_LEFT_THUMB_LEFT)) Controllers[0].InputState.Gamepad.sThumbLX = -32767;
		if (App::IsKeyPressed(APP_PAD_EMUL_LEFT_THUMB_RIGHT)) Controllers[0].InputState.Gamepad.sThumbLX = 32767;
		if (App::IsKeyPressed(APP_PAD_EMUL_LEFT_THUMB_UP)) Controllers[0].InputState.Gamepad.sThumbLY = 32767;
		if (App::IsKeyPressed(APP_PAD_EMUL_LEFT_THUMB_DOWN)) Controllers[0].InputState.Gamepad.sThumbLY = -32767;
		if (App::IsKeyPressed(APP_PAD_EMUL_BUTTON_ALT_A)) Btns |= XINPUT_GAMEPAD_A;
		if (App::IsKeyPressed(APP_PAD_EMUL_START)) Btns |= XINPUT_GAMEPAD_START;
		
		if (App::IsKeyPressed(APP_PAD_EMUL_RIGHT_THUMB_LEFT)) Controllers[0].InputState.Gamepad.sThumbRX = -32767;
		if (App::IsKeyPressed(APP_PAD_EMUL_RIGHT_THUMB_RIGHT)) Controllers[0].InputState.Gamepad.sThumbRX = 32767;
		if (App::IsKeyPressed(APP_PAD_EMUL_RIGHT_THUMB_UP)) Controllers[0].InputState.Gamepad.sThumbRY = -32767;
		if (App::IsKeyPressed(APP_PAD_EMUL_RIGHT_THUMB_DOWN)) Controllers[0].InputState.Gamepad.sThumbRY = 32767;

		if (App::IsKeyPressed(APP_PAD_EMUL_DPAD_UP))   Btns |= XINPUT_GAMEPAD_DPAD_UP;
		if (App::IsKeyPressed(APP_PAD_EMUL_DPAD_DOWN)) Btns |= XINPUT_GAMEPAD_DPAD_DOWN;
		if (App::IsKeyPressed(APP_PAD_EMUL_DPAD_LEFT)) Btns |= XINPUT_GAMEPAD_DPAD_LEFT;
		if (App::IsKeyPressed(APP_PAD_EMUL_DPAD_RIGHT))Btns |= XINPUT_GAMEPAD_DPAD_RIGHT;

		if (App::IsKeyPressed(APP_PAD_EMUL_BUTTON_BACK)) Btns |= XINPUT_GAMEPAD_BACK;

		if (App::IsKeyPressed(APP_PAD_EMUL_BUTTON_A)) Btns |= XINPUT_GAMEPAD_A;
		if (App::IsKeyPressed(APP_PAD_EMUL_BUTTON_B)) Btns |= XINPUT_GAMEPAD_B;
		if (App::IsKeyPressed(APP_PAD_EMUL_BUTTON_X)) Btns |= XINPUT_GAMEPAD_X;
		if (App::IsKeyPressed(APP_PAD_EMUL_BUTTON_Y)) Btns |= XINPUT_GAMEPAD_Y;

		if (App::IsKeyPressed(APP_PAD_EMUL_LEFT_TRIGGER)) Controllers[0].InputState.Gamepad.bLeftTrigger = 255;
		if (App::IsKeyPressed(APP_PAD_EMUL_RIGHT_TRIGGER)) Controllers[0].InputState.Gamepad.bRightTrigger = 255;

		if (App::IsKeyPressed(APP_PAD_EMUL_BUTTON_LEFT_THUMB)) Btns |= XINPUT_GAMEPAD_LEFT_THUMB;
		if (App::IsKeyPressed(APP_PAD_EMUL_BUTTON_RIGHT_THUMB)) Btns |= XINPUT_GAMEPAD_RIGHT_THUMB;
		if (App::IsKeyPressed(APP_PAD_EMUL_BUTTON_LEFT_SHOULDER)) Btns |= XINPUT_GAMEPAD_LEFT_SHOULDER;
		if (App::IsKeyPressed(APP_PAD_EMUL_BUTTON_RIGHT_SHOULDER)) Btns |= XINPUT_GAMEPAD_RIGHT_SHOULDER;

		Controllers[0].InputState.Gamepad.wButtons = Btns;
	}

	for (DWORD Idx = 0; Idx < MAX_CONTROLLERS; Idx++)
	{
		if (Controllers[Idx].bConnected)
		{
#ifdef UPDATE_DZONE					
			Controllers[Idx].DebouncedButtons = ~Controllers[Idx].LastButtons &Controllers[Idx].InputState.Gamepad.wButtons;
			Controllers[Idx].LastButtons = Controllers[Idx].InputState.Gamepad.wButtons;

			// Zero value if thumbsticks are within the dead zone 
			if ((Controllers[Idx].InputState.Gamepad.sThumbLX < INPUT_DEADZONE &&
				Controllers[Idx].InputState.Gamepad.sThumbLX > -INPUT_DEADZONE) &&
				(Controllers[Idx].InputState.Gamepad.sThumbLY < INPUT_DEADZONE &&
					Controllers[Idx].InputState.Gamepad.sThumbLY > -INPUT_DEADZONE))
			{
				Controllers[Idx].InputState.Gamepad.sThumbLX = 0;
				Controllers[Idx].InputState.Gamepad.sThumbLY = 0;
			}

			if ((Controllers[Idx].InputState.Gamepad.sThumbRX < INPUT_DEADZONE &&
				Controllers[Idx].InputState.Gamepad.sThumbRX > -INPUT_DEADZONE) &&
				(Controllers[Idx].InputState.Gamepad.sThumbRY < INPUT_DEADZONE &&
					Controllers[Idx].InputState.Gamepad.sThumbRY > -INPUT_DEADZONE))
			{
				Controllers[Idx].InputState.Gamepad.sThumbRX = 0;
				Controllers[Idx].InputState.Gamepad.sThumbRY = 0;
			}
#endif		
		}
	}
}