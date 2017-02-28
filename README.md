# dropControl
Arduino sketch for drop controller.

Original idea, circuits and sketchs developed by Martyn Currey.
Full Information about drop controller you can find on the [drop controller site](http://www.dropcontroller.com/dropcontroller-2/)


## a_dropControl v2_01
Martyn Currey no longer maintain this version of the code.

The Arduino sketch is an ino file and you will need to use the Arduino IDE to compile it and then upload it to an Arduino
The sketch has 7 tabs:
* dropControl
* Functions
* Create_Menu_strings
* Eeprom
* Menu
* Make_Drops
* Keypad
The main sketch is in the dropControl tab.

The dropController sketch uses the following Arduino libraries:
* EEPROM. Included in the Arduino installation
* Wire. Included in the Arduino installation
* Button. [Download from here](http://arduino-info.wikispaces.com/file/view/Button.zip/405390486/Button.zip)
* digital IO performance. [Download from here](https://github.com/projectgus/digitalIOPerformance)
* LiquidCrystal_I2C. [I used the version from here](https://bitbucket.org/fmalpartida/new-liquidcrystal/wiki/Home). The LCD page on the [Arduino Wiki](http://arduino-info.wikispaces.com/LCD-Blue-I2C) is very helpful.
* MemoryFree. [Available from here](https://github.com/sudar/MemoryFree).

Libraries must be installed in the correct Arduino folder before they can used. For more information have a look [here](http://arduino.cc/en/Guide/Libraries) and [here](http://arduino-info.wikispaces.com/Arduino-Libraries).
