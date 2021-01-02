# RemoteControl
Control your Windows PC with an Xbox 360/One controller!

Instructions for using the Config CSV files:
------------------------------------------------------------------
In the map*.csv config files, the columns are organized in the order of button, action, repeat time, where the button is the button pressed on the Xbox controller that results in that action, and the action is a key on the list at the URL below. All "Action" column values are hexadecimal virtual key codes, but there are a few "special" key codes, and they are listed below:

No action: 0
Open/Close Virtual Keyboard: -0x01
Disable the Controller: -0x02
Show Where the Mouse Is: -0x03

Virtual Key Codes: https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
