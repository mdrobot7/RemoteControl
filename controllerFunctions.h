#pragma once

int deadzone = 7000; //Range: -32768 - 32767, 0 is the center (positive is either up or right)
int scrollDeadzone = 7000; //the deadzone on the page scrolling stick
int otherAxisDeadzone = 10000; //the deadzone for the other axis when the stick is moved all the way in one direction (to not register a diagonal or a different direction)
                               //used for keyboard stuff only
int triggerThreshold = 75; //Range: 0-255

void refresh(BYTE controllerID) //controllerID - 0-3, which 'player' you are
{
	ZeroMemory(&controller, sizeof(XINPUT_STATE));
	XInputGetState(controllerID, &controller);
}

bool button(int key) //key - XINPUT_GAMEPAD button code for which button to scan
{
	if (key == XINPUT_GAMEPAD_LTRIGGER) //key = -1
	{
		if (int(controller.Gamepad.bLeftTrigger) > triggerThreshold) return true;
		else return false;
	}
	if (key == XINPUT_GAMEPAD_RTRIGGER) //key = -2
	{
		if (int(controller.Gamepad.bRightTrigger) > triggerThreshold) return true;
		else return false;
	}
	return ((controller.Gamepad.wButtons & key) != 0); //& = bitwise AND comparison, compares the two values (wButtons adn key) on a bit level, returns 1 for true, 0 for false
}

bool ltrigger()
{
	return (controller.Gamepad.bLeftTrigger > triggerThreshold);
}

bool rtrigger()
{
	return (controller.Gamepad.bRightTrigger > triggerThreshold);
}

void initController()
{
	refresh(0);
	while (XInputGetState(0, &controller) != ERROR_SUCCESS) //waiting for controller to connect on port 0
	{
		refresh(0);
		Sleep(5);
	}
	cout << "Controller 0 connected.\n";
	refresh(0);
	cout << "Press Start and Back on the controller at the same time to begin.\n";
	while (button(XINPUT_GAMEPAD_START) != true && button(XINPUT_GAMEPAD_BACK) != true)
	{
		refresh(0);
		Sleep(5);
	}
	while (button(XINPUT_GAMEPAD_START) == true || button(XINPUT_GAMEPAD_BACK) == true)
	{
		refresh(0);
		Sleep(5);
	}
	cout << "Controller 0 enabled! Press the Xbox button to turn it on!\n";
}