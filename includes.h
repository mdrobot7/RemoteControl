#pragma once //tells the compiler to only include this file once
#define _USE_MATH_DEFINES

#include <Windows.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <Xinput.h>
#pragma comment (lib, "xinput.lib")
#include <cmath>
#include <chrono>
#include <ctime>

using namespace std;

fstream pos; //mouse positions on the keyboard
fstream map; //button mappings on the controller
fstream map2; //button mappings for the controller when the keyboard is up

INPUT input; //mouse and keyboard input
POINT mouse; //used to get the position of the mouse cursor

XINPUT_STATE controller; //the controller state struct

#define XINPUT_GAMEPAD_LTRIGGER -1
#define XINPUT_GAMEPAD_RTRIGGER -2

#define _triggerKeyboard -1
#define _triggerDisable -2
#define _triggerShowMouse -3

bool triggerKeyboard = false; //triggers the keyboard to start

#define _esc -1
#define _keyboard 0
#define _a 1
#define _b 2
#define _c 3
#define _d 4
#define _e 5
#define _f 6
#define _g 7
#define _h 8
#define _i 9
#define _j 10
#define _k 11
#define _l 12
#define _m 13
#define _n 14
#define _o 15
#define _p 16
#define _q 17
#define _r 18
#define _s 19
#define _t 20
#define _u 21
#define _v 22
#define _w 23
#define _x 24
#define _y 25
#define _z 26
#define _shift 27
#define _comma 28
#define _period 29
#define _question 30
#define _apostrophe 31
#define _symbol 32
#define _0 33
#define _1 34
#define _2 35
#define _3 36
#define _4 37
#define _5 38
#define _6 39
#define _7 40
#define _8 41
#define _9 42
#define _exit 43