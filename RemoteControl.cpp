#include "includes.h"
#include "keyboard.h"
#include "mouse.h"
#include "buttons.h"

//screen dimensions: 1536 x 864

/*
TODO:
-handle triggers in the handleButtons() function -- DONE!
-handle repeated mouse clicks in the handleButtons() function (make them not repeat, and for the click to hold as the button is held) -- DONE!
-fix keyboard -- DONE!
-add a different controller layout for when the keyboard is up (arrow keys, shift, etc) -- DONE!
-fix symbol button on keyboard -- DONE!
-mouse speed variable
-mouse acceleration curve? might not be necessary, since the speed variable already exists
-page scrolling (with inputs and the scroll wheel) -- DONE!
-show where the mouse is (like in the windows settings) with r stick down
-enable/disable controller
-TEST!
*/

//0x101D - spi_setmousesonar

int main()
{
	initController();
	//ShowWindow(GetConsoleWindow(), SW_HIDE); //hides the terminal window (change to SW_RESTORE to show again)
	//SetConsoleTitle(TEXT("ControllerKeyboard"));
	cout << "RemoteControl!\n";
	cout << "----------------------------------------------------\n" << endl;
	//print the button mappings and info here
	cout << "----------------------------------------------------\n" << endl;

	now = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

	bool x[1] = { true };
	SystemParametersInfoA(SPI_SETMOUSESONAR, 0, (bool *) *x, SPIF_UPDATEINIFILE);
	Sleep(5000);
	ZeroMemory(&input, sizeof(INPUT));
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = VK_LCONTROL;
	//comment added here
	input.ki.dwFlags = 0; //0 for a keypress
	SendInput(1, &input, sizeof(INPUT));
	Sleep(100);

	input.ki.dwFlags = KEYEVENTF_KEYUP; //this or 0x0002 for a key up
	SendInput(1, &input, sizeof(INPUT));
	Sleep(100);
	
	cout << "Done";
	x[0] = false;
	SystemParametersInfoA(SPI_SETMOUSESONAR, 0, (bool *) *x, SPIF_UPDATEINIFILE);
	while (true);

	while (true)
	{
		handleKeyboard();
		handleMouse();
		handleScrolling();
		handleButtons();
		Sleep(2);
	}
	return 0;
}
