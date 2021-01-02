#pragma once //tells the compiler to only include this file once
#include "files.h"
#include "controllerFunctions.h"

int currentLetter = _f; //starting letter, after that it keeps track of the currently highlighted letter
bool inSymbolMenu = false; //false = the cursor is on the keyboard; true = cursor is in the symbol menu

void doInput(int key, bool release) //key - virtual key code for the key, release - if it's a press and release (if false, just a press)
{
	if (key == VK_LBUTTON || key == VK_RBUTTON || key == VK_MBUTTON) //the non-functional virtual keycodes for the mouse buttons
	{
		if (key == VK_LBUTTON) key = MOUSEEVENTF_LEFTDOWN;
		else if (key == VK_RBUTTON) key = MOUSEEVENTF_RIGHTDOWN;
		else if (key == VK_MBUTTON) key = MOUSEEVENTF_MIDDLEDOWN;

		ZeroMemory(&input, sizeof(INPUT)); //clear the struct for the INPUT input object (make it all 0's)
		input.type = INPUT_MOUSE; //change the 'type' variable to configure the input as a mouse (this can be changed at any time)
		input.mi.dwFlags = key; //set the 'flags' variable (essentially what happened) to 'left button down'
		SendInput(1, &input, sizeof(INPUT)); //send the whole struct through this function to execute the command

		if (release)
		{
			if (key == MOUSEEVENTF_LEFTDOWN) key = MOUSEEVENTF_LEFTUP;
			else if (key == MOUSEEVENTF_RIGHTDOWN) key = MOUSEEVENTF_RIGHTUP;
			else if (key == MOUSEEVENTF_MIDDLEDOWN) key = MOUSEEVENTF_MIDDLEUP;

			ZeroMemory(&input, sizeof(INPUT));
			input.type = INPUT_MOUSE;
			input.mi.dwFlags = key;
			SendInput(1, &input, sizeof(INPUT));
		}
	}
	else
	{
		ZeroMemory(&input, sizeof(INPUT));
		input.type = INPUT_KEYBOARD;
		input.ki.wVk = key;
		input.ki.dwFlags = 0; //0 for a keypress
		SendInput(1, &input, sizeof(INPUT));
		if (release)
		{
			input.ki.dwFlags = KEYEVENTF_KEYUP; //this or 0x0002 for a key up
			SendInput(1, &input, sizeof(INPUT));
		}
	}
}

//note: the code for referencing the config files in the function below is directly from handleButtons() in buttons.h, but the compiler wouldn't
//include this file and buttons.h into each other (they each would have functions and assets that the other file needed if I referenced handleButtons()
//below, and that's apparently an issue)
void handleJoystick(BYTE left, BYTE right, BYTE up, BYTE down, BYTE upleft, BYTE upright, BYTE downleft, BYTE downright)
{
	//these variables are local on purpose, don't make them global!!!!!
	int lastButton = 0; //the last value of i used in the last run of the for loop
	int lastControllerState = -3; //the last value of wbuttons from the last run of the loop

	using namespace std::chrono;
	long long now = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

	bool mouseButtonReleased = false; //checks if a particular mouse button has been previously released. false = no, true = yes.
	//while the stick hasn't moved
	while (controller.Gamepad.sThumbLX < deadzone && controller.Gamepad.sThumbLX > -deadzone && controller.Gamepad.sThumbLY < deadzone && controller.Gamepad.sThumbLY > -deadzone)
	{
		int _xinputButtonCodes[16] = { 0x0001, 0x0002, 0x0004, 0x0008, 0x0010, 0x0020, 0x0040, 0x0080, 0x0100, 0x0200, 0x1000, 0x2000, 0x4000, 0x8000, -1, -2 };
		//order of values above: Dpad up, Dpad down, Dpad left, Dpad right, start, back, Left stick click, Right stick click, LB, RB, A, B, X, Y, ltrigger, rtrigger
		//triggers are held in different variables
		refresh(0);
		long long nowPlusDelay = now + repeatDelays(lastButton, 1);
		if (controller.Gamepad.wButtons != lastControllerState && lastControllerState >= 0) lastControllerState = -3; //if the appropriate buttons are no longer being pushed, invalidate the controller state
		if (!button(XINPUT_GAMEPAD_LTRIGGER) && lastControllerState == -1) lastControllerState = -3; //if the buttons are no longer being pressed
		if (!button(XINPUT_GAMEPAD_RTRIGGER) && lastControllerState == -2) lastControllerState = -3;
		if ((controller.Gamepad.wButtons == lastControllerState || lastControllerState == -1 || lastControllerState == -2)
			&& (controller.Gamepad.wButtons != 0 || button(XINPUT_GAMEPAD_LTRIGGER) || button(XINPUT_GAMEPAD_RTRIGGER))) //if lastControllerState is flagged and the buttons are still being pushed
		{
			cout << lastButton << ", " << repeatDelays(lastButton, 1) << endl;
			if (duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count() <= nowPlusDelay) continue; //if the timer hasn't expired yet, return the function and try again
			//repeat the keystroke, unless it's a mouse keystroke. in that case, just hold the keystroke as is, and return; the function.
			//reset the timer
			if (keyMappings(lastButton, 1) == VK_MBUTTON || keyMappings(lastButton, 1) == VK_RBUTTON) continue; //don't repeat right and middle mouse clicks
			doInput(keyMappings(lastButton, 1), true);
			now = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
			continue;
		}
		//no mouse button holds are required because they don't work on the OSK
		if (controller.Gamepad.wButtons != 0 || button(XINPUT_GAMEPAD_LTRIGGER) || button(XINPUT_GAMEPAD_RTRIGGER))
		{
			for (int i = 0; i < sizeof(_xinputButtonCodes); i++)
			{
				if (button(_xinputButtonCodes[i]))
				{
					if (keyMappings(i, 1) == -1)
					{
						currentLetter = _esc;
						return;
					}
					else if (keyMappings(i, 1) == VK_SHIFT && inSymbolMenu == false)
					{
						SetCursorPos(xCoordinates(_shift), yCoordinates(_shift));
						doInput(VK_LBUTTON, true);
						SetCursorPos(xCoordinates(currentLetter), yCoordinates(currentLetter));
					}
					else doInput(keyMappings(i, 1), true);
					if (keyMappings(i, 1) == VK_LBUTTON && currentLetter == _symbol) //PROBLEM IS IN HERE SOMEWHERE
					{
						inSymbolMenu = !inSymbolMenu;
						Sleep(repeatDelays(i, 1));
						return;
					}
					lastButton = i;
					lastControllerState = controller.Gamepad.wButtons;
					if (i == 14) lastControllerState = -1;
					else if (i == 15) lastControllerState = -2;
					else;
					now = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
					break;
				}
			}
		}
	}
	//the second the sticks move, run through these:
	if (controller.Gamepad.sThumbLX > deadzone && controller.Gamepad.sThumbLY < otherAxisDeadzone && controller.Gamepad.sThumbLY > -otherAxisDeadzone)
	{
		SetCursorPos(xCoordinates(right), yCoordinates(right));
		currentLetter = right;
	}
	else if (controller.Gamepad.sThumbLX < -deadzone && controller.Gamepad.sThumbLY < otherAxisDeadzone && controller.Gamepad.sThumbLY > -otherAxisDeadzone)
	{
		SetCursorPos(xCoordinates(left), yCoordinates(left));
		currentLetter = left;
	}
	else if (controller.Gamepad.sThumbLY > deadzone && controller.Gamepad.sThumbLX < otherAxisDeadzone && controller.Gamepad.sThumbLX > -otherAxisDeadzone)
	{
		SetCursorPos(xCoordinates(up), yCoordinates(up));
		currentLetter = up;
	}
	else if (controller.Gamepad.sThumbLY < -deadzone && controller.Gamepad.sThumbLX < otherAxisDeadzone && controller.Gamepad.sThumbLX > -otherAxisDeadzone)
	{
		SetCursorPos(xCoordinates(down), yCoordinates(down));
		currentLetter = down;
	}
	else {}
	ZeroMemory(&controller, sizeof(XINPUT_STATE));
	Sleep(250);
}

//==========================================================================================================================================//

void keyboard() //the main loop for the controller keyboard, put here to neaten up the main file
{
	switch (currentLetter) //joystick syntax: (left, right, up, down, upleft, upright, downleft, downright)
						   //set the handlejoystick to the current letter (itself) if you don't want the cursor moved at all
	{
	case _a:
		handleJoystick(_a, _s, _q, _z, _a, _a, _a, _a);
		break;
	case _b:
		handleJoystick(_v, _n, _g, _b, _b, _b, _b, _b);
		break;
	case _c:
		handleJoystick(_x, _v, _d, _c, _c, _c, _c, _c);
		break;
	case _d:
		handleJoystick(_s, _f, _e, _c, _d, _d, _d, _d);
		break;
	case _e:
		handleJoystick(_w, _r, _e, _d, _e, _e, _e, _e);
		break;
	case _f:
		handleJoystick(_d, _g, _r, _v, _f, _f, _f, _f);
		break;
	case _g:
		handleJoystick(_f, _h, _t, _b, _g, _g, _g, _g);
		break;
	case _h:
		handleJoystick(_g, _j, _y, _n, _h, _h, _h, _h);
		break;
	case _i:
		handleJoystick(_u, _o, _i, _k, _i, _i, _i, _i);
		break;
	case _j:
		handleJoystick(_h, _k, _u, _m, _j, _j, _j, _j);
		break;
	case _k:
		handleJoystick(_j, _l, _i, _comma, _k, _k, _k, _k);
		break;
	case _l:
		handleJoystick(_k, _apostrophe, _o, _period, _l, _l, _l, _l);
		break;
	case _m:
		handleJoystick(_n, _comma, _j, _m, _m, _m, _m, _m);
		break;
	case _n:
		handleJoystick(_b, _m, _h, _n, _n, _n, _n, _n);
		break;
	case _o:
		handleJoystick(_i, _p, _o, _l, _o, _o, _o, _o);
		break;
	case _p:
		handleJoystick(_o, _p, _p, _apostrophe, _p, _p, _p, _p);
		break;
	case _q:
		handleJoystick(_q, _w, _q, _a, _q, _q, _q, _q);
		break;
	case _r:
		handleJoystick(_e, _t, _r, _f, _r, _r, _r, _r);
		break;
	case _s:
		handleJoystick(_a, _d, _w, _x, _s, _s, _s, _s);
		break;
	case _t:
		handleJoystick(_r, _y, _t, _g, _t, _t, _t, _t);
		break;
	case _u:
		handleJoystick(_y, _i, _u, _j, _u, _u, _u, _u);
		break;
	case _v:
		handleJoystick(_c, _b, _f, _v, _v, _v, _v, _v);
		break;
	case _w:
		handleJoystick(_q, _e, _w, _s, _w, _w, _w, _w);
		break;
	case _x:
		handleJoystick(_z, _c, _s, _x, _x, _x, _x, _x);
		break;
	case _y:
		handleJoystick(_t, _u, _y, _h, _y, _y, _y, _y);
		break;
	case _z:
		handleJoystick(_symbol, _x, _a, _symbol, _z, _z, _z, _z);
		break;
		//shift goes in here, but it doesn't need a case.
	case _comma:
		handleJoystick(_m, _period, _k, _comma, _comma, _comma, _comma, _comma);
		break;
	case _period:
		handleJoystick(_comma, _question, _l, _period, _period, _period, _period, _period);
		break;
	case _question:
		handleJoystick(_period, _question, _apostrophe, _question, _question, _question, _question, _question);
		break;
	case _apostrophe:
		handleJoystick(_l, _apostrophe, _p, _question, _apostrophe, _apostrophe, _apostrophe, _apostrophe);
		break;
	case _symbol:
		if (inSymbolMenu) //if it's in the symbol menu
		{
			handleJoystick(_symbol, _1, _symbol, _symbol, _symbol, _symbol, _symbol, _symbol);
		}
		else //if it's on the keyboard
		{
			handleJoystick(_symbol, _z, _a, _symbol, _symbol, _symbol, _symbol, _symbol);
		}
		break;
	case _0:
		handleJoystick(_symbol, _0, _7, _0, _0, _0, _0, _0);
		break;
	case _1:
		handleJoystick(_symbol, _2, _1, _4, _1, _1, _1, _1);
		break;
	case _2:
		handleJoystick(_1, _3, _2, _5, _2, _2, _2, _2);
		break;
	case _3:
		handleJoystick(_2, _3, _3, _6, _3, _3, _3, _3);
		break;
	case _4:
		handleJoystick(_symbol, _5, _1, _7, _4, _4, _4, _4);
		break;
	case _5:
		handleJoystick(_4, _6, _2, _8, _5, _5, _5, _5);
		break;
	case _6:
		handleJoystick(_5, _6, _3, _9, _6, _6, _6, _6);
		break;
	case _7:
		handleJoystick(_symbol, _8, _4, _0, _7, _7, _7, _7);
		break;
	case _8:
		handleJoystick(_7, _9, _5, _0, _8, _8, _8, _8);
		break;
	case _9:
		handleJoystick(_8, _9, _6, _0, _9, _9, _9, _9);
		break;
	}
}