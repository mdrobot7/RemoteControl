#pragma once
#include "includes.h"
#include "keyboardFunctions.h"
#include "controllerFunctions.h"
#include "files.h"

int lastButton = 0; //the last value of i used in the last run of the for loop
int lastControllerState = -3; //the last value of wbuttons from the last run of the loop

using namespace std::chrono;

bool mouseButtonReleased = false; //checks if a particular mouse button has been previously released. false = no, true = yes.

void handleButtons()
{
	int _xinputButtonCodes[16] = { 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x1000, 0x2000, 0x4000, 0x8000, -1, -2 };
	//order of values above: Dpad up, Dpad down, Dpad left, Dpad right, start, back, Left stick click, Right stick click, LB, RB, A, B, X, Y, ltrigger, rtrigger
	//triggers are held in different variables
	refresh(0);
	buttonTime.nowPlusDelay = buttonTime.now + repeatDelays(lastButton, 0);
	if (controller.Gamepad.wButtons != lastControllerState && lastControllerState >= 0) lastControllerState = -3; //if the appropriate buttons are no longer being pushed, invalidate the controller state
	if (!button(XINPUT_GAMEPAD_LTRIGGER) && lastControllerState == -1) lastControllerState = -3; //if the buttons are no longer being pressed
	if (!button(XINPUT_GAMEPAD_RTRIGGER) && lastControllerState == -2) lastControllerState = -3;
	if ((controller.Gamepad.wButtons == lastControllerState || lastControllerState == -1 || lastControllerState == -2)
		&& (controller.Gamepad.wButtons != 0 || button(XINPUT_GAMEPAD_LTRIGGER) || button(XINPUT_GAMEPAD_RTRIGGER))) //if lastControllerState is flagged and the buttons are still being pushed
	{
		if (duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() <= buttonTime.nowPlusDelay) return; //if the timer hasn't expired yet, return the function and try again
		//repeat the keystroke, unless it's a mouse keystroke. in that case, just hold the keystroke as is, and return; the function.
		//reset the timer
		if (keyMappings(lastButton, 0) == VK_LBUTTON || keyMappings(lastButton, 0) == VK_MBUTTON || keyMappings(lastButton, 0) == VK_RBUTTON) return; //don't repeat mouse clicks
		doInput(keyMappings(lastButton, 0), true);
		buttonTime.now = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
		return;
	}
	int key = keyMappings(lastButton, 0);
	if ((key <= 4 && key != 3) && mouseButtonReleased == false) //releases the mouse buttons once the button has been released
	{
		if (key == VK_LBUTTON) key = MOUSEEVENTF_LEFTUP;
		else if (key == VK_RBUTTON) key = MOUSEEVENTF_RIGHTUP;
		else if (key == VK_MBUTTON) key = MOUSEEVENTF_MIDDLEUP;
		ZeroMemory(&input, sizeof(INPUT));
		input.type = INPUT_MOUSE;
		input.mi.dwFlags = key;
		SendInput(1, &input, sizeof(INPUT));
		mouseButtonReleased = true;
	}
	if (controller.Gamepad.wButtons != 0 || button(XINPUT_GAMEPAD_LTRIGGER) || button(XINPUT_GAMEPAD_RTRIGGER))
	{
		for (int i = 0; i < sizeof(_xinputButtonCodes); i++)
		{
			if (button(_xinputButtonCodes[i]))
			{
				if (keyMappings(i, 0) == VK_LBUTTON || keyMappings(i, 0) == VK_MBUTTON || keyMappings(i, 0) == VK_RBUTTON)
				{
					doInput(keyMappings(i, 0), false);
					mouseButtonReleased = false;
				}
				else if (keyMappings(i, 0) == _triggerKeyboard) triggerKeyboard = true;
				else if (keyMappings(i, 0) == _triggerShowMouse)
				{
					/*
					int _tempPointer[1] = { true };
					SystemParametersInfoA(SPI_SETMOUSESONAR, 0, (bool *) *_tempPointer, SPIF_UPDATEINIFILE);
					doInput(VK_LCONTROL, true);
					_tempPointer[0] = false;
					SystemParametersInfoA(SPI_SETMOUSESONAR, 0, (bool *) *_tempPointer, SPIF_UPDATEINIFILE);
					*/
				}
				else doInput(keyMappings(i, 0), true);
				lastButton = i;
				lastControllerState = controller.Gamepad.wButtons;
				if (i == 14) lastControllerState = -1;
				else if (i == 15) lastControllerState = -2;
				else;
				buttonTime.now = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
				return;
			}
		}
	}
}

bool handleDisable()
{
	refresh(0);
	int _xinputButtonCodes[16] = { 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x1000, 0x2000, 0x4000, 0x8000, -1, -2 };
	if (controller.Gamepad.wButtons != 0 || button(XINPUT_GAMEPAD_LTRIGGER) || button(XINPUT_GAMEPAD_RTRIGGER))
	{
		for (int i = 0; i < sizeof(_xinputButtonCodes); i++)
		{
			if (button(_xinputButtonCodes[i]) && keyMappings(i, 0) == -2) //the custom keymapping for enable/disable is -2, or -0x02
			{
				return true;
			}
		}
	}
	return false;
}

/*
HELPFUL LINKS:
https://docs.microsoft.com/en-us/windows/win32/api/XInput/ns-xinput-xinput_gamepad
https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes?redirectedfrom=MSDN
https://docs.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-input
https://docs.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-keybdinput
https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-sendinput
https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-blockinput

https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-systemparametersinfoa
*/