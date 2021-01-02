#pragma once //tells the compiler to only include this file once
#include "includes.h"
#include "controllerFunctions.h"

#define lstickX controller.Gamepad.sThumbLX
#define lstickY controller.Gamepad.sThumbLY
#define rstickX controller.Gamepad.sThumbRX
#define rstickY controller.Gamepad.sThumbRY

float speed = 1;
float slope = 0;
//for a description on the angles, see the block comment at the bottom

int xcursor = 500; //random starting positions, they get updated every time moveCursorRelative is called
int ycursor = 500;

//these exist solely to simplify the code in the handleMouse() function. they really don't serve any other purpose.
//they are the x and y coordinates of where the mouse moves for each of the mouse zones.
const short _cursorX[16] = { 2, 2, 2, 1, 0, -1, -2, -2, -2, -2, -2, -1, 0, 1, 2, 2 };
const short _cursorY[16] = { 0, 1, 2, 2, 2, 2, 2, 1, 0, -1, -2, -2, -2, -2, -2, -1 };
short zone = 0; //intermediary variable between handleZones() and handleMouse() because the compiler didn't like the former as an int function.

void setCursorRelative(int x, int y)
{
    xcursor = (xcursor + x);
    ycursor = (ycursor + y);
    SetCursorPos(xcursor, ycursor);
}

void handleZones()
{
    if (lstickY > 0)
    {
        if (slope > 0 && slope <= 0.5 / 2) zone = 0;
        else if (slope > 0.5 / 2 && slope <= 1.5 / 2) zone = 1;
        else if (slope > 1.5 / 2 && slope <= 2 / 1.5) zone = 2;
        else if (slope > 2 / 1.5 && slope <= 2 / 0.5) zone = 3;
        else if (slope > 2 / 0.5 || slope <= 2 / -0.5) zone = 4;
        else if (slope > 2 / -0.5 && slope <= 2 / -1.5) zone = 5;
        else if (slope > 2 / -1.5 && slope <= 1.5 / -2) zone = 6;
        else if (slope > 1.5 / -2 && slope <= 0.5 / -2) zone = 7;
        else if (slope > 0.5 / -2 && slope <= 0) zone = 8;
    }
    else
    {
        if (slope > 0 && slope <= -0.5 / -2) zone = 8;
        else if (slope > -0.5 / -2 && slope <= -1.5 / -2) zone = 9;
        else if (slope > -1.5 / -2 && slope <= -2 / -1.5) zone = 10;
        else if (slope > -2 / -1.5 && slope <= -2 / -0.5) zone = 11;
        else if (slope > -2 / -0.5 || slope <= -2 / 0.5) zone = 12;
        else if (slope > -2 / 0.5 && slope <= -2 / 1.5) zone = 13;
        else if (slope > -2 / 1.5 && slope <= -1.5 / 2) zone = 14;
        else if (slope > -1.5 / 2 && slope <= -0.5 / 2) zone = 15;
        else if (slope > -0.5 / 2 && slope <= 0) zone = 0;
    }

}

void handleMouse()
{
    refresh(0); //get joystick values
    if (lstickX < deadzone && lstickX > -deadzone && lstickY < deadzone && lstickY > -deadzone) //if the stick is still in the deadzone
    {
        return;
    }
    speed = sqrt(pow(lstickX, 2) + pow(lstickY, 2)); //distance formula (from origin to current stick pos) div. by 10 to scale the value
    if (lstickX != 0 && lstickY != 0)
    {
        slope = float(lstickY) / float(lstickX);
        handleZones();
    }
    else if (lstickX == 0)
    {
        if (lstickY > 0) zone = 4;
        else if (lstickY < 0) zone = 12;
    }
    else if (lstickY == 0)
    {
        if (lstickX > 0) zone = 0;
        else if (lstickX < 0) zone = 8;
    }
    else;
    setCursorRelative(_cursorX[zone], -_cursorY[zone]);
}

void handleScrolling()
{
    refresh(0);
    ZeroMemory(&input, sizeof(INPUT));
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_WHEEL;
    input.mi.time = 0;
    int scrollSpeed = rstickY / 2000;
    if (rstickY > scrollDeadzone || rstickY < -scrollDeadzone)
    {
        input.mi.mouseData = scrollSpeed;
    }
    else return;
    SendInput(1, &input, sizeof(INPUT));
}

/*
HOW THE ANGLES WORK:

Take a coordinate grid, and draw a 4x4 unit square centered on the origin (the sides are y=2, y=-2, x=2, x=-2, and the intersections are the
corners). Now, starting at a corner, draw a dot. Now go one unit clockwise around the sides of the square. Draw another dot. Repeat. Each dot
should be on the intersection of 2 grid lines on the graph. Each point is an integer.

Each of these dots is an angle that is used in the calculation of where the mouse will go. Now, go around the square and draw an 'x' directly
between each of the dots. It should look like you shifted all of the dots clockwise around the square by 0.5 units and turned them into x's.

Now, draw lines between each of the x's and the origin. Calculate the slope of each of these lines. These are the borders between
all of the dots' regions.

The code draws a line between the imaginary point (lstickX, lstickY) to the origin, and then tries to find the closest dot on that square to 
the imaginary line. To do this, it finds what 'region' the point is in, by finding the slope of the imaginary line with the slope formula
(delta Y/delta X). It can calculate the slopes of each of the region boundaries, and it's if loops from there.

With the region number, then you can finally move the mouse to the corresponding dot on the square.

The speed of the mouse movement is done by calculating the result of the distance formula between (0,0) and (lstickX, lstickY), and plugging it into a
formula to scale it down to an appropriate delay amount.

====================================================================

The whole purpose of this is because the SetCursorPos function won't take floating point inputs, so you have to make the region lines and
calculate the region and movement from there. If it could take floating point inputs, and accurately use them without truncating, then you
could do it a simpler way.
*/