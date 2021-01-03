#include "includes.h"
#include "keyboard.h"
#include "mouse.h"
#include "buttons.h"

//screen dimensions: 1536 x 864

/*
TODO:
-show where the mouse is (like in the windows settings) with r stick down
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

	buttonTime.now = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
	mouseTime.now = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();

	initFiles();
	
	while (true)
	{
		while (handleDisable() == false)
		{
			handleKeyboard();
			handleMouse();
			handleScrolling();
			handleButtons();
			Sleep(1);
		}
		while (handleDisable() == true) Sleep(1); //while the disable button is still being pressed
		while (handleDisable() == false) Sleep(1); //while the disable button hasn't been pressed yet
		while (handleDisable() == true) Sleep(1); //while the disable button is being pressed
	}
	return 0;
}
