#ifndef GE_CONTROLLER_H
#define GE_CONTROLLER_H

#if (_WIN32_WINNT >= 0x0604 /*_WIN32_WINNT_WIN8*/)
#include <XInput.h>
#pragma comment(lib,"xinput.lib")
#else
#include <XInput.h>
#pragma comment(lib,"xinput9_1_0.lib")
#endif

// XInput handles up to 4 controllers 
#define MAX_CONTROLLERS 4

class CController
{
public:
	friend class CControllerManager;
	CController() : LastButtons(0), DebouncedButtons(0), bConnected(false)
	{
		ZeroMemory(&InputState, sizeof(XINPUT_STATE));
	}	

	bool CheckButton(int button, bool bPress = true) const
	{
		if (bPress)
		{
			return (DebouncedButtons & button) != 0;
		}
		else
		{
			return (LastButtons & button) != 0;
		}
	}

	float GetLeftThumbStickX() const
	{
		return static_cast<float>(InputState.Gamepad.sThumbLX) / 32768.0f;
	}

	float GetLeftThumbStickY() const
	{
		return static_cast<float>(InputState.Gamepad.sThumbLY) / 32768.0f;
	}

	float GetRightThumbStickX() const
	{
		return static_cast<float>(InputState.Gamepad.sThumbRX) / 32768.0f;
	}

	float GetRightThumbStickY() const
	{
		return static_cast<float>(InputState.Gamepad.sThumbRY) / 32768.0f;
	}

	float GetLeftTrigger() const
	{
		return static_cast<float>(InputState.Gamepad.bLeftTrigger) / 255.0f;
	}

	float GetRightTrigger() const
	{
		return static_cast<float>(InputState.Gamepad.bRightTrigger) / 255.0f;
	}

protected:
	XINPUT_STATE InputState;
	WORD LastButtons;
	WORD DebouncedButtons;
	bool bConnected;
};

class CControllerManager
{
public:
	static CControllerManager& GetInstance();
	void Update();
	const CController& GetController(int PadId = 0)
	{
		if (PadId >= MAX_CONTROLLERS)
		{
			PadId = 0;
		}
		
		return Controllers[PadId];
	}
private:
	CController Controllers[MAX_CONTROLLERS];
};
#endif