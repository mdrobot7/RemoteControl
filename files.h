#pragma once //tells the compiler to only include this file once
#include "includes.h"

char alpha[26] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z' };
char num[10] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' };
string special[6] = { "Shift", "Comma", "Period", "Question-Mark", "Apostrophe", "Symbol-Button" };

string buttons[16] = { "DPad-Up", "DPad-Down", "DPad-Left", "DPad-Right", "Start", "Back", "Left-Stick-Click", "Right-Stick-Click", "Left-Bumper", "Right-Bumper", "A", "B", "X", "Y", "Left-Trigger", "Right-Trigger" };
//start is the 3 lines, back is the 2 boxes

int incrementer = 0;
int index = 0;

string line; //the line of the .csv file that is being processed
string line2; //the variable that receives each individual value of each line of the .csv, and passes it to the values[] object
string values[3]; //where each line's values are stored, one by one
int ints[3] = { 0, 0, 0 }; //the 'values' array, but converted into integers

void waitForClick()
{
	while (!(GetKeyState(VK_LBUTTON) & 0x8000))
	{
		Sleep(1);
	}
}

void waitForRelease()
{
	while (GetKeyState(VK_LBUTTON) & 0x8000)
	{
		Sleep(1);
	}
}

void createPosFile()
{
	incrementer = 0;
	ZeroMemory(&mouse, sizeof(mouse)); //zeroes out the POINT mouse struct
	pos.open("pos.csv", ios::out); //opens the csv file in ::out mode
	cout << "Move your mouse to the Touch Keyboard button in the bottom right of your screen, and click.\n";
	waitForClick(); //waits for a mouse click
	waitForRelease();
	GetCursorPos(&mouse); //fetches the cursor location from the mouse struct
	pos << mouse.x << "," << mouse.y << "," << "keyboard" << endl;
	ZeroMemory(&mouse, sizeof(mouse));
	while (incrementer < sizeof(alpha))
	{
		cout << "Click on " << alpha[incrementer] << endl;
		waitForClick();
		waitForRelease();
		GetCursorPos(&mouse);
		pos << mouse.x << "," << mouse.y << "," << alpha[incrementer] << endl; //adds a line item to the csv file: [x,y,letter]
		ZeroMemory(&mouse, sizeof(mouse));
		incrementer++;
		Sleep(10);
	}
	cout << "Click on Shift" << endl;
	waitForClick();
	waitForRelease();
	GetCursorPos(&mouse);
	pos << mouse.x << "," << mouse.y << "," << "Shift" << endl;
	ZeroMemory(&mouse, sizeof(mouse));
	Sleep(10);
	cout << "Click on Shift again to release it" << endl; //an extra click so the user can disable the shift button so they can see what they're doing
	waitForClick();
	waitForRelease(); //no pointer tracking required here
	incrementer = 1;
	while (incrementer < 6)
	{
		cout << "Click on " << special[incrementer] << endl;
		waitForClick();
		waitForRelease();
		GetCursorPos(&mouse);
		pos << mouse.x << "," << mouse.y << "," << special[incrementer] << endl;
		ZeroMemory(&mouse, sizeof(mouse));
		incrementer++;
		Sleep(10);
	}
	incrementer = 0;
	cout << "Go into the special characters menu, and use the numbers in that keyboard\n" << "for this portion of the setup.\n";
	while (incrementer < sizeof(num))
	{
		cout << "Click on " << num[incrementer] << endl;
		waitForClick();
		waitForRelease();
		GetCursorPos(&mouse);
		pos << mouse.x << "," << mouse.y << "," << num[incrementer] << endl;
		ZeroMemory(&mouse, sizeof(mouse));
		incrementer++;
		Sleep(10);
	}
	cout << "Click on the 'X' to close the on-screen keyboard" << endl;
	waitForClick();
	waitForRelease();
	GetCursorPos(&mouse);
	pos << mouse.x << "," << mouse.y << "," << "Exit" << endl;
	ZeroMemory(&mouse, sizeof(mouse));
	Sleep(10);
	cout << "Done!" << endl;
	pos.close();
}

int xCoordinates(int key)
{
	incrementer = 0;
	index = 0;
	pos.open("pos.csv", ios::in); //opens the csv in ::in mode
	while (pos >> line && key >= index) //gets the line from the csv file, puts it into 'line'
	{
		//getline(pos, line); //gets the first line from the csv file.
		//format: getline(file to pull from, target string variable to put into, optional delinator);
		//see 'stringstream' object info below
		stringstream stream(line); //converts the string object 'line' into a stream (like cin, cout, fstream, etc) object 'stream'. needed for getline()
		while (getline(stream, line2, ',')) //while the line 'line' (passed into object 'stream') still has values in it, pass the values to 'line2'
		{
			values[incrementer] = line2; //set the current value of 'line2' to the appropriate value in the 'values' string array
			incrementer++;
		}
		index++;
		incrementer = 0;
	}
	stringstream convert(values[0]); //converts the 'values' strings from 'pos' to a stringstream 'convert'
	convert >> ints[0]; //streams the 'convert' stringstream into the 'ints' array slot
	pos.close();
	return ints[0];
}

int yCoordinates(int key)
{
	incrementer = 0;
	index = 0;
	pos.open("pos.csv", ios::in); //opens the csv in ::in mode
	while (pos >> line && key >= index) //gets the line from the csv file, puts it into 'line'
	{
		//getline(pos, line); //gets the first line from the csv file.
		//format: getline(file to pull from, target string variable to put into, optional delinator);
		//see 'stringstream' object info below
		stringstream stream(line); //converts the string object 'line' into a stream (like cin, cout, fstream, etc) object 'stream'. needed for getline()
		while (getline(stream, line2, ',')) //while the line 'line' (passed into object 'stream') still has values in it, pass the values to 'line2'
		{
			values[incrementer] = line2; //set the current value of 'line2' to the appropriate value in the 'values' string array
			incrementer++;
		}
		index++;
		incrementer = 0;
	}
	stringstream convert(values[1]); //converts the 'values' strings from 'pos' to a stringstream 'convert'
	convert >> ints[1]; //streams the 'convert' stringstream into the 'ints' array slot
	pos.close();
	return ints[1];
}

//======================================================================================================================================================//

void createMapFile(int _map)
{
	if (_map == 0)
	{
		map.open("map.csv", ios::out);
		map << buttons[0] << "," << "0xAF" << "," << "325" << endl; //volume up
		map << buttons[1] << "," << "0xAE" << "," << "300" << endl; //volume down
		map << buttons[2] << "," << "0xAD" << "," << "400" << endl; //volume mute
		map << buttons[3] << "," << "0xB3" << "," << "400" << endl; //play/pause
		map << buttons[4] << "," << "0x5B" << "," << "400" << endl; //left windows key
		map << buttons[5] << "," << "0x1B" << "," << "400" << endl; //esc
		map << buttons[6] << "," << "0x04" << "," << "400" << endl; //middle mouse click
		map << buttons[7] << "," << "-0x03" << "," << "100" << endl; //show cursor
		map << buttons[8] << "," << "0xA6" << "," << "350" << endl; //browser back
		map << buttons[9] << "," << "0xA7" << "," << "350" << endl; //browser forward
		map << buttons[10] << "," << "0x01" << "," << "250" << endl; //left click
		map << buttons[11] << "," << "0x02" << "," << "250" << endl; //right click
		map << buttons[12] << "," << "0x0D" << "," << "300" << endl; //enter
		map << buttons[13] << "," << "-0x01" << "," << "100" << endl; //keyboard trigger button
		map << buttons[14] << "," << "0x20" << "," << "225" << endl; //space
		map << buttons[15] << "," << "0x08" << "," << "275" << endl; //backspace
	}
	if (_map == 1)
	{
		map.open("map2.csv", ios::out);
		map << buttons[0] << "," << "0x26" << "," << "275" << endl; //up arrow
		map << buttons[1] << "," << "0x28" << "," << "275" << endl; //down arrow
		map << buttons[2] << "," << "0x25" << "," << "275" << endl; //left arrow
		map << buttons[3] << "," << "0x27" << "," << "275" << endl; //right arrow
		map << buttons[4] << "," << "0" << "," << "100" << endl;
		map << buttons[5] << "," << "0" << "," << "100" << endl;
		map << buttons[6] << "," << "0" << "," << "100" << endl;
		map << buttons[7] << "," << "0" << "," << "100" << endl;
		map << buttons[8] << "," << "0" << "," << "100" << endl;
		map << buttons[9] << "," << "0" << "," << "100" << endl;
		map << buttons[10] << "," << "0x01" << "," << "175" << endl; //select
		map << buttons[11] << "," << "0x10" << "," << "350" << endl; //shift
		map << buttons[12] << "," << "0x0D" << "," << "400" << endl; //enter and close
		map << buttons[13] << "," << "-0x01" << "," << "100" << endl; //keyboard trigger button
		map << buttons[14] << "," << "0x20" << "," << "300" << endl; //space
		map << buttons[15] << "," << "0x08" << "," << "200" << endl; //backspace
	}
	map.close();
}

int keyMappings(int _button, int _map) //the problem with the handleButtons() function is somewhere in here
{
	if (_map == 0) map.open("map.csv", ios::in);
	else if (_map == 1) map.open("map2.csv", ios::in);
	else return 0;
	incrementer = 0;
	index = 0;
	while (map >> line && _button >= index) //gets the line from the csv file, puts it into 'line'
	{
		//getline(pos, line); //gets the first line from the csv file.
		//format: getline(file to pull from, target string variable to put into, optional delinator);
		//see 'stringstream' object info below
		stringstream stream(line); //converts the string object 'line' into a stream (like cin, cout, fstream, etc) object 'stream'. needed for getline()
		while (getline(stream, line2, ',')) //while the line 'line' (passed into object 'stream') still has values in it, pass the values to 'line2'
		{
			values[incrementer] = line2; //set the current value of 'line2' to the appropriate value in the 'values' string array
			incrementer++;
		}
		index++;
		incrementer = 0;
	}
	ints[1] = stoi(values[1], 0, 16); //stoi - string-to-integer. converts the string values[1] to an integer, and puts it in ints[1]. values[1] is in base-16 (hex)
	map.close();
	return ints[1];
}

int repeatDelays(int _button, int _map)
{
	if (_map == 0) map.open("map.csv", ios::in);
	else if (_map == 1) map.open("map2.csv", ios::in);
	else return 0;
	incrementer = 0;
	index = 0;
	while (map >> line && _button >= index) //gets the line from the csv file, puts it into 'line'
	{
		//getline(pos, line); //gets the first line from the csv file.
		//format: getline(file to pull from, target string variable to put into, optional delinator);
		//see 'stringstream' object info below
		stringstream stream(line); //converts the string object 'line' into a stream (like cin, cout, fstream, etc) object 'stream'. needed for getline()
		while (getline(stream, line2, ',')) //while the line 'line' (passed into object 'stream') still has values in it, pass the values to 'line2'
		{
			values[incrementer] = line2; //set the current value of 'line2' to the appropriate value in the 'values' string array
			incrementer++;
		}
		index++;
		incrementer = 0;
	}
	ints[2] = stoi(values[2], 0, 10); //stoi - string-to-integer. converts the string values[1] to an integer, and puts it in ints[1].
	map.close();
	return ints[2];
}

/*
A stringstream is similar to input/output
file stream. We need to declare a stringstream
just like an fstream, for example:
stringstream ss;

and, like an fstream or cout, we can write to it:
ss << myString; or
ss << myCstring; or
ss << myInt;, or float, or double, etc.

and we can read from it:
ss >> myChar; or
ss >> myCstring; or
ss >> myInt;

TL;DR -- it's a convenient way to manipulate strings
Other info - you can look into stoi() and atoi() functions
Source: https://www.geeksforgeeks.org/converting-strings-numbers-cc/
*/

//=====================================================================================================================================================//

void initFiles()
{
	pos.open("pos.csv");
	if (!pos)
	{
		cout << "Keyboard position (pos.csv) file doesn't exist. The program will not work correctly without it.\n";
		cout << "Remaking keyboard position file...\n";
		pos.open("pos.csv", ios::out);
		if (!pos)
		{
			cout << "Error creating the file!\n";
			return;
		}
		cout << "Make sure to have the Touch Keyboard button available on the taskbar before starting.\n";
		cout << "Press [Enter] when you are ready to begin.";
		while (cin.get() != '\n')
		{
			Sleep(1);
		}
		cout << endl;
		createPosFile();
	}
	pos.close();
	map.open("map.csv");
	if (!map)
	{
		cout << "Button mapping file 1 (map.csv) doesn't exist. The program will not work correctly without it.\n";
		cout << "This button mapping file will be used when the on-screen keyboard is not up.\n";
		cout << "Remaking button mapping file...\n";
		map.open("map.csv", ios::out);
		if (!map)
		{
			cout << "Error creating the file!\n";
			return;
		}
		map.close();
		createMapFile(0);
	}
	map.close();
	map.open("map2.csv");
	if (!map)
	{
		cout << "Button mapping file 2 (map2.csv) doesn't exist. The program will not work correctly without it.\n";
		cout << "This button mapping file will be used when the on-screen keyboard is active.\n";
		cout << "Remaking button mapping file...\n";
		map.open("map2.csv", ios::out);
		if (!map)
		{
			cout << "Error creating the file!\n";
			return;
		}
		map.close();
		createMapFile(1);
	}
	map.close();
	cout << "All files are present.\n";
	cout << "To edit the button mapping files, open them in Notepad.\n";
}