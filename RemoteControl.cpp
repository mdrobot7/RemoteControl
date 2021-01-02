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
-mouse speed variable -- DONE!!!
-mouse acceleration curve? might not be necessary, since the speed variable already exists -- DONE!!!!!
-page scrolling (with inputs and the scroll wheel) -- DONE!
-show where the mouse is (like in the windows settings) with r stick down
-enable/disable controller -- DONE!
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
