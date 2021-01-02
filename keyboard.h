#pragma once //tells the compiler to only include this file once
#include "includes.h"
#include "keyboardFunctions.h"

void handleKeyboard()
{
    if (triggerKeyboard == true)
    {
        ZeroMemory(&mouse, sizeof(mouse));
        GetCursorPos(&mouse);
        SetCursorPos(xCoordinates(_keyboard), yCoordinates(_keyboard)); //open osk
        doInput(VK_LBUTTON, true);
        SetCursorPos(xCoordinates(_f), yCoordinates(_f)); //put cursor on the f key
        currentLetter = _f;
        while (button(XINPUT_GAMEPAD_Y) == true) //wait for y button to be released
        {
            refresh(0);
            Sleep(10);
        }
        while (currentLetter != _esc) //while the esc flag isn't set
        {
            keyboard(); //the switch loop
        }
        SetCursorPos(xCoordinates(_exit), yCoordinates(_exit));
        Sleep(250);
        doInput(VK_LBUTTON, true);
        doInput(VK_LBUTTON, true);
        SetCursorPos(mouse.x, mouse.y);
        triggerKeyboard = false;
    }
    else
    {
        ZeroMemory(&controller, sizeof(XINPUT_STATE));
    }
}