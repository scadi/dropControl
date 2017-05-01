/*new*/
/*
  _____   _____    ____   _____     _____   ____   _   _  _______  _____    ____   _      
 |  __ \ |  __ \  / __ \ |  __ \   / ____| / __ \ | \ | ||__   __||  __ \  / __ \ | |     
 | |  | || |__) || |  | || |__) | | |     | |  | ||  \| |   | |   | |__) || |  | || |     
 | |  | ||  _  / | |  | ||  ___/  | |     | |  | || . ` |   | |   |  _  / | |  | || |     
 | |__| || | \ \ | |__| || |      | |____ | |__| || |\  |   | |   | | \ \ | |__| || |____ 
 |_____/ |_|  \_\ \____/ |_|       \_____| \____/ |_| \_|   |_|   |_|  \_\ \____/ |______|
 
Created by Martyn Currey
www.martyncurrey.com

Device for controlling up to 3 solenoid valves.
Used for creating water drops for water drop collision photography.

Has two modes; Full Mode and Quick Mode
Full Mode allows up to 9 drops. Quick Mode allows up to 3 drops
Quick Mode uses a simplified screen layout

Most controllers I have looked at use a linear sequence; drop, delay, drop, delay, flash.   I found this to restrictive.
This sketch uses a start time and size for each drop. The drop stop time is then calculated from the size. 
This means specific delays are not required. It also means you can have two drops at the same time. 

At present there is only a very basic check to see if drops times overlap.
If the drop times on the same valve overlap "OL" is displayed on the LCD. 
There are no current checks to see if the drop times are in the correct order and it is possible to give drop 2 a start time before drop 1.
Although the system shouldn't break, the desired drop effect may not be obtained.





================================================================================
  _______  ____       _____    ____  
 |__   __|/ __ \     |  __ \  / __ \ 
    | |  | |  | |    | |  | || |  | |
    | |  | |  | |    | |  | || |  | |
    | |  | |__| |    | |__| || |__| |
    |_|   \____/     |_____/  \____/ 

LCD contrast

  
shutter open time. Set minimum duration.
If the total drop sequence time < minimum shutter open time, add delay after open shutter

Add 3 more solenoid vales (up to 6)

Edit drops. update stop time after edit start and edit size




================================================================================
  _____  _____  _   _   _____ 
 |  __ \|_   _|| \ | | / ____|
 | |__) | | |  |  \| || (___  
 |  ___/  | |  | . ` | \___ \ 
 | |     _| |_ | |\  | ____) |
 |_|    |_____||_| \_||_____/ 
                              
only one navigation keypad is required; either a digital keypad on pins 2 to 6, or an analogue keypad on pin A0

PINS:
PIN  2 -     - OK BTN
PIN  3 - PWM - RT BTN
PIN  4 -     - DN BTN
PIN  5 - PWM - UP BTN
PIN  6 -     - LF BTN
PIN  7 -     - SOL 1
PIN  8 -     - SOL 2
PIN  9 - PWM - SOL 3
PIN 10 - PWM - flash trigger
PIN 11 - PWM - camera trigger - shutter release
PIN 12 -     - camera trigger - focus
PIN 13 - 

pIN A0 [14] -  Analogue Keypad
PIN A1 [15] - 
PIN A2 [16] - 
PIN A3 [17] - Start / Pause / Remote button
PIN A4 [18] - LCD
PIN A5 [19] - LCD
PIN A6 [20] - 
PIN A7 [21] - 


================================================================================
  ______  ______  _____   _____    ____   __  __ 
 |  ____||  ____||  __ \ |  __ \  / __ \ |  \/  |
 | |__   | |__   | |__) || |__) || |  | || \  / |
 |  __|  |  __|  |  ___/ |  _  / | |  | || |\/| |
 | |____ | |____ | |     | | \ \ | |__| || |  | |
 |______||______||_|     |_|  \_\ \____/ |_|  |_|
                                                 


The settings and drop times can be stored in eeprom memory

Settings are automatically saved when the user edits them.
Drop data has to be manually saved.

There are 3 memory areas for quick mode and 3 memory areas for full mode.
The user can set a default memory area which is used to auto load drop data when the device starts.


         Required   Start   End   Allocated
            Bytes 
Common       13        0     13     20
QM1          66       20    109     89
QM2          66      110    199     89
QM3          66      200    289     89
FM1         174      300    499    199
FM2         174      500    699    199
FM3         174      700    899    199

                  Type   Size   Start     End      
FM Set 1          U int    2     1000    1001      0=empty   >0 = number of time written
FM Set 2          U int    2     1002    1003      0=empty   >0 = number of time written
FM Set 3          U int    2     1004    1005      0=empty   >0 = number of time written
QM Set 1          U int    2     1006    1007      0=empty   >0 = number of time written
QM Set 2          U int    2     1008    1009      0=empty   >0 = number of time written
QM Set 3          U int    2     1010    1011      0=empty   >0 = number of time written

FM default mem    Byte     1     1021    1021      1,2,or 3 - the default user data set - used to load the user data on start
QM default mem    Byte     1     1022    1022      1,2,or 3 - the default user data set - used to load the user data on start
initialised flag   Byte     1     1023    1023      
settings flag     byte     1       19      19

QM = Quick mode
FM = Full mode

================================================================================

*/










// software version number  2.01
// Binary sketch size:  Digital keypad 28,856 bytes.  
//                      Analogue keypad 29,132 bytes

const char ver[5] = "2.01";


//#define DEBUG

// comment out the type of keypad you are not using
#define KEYPAD_ANALOGUE
//#define KEYPAD_DIGITAL



#include <MemoryFree.h>
//#include <digitalIOPerformance.h>
#include <EEPROM.h>
#include <Wire.h>  
#include <LiquidCrystal.h>
LiquidCrystal lcd(8,9,4,5,6,7);  // Set the LCD I2C address

// Alexander Brevig's Button library
// used for the digital keypad and the start button
//#include <Button.h>
//  Button rm_button = Button(17,PULLDOWN);  //A3


//#ifdef KEYPAD_DIGITAL
  // Set pins for button switches
  /*Button ok_button = Button(2,PULLDOWN);
  Button rt_button = Button(3,PULLDOWN);
  Button dn_button = Button(4,PULLDOWN);
  Button up_button = Button(5,PULLDOWN);
  Button lf_button = Button(6,PULLDOWN);*/
//#endif


#ifdef KEYPAD_ANALOGUE
  // defines for setting and clearing register bits
  // Used later in the setup() function to change the ADC prescaler to 16. Speeds up the analogRead() function which is used by the analogue keypad
  // Only the keypad uses the ADC.
  #ifndef cbi
    #define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
  #endif
  #ifndef sbi
    #define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
  #endif
#endif




/*
================================================================================
   _____  ____   _   _   _____  _______         _   _  _______  _____  
  / ____|/ __ \ | \ | | / ____||__   __| /\    | \ | ||__   __|/ ____|   
 | |    | |  | ||  \| || (___     | |   /  \   |  \| |   | |  | (___    
 | |    | |  | || . ` | \___ \    | |  / /\ \  | . ` |   | |   \___ \   
 | |____| |__| || |\  | ____) |   | | / ____ \ | |\  |   | |   ____) | 
  \_____|\____/ |_| \_||_____/    |_|/_/    \_\|_| \_|   |_|  |_____/  

*/


// attached devices
const byte numSolenoids   =  1;
const byte camera  = 1;
const byte focus   = 2;
const byte flash   = 3;


// Pins
const byte sol1TriggerPin =  13; 
//const byte sol2TriggerPin =  8; 
//const byte sol3TriggerPin =  9; 
const byte flashTrigPin   = 10;
const byte camTrigSHPin   = 11; 
const byte camTrigFCPin   = 12; 

// Analogue keypad
const byte analogueKeypadPin     = A0;

const byte BUTTON_RIGHT  = 0;
const byte BUTTON_UP     = 1;
const byte BUTTON_DOWN   = 2;
const byte BUTTON_LEFT   = 3;
const byte BUTTON_SELECT = 4;
const byte BUTTON_NONE   = 5;

// flags
const byte YES_NO       = 1;
const byte ON_OFF       = 2;
const byte BEFORE_AFTER = 3;
const byte INC_DEC      = 4;
const byte QM_FM        = 5;

// eeprom 
const int  eep_INIT_FLAG_ADDRESS               = 1023;           // a value of 99 means the eeprom has been initialized
const int  eep_HAVE_SETTINGS_FLAG_ADDRESS      = 19;             // a value of 99 means there are settings saved in the eeprom
const int  eep_DEFAULT_SAVED_DATA_ADDRESS[2]   = {1021,1022};    // the user can select a default saved data set
const int  eep_SETTINGS_START_ADDRESS          = 0;

// This is the number of times a memory location has been used. This value is used in the load/save menu and displayed in square brackets
// serves no real purpose
const int  eep_NUM_TIMES_WRITTEN_ADDRESS[2][4] =  { {0000, 1000, 1002, 1004},  {0000, 1006, 1008, 1010} };  // [0] not used. Wasteful but means I don't need to -1 everything
const int  eep_MEM_LOCATIONS_ADDRESS[2][4]     =  { {   0,   20,  220,  420},  {   0,  620,  710,  800} };  // [0] not used. Wasteful but means I don't need to -1 everything


// general values
const byte LOAD_DATA          = 0;
const byte SAVE_DATA          = 1;
const byte SHOW_SCREEN        = 1;
const byte DO_NOT_SHOW_SCREEN = 0;
const byte FULL_MODE          = 0;
const byte QUICK_MODE         = 1;
const byte YES                = true;
const byte NO                 = false;




         
/*
================================================================================
    _____  _       ____   ____            _       __      __        _____    _____ 
   / ____|| |     / __ \ |  _ \    /\    | |      \ \    / / /\    |  __ \  / ____|
  | |  __ | |    | |  | || |_) |  /  \   | |       \ \  / / /  \   | |__) || (___  
  | | |_ || |    | |  | ||  _ <  / /\ \  | |        \ \/ / / /\ \  |  _  /  \___ \ 
  | |__| || |____| |__| || |_) |/ ____ \ | |____     \  / / ____ \ | | \ \  ____) |
   \_____||______|\____/ |____//_/    \_\|______|     \/ /_/    \_\|_|  \_\|_____/ 

*/

// Menu
byte    numMenuItems           = 0;           // number of elements in current menu
char    menuList[14][21]       = { " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " "," "  };                       // used to hold the menus
boolean menuItemSelectable[14] = {false, true, true, true, true, true, true, true, true, true, true, true, true, true };   //  not yet implemented
char    charMenu[6]            = "\0";        // holds the current menu selection. This could be changed to a number/byte to save space.
byte    selected               = 1;           // selected menu element
byte    old_Selected[]         = {0,0,0,0};   // used to remember the place in the previous menu. Allows me to jump back to the previous selection 
const byte rightArrow          = 126;         // used in the menu
boolean drawScreen             = true;        // used inside the menu function. Redraw the screen if true. Used when the menu line has been changed and needs redrawing.
byte    arrowPos               = 1;           // vertical position of the arrow
byte currentItem               = 0;
byte topPos                    = 0;           // value of the menu item at the top of the screen

char numberString[6]           = "";         // used to convert a number to a string for displaying on the LCD
const char tenSpaces[11]       = "          ";  // I might move this to program memory later



// mirrorLockup. When set will issue a pre-sequence shutter trigger to activate the mirror lockup on the camera. 
// The delay is how long to wait after the lockup. This allows vibrations in the camera to stop
boolean mirrorLockup           = true;
unsigned int mirrorLockupDelay = 1000;

// This is an extra delay after the drops are done. Displays the old values on the LCD and allows me time to record the data for the drop just done. 
boolean showOldValues = true;

// Trigger duration
int  camTriggerDuration      = 50;        // Time the camera trigger is active
int  flashTriggerDuration    = 50;        // Time the flash trigger is active. Need to experiment with different times. I assume shorter is better but do not really know.



boolean repeatLoop[]        = {true,true};
long    loopDelay[]         = {0,0};
boolean usingRemoteSwitch   = true;

// loopDelay and repeatLoop -
// [0] for FULLMODE. [1] for QUICKMODE  - I currently do not use the array. Both FM and QM use the first element only.
// Left the array here in case I want to change it back later

// when repeatLoop[] is true the system will keep repeating the drop sequence
// loopDelay[] is the delay between loops
// if loopDelay[] == 0 then the system will pause until the remote button is pressed







/*
================================================================================
   _____  ______  _______  _______  _____  _   _   _____   _____ 
  / ____||  ____||__   __||__   __||_   _|| \ | | / ____| / ____|
 | (___  | |__      | |      | |     | |  |  \| || |  __ | (___  
  \___ \ |  __|     | |      | |     | |  | . ` || | |_ | \___ \ 
  ____) || |____    | |      | |    _| |_ | |\  || |__| | ____) |
 |_____/ |______|   |_|      |_|   |_____||_| \_| \_____||_____/ 
                                                                 
Variables used for the settings + default values


  _____   ______  ______         _    _  _    _______  __      __        _      _    _  ______   _____ 
 |  __ \ |  ____||  ____| /\    | |  | || |  |__   __| \ \    / / /\    | |    | |  | ||  ____| / ____|
 | |  | || |__   | |__   /  \   | |  | || |     | |     \ \  / / /  \   | |    | |  | || |__   | (___  
 | |  | ||  __|  |  __| / /\ \  | |  | || |     | |      \ \/ / / /\ \  | |    | |  | ||  __|   \___ \ 
 | |__| || |____ | |   / ____ \ | |__| || |____ | |       \  / / ____ \ | |____| |__| || |____  ____) |
 |_____/ |______||_|  /_/    \_\ \____/ |______||_|        \/ /_/    \_\|______|\____/ |______||_____/ 


Device default drop values . 
*/

byte numDrops[2]             = {2,2};
long camTrigStart[2]         = {0,0};        // shutter release delay. Shutter is activated at the start of the drop sequence 0 = bulb mode
long flashTrigStart[2]       = {500,520};    // flash trigger delay/start time.  
int  camTrigInc[2]           = {0,0};        // how much time to add/subtract to the camera tigger delay every loop.  Uses incDecFlag[] to determine inc or dec
int  flashTrigInc[2]         = {0,0};        // how much time to add/subtract to the flash tigger delay every loop.   Uses incDecFlag[] to determine inc or dec
   
byte mode = QUICK_MODE; 



//Multidrop values

// Drop size and delays -  a maximum of 9 drops in full mode. Max 3 drops in quick mode
// [0] not used
// [1]-[9] used for full mode
// [10]-[12] used for quick mode


long dropStartTime[13]     = {0,  100, 200, 300,   0,   0,   0,   0,   0,   0, 100, 200, 300};     // start times in millisecond 
long dropSize[13]          = {0,   50,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50, 50};      // dropSize in millisecond - up to 9 drops
byte sol[13]               = {0,    1,   1,   2,   1,   1,   1,   1,   1,   1,    1,  1,  2};      // the solenoid to use for the drop; 1,2 or 3

// The inc times are added or subtracted to the drop times after every sequence when using repeatLoop[]
// incDecFlag: if inc - add the values. If dec - subtract the value.
// The original drop times are not changed.
long incDropStartTime[13]  = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};    
long incSize[13]           = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 
byte incDecFlag[13]        = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 





/*
=====================================================================================================================================
  __  __  ______  _   _  _    _   _____ 
 |  \/  ||  ____|| \ | || |  | | / ____|
 | \  / || |__   |  \| || |  | || (___  
 | |\/| ||  __|  | . ` || |  | | \___ \ 
 | |  | || |____ | |\  || |__| | ____) |
 |_|  |_||______||_| \_| \____/ |_____/ 
                                        
Not all the menus are stored in PROGMEM. Some are created on the fly based on drop data                                        
*/


// common menu values.  Also used for About
PROGMEM const char common_0[] = ""; 
PROGMEM const char common_1[] = "Start";
PROGMEM const char common_2[] = "Exit";
PROGMEM const char* const common[] = { common_0, common_1, common_2 };


PROGMEM const char about_00[] = "About"; 
PROGMEM const char about_01[] = "dropControl";
PROGMEM const char about_02[] = "By Martyn Currey";
PROGMEM const char about_03[] = "martyncurrey.com";
PROGMEM const char about_04[] = "Version: ";
PROGMEM const char about_05[] = "Date: ";
PROGMEM const char about_06[] = "free mem: ";
PROGMEM const char* const about[] = { about_00, about_01, about_02, about_03, about_04, about_05, about_06, common_2 };


// Main Menu
// will display Full Mode or Quick mode depending on the mode.
PROGMEM const char menu_00[] = "Main Menu";
PROGMEM const char menu_01[] = "Quick mode";
PROGMEM const char menu_02[] = "Load/Save";
PROGMEM const char menu_03[] = "Drop information";
PROGMEM const char menu_04[] = "Drain valves";
PROGMEM const char menu_05[] = "Settings";
PROGMEM const char menu_06[] = "About";
PROGMEM const char menu_07[] = "Full mode";
PROGMEM const char* const menu_main[] = { menu_00, menu_01, menu_02, menu_03, menu_04, menu_05, menu_06, menu_07 };



// QUICK MODE MENU
//PROGMEM const char menu_10[] = "QUICK MODE";
PROGMEM const char menu_11[] = "1 Drop";
PROGMEM const char menu_12[] = "2 Drop";
PROGMEM const char menu_13[] = "3 Drop";
PROGMEM const char* const menu_quickMode[] = { menu_01, menu_11, menu_12, menu_13, common_2 };

//PROGMEM const char menu_101[] = "1DROP  DP S STRT SZE";
PROGMEM const char menu_101[] = "1D   S STRT SZE STOP";
PROGMEM const char menu_102[] = "2D   S STRT SZE STOP";
PROGMEM const char menu_103[] = "3D   S STRT SZE STOP";
PROGMEM const char* const menu_quickModeHeaders[] = {  menu_101, menu_102, menu_103 };

// FULL MODE MENU
PROGMEM const char menu_20[] = "Full mode  ";
PROGMEM const char menu_21[] = "Num of drops = ";
PROGMEM const char menu_22[] = "Edit drop times";
PROGMEM const char menu_23[] = "Edit inc times";
PROGMEM const char menu_24[] = "Edit trigger times";
PROGMEM const char* const menu_fullMode[] = {  menu_20, menu_21, menu_22, menu_23, menu_24, common_1, common_2 };

PROGMEM const char menu_40[] = "Set triggers";
PROGMEM const char menu_41[] = "CAM start:     ";
PROGMEM const char menu_42[] = "CAM start inc:  ";
PROGMEM const char menu_43[] = "FLSH start:    ";
PROGMEM const char menu_44[] = "FLSH start inc: ";
PROGMEM const char* const menu_TD[] = { menu_40, menu_41, menu_42, menu_43, menu_44, common_2 };  

//SETTINGS
//PROGMEM const char menu_50[] = "SETTINGS";
PROGMEM const char menu_51[] = "Mode:         ";
PROGMEM const char menu_52[] = "Loop:           ";
PROGMEM const char menu_53[] = "Loop delay:    ";
PROGMEM const char menu_54[] = "Mir lock:       ";
PROGMEM const char menu_55[] = "Mir lock len:  ";
PROGMEM const char menu_56[] = "CAM trig len:   ";
PROGMEM const char menu_57[] = "FLSH trig len:  ";
PROGMEM const char* const menu_set[] = { menu_05, menu_51, menu_52, menu_53, menu_54, menu_55, menu_56, menu_57, common_2 };  

// DRAIN MENU
// I have just added manual drain switches and may not need this menu anymore.
//PROGMEM const char menu_60[] = "DRAIN";
PROGMEM const char menu_61[] = "Valve 1";
PROGMEM const char menu_62[] = "Valve 2";
PROGMEM const char menu_63[] = "Valve 3";
PROGMEM const char* const menu_drain[] = { menu_04, menu_61, menu_62, menu_63, common_2 };         

// LOAD/SAVE MENU
//PROGMEM const char menu_70[] = "LOAD/SAVE";
PROGMEM const char menu_71[] = "Load drop data";
PROGMEM const char menu_72[] = "Save drop data";
PROGMEM const char menu_73[] = "Set default";
PROGMEM const char menu_74[] = "Erase saved data";
PROGMEM const char* const menu_loadSave[] = { menu_02, menu_71, menu_72, menu_73, menu_74, common_2 };  

// ERASE MEMORY
PROGMEM const char menu_80[] = "Erase memory";
PROGMEM const char menu_81[] = "Are you sure?";
PROGMEM const char menu_82[] = "Yes";
PROGMEM const char menu_83[] = "No";
PROGMEM const char* const menu_erase[] = { menu_80, menu_81, menu_82, menu_83 }; 




//=====================================================================================================================================









/*
   _____  ______  _______  _    _  _____  
  / ____||  ____||__   __|| |  | ||  __ \ 
 | (___  | |__      | |   | |  | || |__) |
  \___ \ |  __|     | |   | |  | ||  ___/ 
  ____) || |____    | |   | |__| || |     
 |_____/ |______|   |_|    \____/ |_|     

*/

void setup() 
{
        //If we are debugging open the serial monitor
        #ifdef DEBUG
             Serial.begin(9600);
             while (!Serial)   { ; }
        #endif 


        // if using an analogue keypad change the ADC prescaler. Speeds up the ADC
        #ifdef KEYPAD_ANALOGUE
            // set prescale to 16. Used to speed up analogRead(). Only the keypad uses analogRead()
            sbi(ADCSRA,ADPS2) ;
            cbi(ADCSRA,ADPS1) ;
            cbi(ADCSRA,ADPS0) ;  
        #endif 
        
        // define the pins and set the pins to low
        pinMode(camTrigSHPin, OUTPUT);    digitalWrite(camTrigSHPin, LOW); 
        pinMode(camTrigFCPin, OUTPUT);    digitalWrite(camTrigSHPin, LOW); 
        pinMode(flashTrigPin, OUTPUT);    digitalWrite(flashTrigPin, LOW); 
        pinMode(sol1TriggerPin, OUTPUT);  digitalWrite(sol1TriggerPin, LOW); 
        //pinMode(sol2TriggerPin, OUTPUT);  digitalWrite(sol2TriggerPin, LOW); 
        //pinMode(sol3TriggerPin, OUTPUT);  digitalWrite(sol3TriggerPin, LOW);
        
        // analogue keypad pin
        //  set analog pin for input and turn on the internal pull-up resistor 
        pinMode(analogueKeypadPin, INPUT_PULLUP); // 
        
        lcd.begin(20, 4);
        lcd.clear(); 
        //  custom character used in the timer animation - this is a "\". The "\" chr is not available in the LCD chr list
        byte line1[8] =  { B00000, B10000, B01000, B00100, B00010, B00001, B00000, B00000 };
        lcd.createChar(0, line1);
        
        //  custom character: vertical pointer for the menu 
        byte line2[8] =  { B00100, B01110, B10101, B00100, B10101, B01110, B00100, B00000 }; 
        lcd.createChar(1, line2);
        
        
        // LCD opening sequence
        delay(500);
        // "dropControl"
        strcpy_P(menuList[1], (char*)pgm_read_word(&about[1]) );
        lcd.setCursor(0, 0);   lcd.print( menuList[1] ); 
        delay(1500);
        
        // By Martyn Currey
        strcpy_P(menuList[1], (char*)pgm_read_word(&about[2]) );
        lcd.setCursor(0, 1);   lcd.print( menuList[1] );
        delay(1000);        
        
         // version
        strcpy_P(menuList[1], (char*)pgm_read_word(&about[4]) );
        lcd.setCursor(0, 2);   lcd.print( menuList[1] ); lcd.print( ver );
        delay(1000);         
        
        lcd.setCursor(0, 3);   
        #ifdef KEYPAD_ANALOGUE
            lcd.print( "Analogue Keypad" );
        #endif
        //#ifdef KEYPAD_DIGITAL
//            lcd.print( "Digital Keypad" );
  //      #endif        
        delay(1500);    
        
        
        
        
    
        // If eeprom location eep_INIT_FLAG_ADDRESS=99 then eeprom has already been initialised.   
        byte tmp = eepromReadByte(eep_INIT_FLAG_ADDRESS);

        if ( tmp != 99)
        {
            // initialize eeprom
            //lcd.clear();
            lcd.setCursor(0, 2);   lcd.print(F("Please wait"));
            lcd.setCursor(0, 3);   lcd.print(F("Initializing..."));

            // I don't really need to write to every memory location but I feel it will alleviate any possible problems due to random values.      
            int percent = 0;
            for (long i=0; i<=1023; i++)
            {
                eepromWriteByte(0,i);
                percent = i*100 / 1023;
                lcd.setCursor(15,3);  lcd.print( percent ); lcd.print( "% " );
            }
            // need to set the eeprom initialized flag
            eepromWriteByte(99, eep_INIT_FLAG_ADDRESS);  
            delay(750);
        }
        else
        {
             
             // see if have settings to load
             int tmp2 =  eepromReadByte(eep_HAVE_SETTINGS_FLAG_ADDRESS);
             if (tmp2 == 99) { loadSaveSettings(LOAD_DATA); }         
             
        }


        // clear lines 2 and 3.
        lcd.setCursor(0,2);  lcd.print(tenSpaces); lcd.print(tenSpaces);
        lcd.setCursor(0,3);  lcd.print(tenSpaces); lcd.print(tenSpaces);
        



        // Device defaults are already loaded.
        // mode has already been set as part of the device default values or user general settings.
        
        // If the left key is not pressed load the user default settings (if there are any)
        // The default number for the user data is stored in eeprom location eep_DEFAULT_SAVED_DATA_ADDRESS[mode]. If set will have a value of 1,2 or 3.
        
        
        int tmpDefaultSet = 0;
        boolean loadedOK = false;
        
        if (checkButtonPress(false)==BUTTON_LEFT )  
        {
            lcd.setCursor(0,3);  lcd.print(F("Default data loaded"));
        }
        else
        {
            tmpDefaultSet = eepromReadByte( eep_DEFAULT_SAVED_DATA_ADDRESS[mode] );
            
            if (tmpDefaultSet >0 and tmpDefaultSet <4)
            {
                    // is the num of times written > 0
                    tmp = eepromReadInt( eep_NUM_TIMES_WRITTEN_ADDRESS[mode][tmpDefaultSet] );
                    if (tmp > 0)
                    {
                        // we have data to load
                        loadedOK = loadSaveDropData( LOAD_DATA, mode, tmpDefaultSet, DO_NOT_SHOW_SCREEN );
                        if (loadedOK) { lcd.setCursor(0,3);  lcd.print(F("User data loaded"));      }
                        else          { lcd.setCursor(0,3);  lcd.print(F("Unable to load data!"));  }                
                    }
                    //  no saved data to load. Num times written is <=0  
                    else { lcd.setCursor(0,3);  lcd.print(F("Error E7")); }
            }
            // the default value is out of range. Should 1, 2, or 3
            else  { lcd.setCursor(0,3);  lcd.print(F("No default user data"));    } 
        }

         

        
         

        unsigned long startTime = 0;
        boolean done = false;
        startTime = millis();
        while (!done)
        {
                if ( (millis()- startTime)  > 2500    ) { done = true;}
             //   if ( rm_button.uniquePress() )    { done = true;   }

        }

        


        #ifdef DEBUG
               Serial.println( "Start"  );
        #endif 
 




        // start in the main menu
        strcpy(charMenu,"main");
}










/*
 __      __ ____   _____  _____      _        ____    ____   _____  
 \ \    / // __ \ |_   _||  __ \    | |      / __ \  / __ \ |  __ \ 
  \ \  / /| |  | |  | |  | |  | |   | |     | |  | || |  | || |__) |
   \ \/ / | |  | |  | |  | |  | |   | |     | |  | || |  | ||  ___/ 
    \  /  | |__| | _| |_ | |__| |   | |____ | |__| || |__| || |     
     \/    \____/ |_____||_____/    |______| \____/  \____/ |_|     
*/

void loop()
{






/*
  __  __            _____  _   _     __  __  ______  _   _  _    _ 
 |  \/  |    /\    |_   _|| \ | |   |  \/  ||  ____|| \ | || |  | |
 | \  / |   /  \     | |  |  \| |   | \  / || |__   |  \| || |  | |
 | |\/| |  / /\ \    | |  | . ` |   | |\/| ||  __|  | . ` || |  | |
 | |  | | / ____ \  _| |_ | |\  |   | |  | || |____ | |\  || |__| |
 |_|  |_|/_/    \_\|_____||_| \_|   |_|  |_||______||_| \_| \____/ 

on the first run oldSelected should = 0;
oldSelected is the previous menu selected item.
It allows me to go back to the previously selected item rather than going to the top of the previous menu
*/

  if ( strcmp(charMenu, "main" )==0 )
     {     

          // copy the menu from program memory to the menu array
          numMenuItems = 6;
          for (int i=0; i<numMenuItems+1; i++)
          {   strcpy_P(menuList[i], (char*)pgm_read_word(&menu_main[i]) ); 
          }
          //strcat(menuList[0], ver);
          
          // if in full mode copy "FULL MODE" to the menu
          if (mode==FULL_MODE) { strcpy_P(menuList[1], (char*)pgm_read_word(&menu_main[7])); }   
            

         // set which menu items are selectable
         for ( byte i=1; i<=numMenuItems; i++)
         {
              menuItemSelectable[i] = true; 
         }


          selected = 0;
          currentItem = 1; 
          if (old_Selected[0] >0) {  currentItem = old_Selected[0];    }
        
          drawScreen = true;
          selected = menuSelection();
          old_Selected[0] = selected;
          
          if (selected == 1) { if ( mode==FULL_MODE) { strcpy(charMenu,"FM");}  else  { strcpy(charMenu,"QM"); }   }  // Full mode or quick mode
          if (selected == 2) { strcpy(charMenu,"L/S"); }                                                              // Load Save menu 
          if (selected == 3) { strcpy(charMenu,"DI"); }                                                               // Drop Information
          if (selected == 4) { strcpy(charMenu,"drain"); }                                                            // Drain solenoids. Have left this in even though now have switches
          if (selected == 5) { strcpy(charMenu,"SET"); }                                                              // Settings
          if (selected == 6) { strcpy(charMenu,"about"); }                                                            // About 

     }   //  if ( strcmp(charMenu, "main" )==0 )  
 
 





/*
   ____   _    _  _____  _____  _  __   __  __   ____   _____   ______ 
  / __ \ | |  | ||_   _|/ ____|| |/ /  |  \/  | / __ \ |  __ \ |  ____|
 | |  | || |  | |  | | | |     | ' /   | \  / || |  | || |  | || |__   
 | |  | || |  | |  | | | |     |  <    | |\/| || |  | || |  | ||  __|  
 | |__| || |__| | _| |_| |____ | . \   | |  | || |__| || |__| || |____ 
  \___\_\ \____/ |_____|\_____||_|\_\  |_|  |_| \____/ |_____/ |______|
 
Quick mode allows easy use for 1 ,2, or 3 drops.
*/
  if ( strcmp(charMenu, "QM" )==0 )
  {
          
          // copy the menu from program memory to the menu array
          numMenuItems = 4;
          for (int i=0; i<numMenuItems+1; i++)
          {   strcpy_P( menuList[i], (char*)pgm_read_word(&menu_quickMode[i])); // Necessary casts and dereferencing. 
          }

         // set which menu items are selectable
         for ( byte i=1; i<=numMenuItems; i++)
         {
              menuItemSelectable[i] = true; 
         }


          selected = 0;
          currentItem = 1; 
          if (old_Selected[1] >0) {  currentItem = old_Selected[1];    }
        
          drawScreen = true; 
          selected = menuSelection();
          old_Selected[1] = selected;
          
          if (selected == 1) { strcpy(charMenu,"QM2");  numDrops[mode]=1;   }
          if (selected == 2) { strcpy(charMenu,"QM2");  numDrops[mode]=2;   }
          if (selected == 3) { strcpy(charMenu,"QM2");  numDrops[mode]=3;   }
          if (selected == 4) { strcpy(charMenu,"main"); old_Selected[1]=0;   } 
   
  }

 

  
/*
   ____    __  __   ___  
  / __ \  |  \/  | |__ \ 
 | |  | | | \  / |    ) |
 | |  | | | |\/| |   / / 
 | |__| | | |  | |  / /_ 
  \___\_\ |_|  |_| |____|
 
QUICK MODE 2
The previous menu selected the number of drops
This menu is for editing the drop data and starting the drop sequence
*/
  if ( strcmp(charMenu, "QM2" )==0 )
  {

       // create the menu elements
       strcpy_P( menuList[0], (char*)pgm_read_word( &menu_quickModeHeaders[numDrops[mode]-1]  ) ); 
       byte j=0;
       for (byte i=1; i <= numDrops[mode]; i++)
       {
            j=(i-1)*2 +1 ;
            createDropMenuString(j,i);       //  menuList[1] - drop1 times
            createIncMenuString(j+1,i,1);    //  menuList[2] - inc1 times
       }
            

       createFlashTriggerMenuLine( numDrops[mode]*2+1);                              // createFlashTriggerMenuLine(int menuRow)
       createCamTriggerMenuLine( numDrops[mode]*2+2); 
       strcpy_P( menuList[numDrops[mode]*2+3], (char*)pgm_read_word(&common[1]));    // START 
       strcpy_P( menuList[numDrops[mode]*2+4], (char*)pgm_read_word(&common[2]));    // EXIT
       
       numMenuItems = numDrops[mode]*2+4;

       // set which menu items are selectable
       for ( byte i=1; i<=numMenuItems; i++)
       {
            menuItemSelectable[i] = true; 
       }


       selected = 0;
       currentItem = 1; 
       if (old_Selected[2] >0) {  currentItem = old_Selected[2];    }

       drawScreen = true;
       boolean done = false; 
       while ( !done )
       {       
           selected = menuSelection();
           old_Selected[2] = selected;

           if (numDrops[mode]==1)
           {           

               if (selected == 1) { editDrop(1);                    createDropMenuString(1,1);     drawScreen = false;   }
               if (selected == 2) { editInc(1,arrowPos);            createIncMenuString(2,1,1);    drawScreen = false ;     } 
               if (selected == 3) { editFlashStartAndFlashInc(3);   drawScreen = false ;    } 
               if (selected == 4) { editCamStartAndCamInc(4);       drawScreen = false ;    } 
               if (selected == 5) { makeDrops();                    strcpy(charMenu,"QM2");        done=true; }
               if (selected == 6) { strcpy(charMenu,"QM");          old_Selected[2] = 0;           done=true; } 
           }
        
        
           else if (numDrops[mode]==2)
           {
               if (selected == 1) { editDrop(1);                      createDropMenuString(1,1);    drawScreen = false;  }
               if (selected == 3) { editDrop(2);                      createDropMenuString(3,2);    drawScreen = false;  }
               if (selected == 2) { editInc(1,arrowPos);              createIncMenuString(2,1,1);   drawScreen = false ;     }
               if (selected == 4) { editInc(2,arrowPos);              createIncMenuString(4,2,1);   drawScreen = false ;     }
               if (selected == 5) { editFlashStartAndFlashInc(5);   } 
               if (selected == 6) { editCamStartAndCamInc(6);       }   
               if (selected == 7) { makeDrops();                      strcpy(charMenu,"QM2");       done=true; }
               if (selected == 8) { strcpy(charMenu,"QM");            old_Selected[2] = 0;          done=true; }           
           }
           
           else if (numDrops[mode]==3)
           {
               if (selected == 1) { editDrop(1);                      createDropMenuString(1,1);    drawScreen = false;  }
               if (selected == 2) { editInc(1,arrowPos);              createIncMenuString(2,1,1);   drawScreen = false;  }
               if (selected == 3) { editDrop(2);                      createDropMenuString(3,2);    drawScreen = false;  }
               if (selected == 4) { editInc(2,arrowPos);              createIncMenuString(4,2,1);   drawScreen = false;  }
               if (selected == 5) { editDrop(3);                      createDropMenuString(5,3);    drawScreen = false;  }
               if (selected == 6) { editInc(3,arrowPos);              createIncMenuString(6,3,1);   drawScreen = false;  }           
               if (selected == 7) { editFlashStartAndFlashInc(7);   }
               if (selected == 8) { editCamStartAndCamInc(8);       }   
               if (selected == 9) { makeDrops();                      strcpy(charMenu,"QM2");       done=true; }
               if (selected == 10) { strcpy(charMenu,"QM");           old_Selected[2] = 0;          done=true; }
           }
        
       } // while ( !done )



  }   



/*
  ______  _    _  _       _         __  __   ____   _____   ______ 
 |  ____|| |  | || |     | |       |  \/  | / __ \ |  __ \ |  ____|
 | |__   | |  | || |     | |       | \  / || |  | || |  | || |__   
 |  __|  | |  | || |     | |       | |\/| || |  | || |  | ||  __|  
 | |     | |__| || |____ | |____   | |  | || |__| || |__| || |____ 
 |_|      \____/ |______||______|  |_|  |_| \____/ |_____/ |______|

Full mode allows for up to 9 drops
*/

  if ( strcmp(charMenu, "FM" )==0 )
     {     

          // check to see if any of the drop times (same solenoid) overlay
          boolean error = checkOverlap();
          
          if (numDrops[mode] >9) {numDrops[mode] = 9;}  // this may not be required.
          if (numDrops[mode] <1) {numDrops[mode] = 1;}
          
          // copy the menu from program memory to the menu array
          numMenuItems = 6;
          for (int i=0; i<numMenuItems+1; i++)
          {   strcpy_P(menuList[i], (char*)pgm_read_word(&menu_fullMode[i]));   }
          numberToString (numDrops[mode], 1);   strcat(menuList[1], numberString );
          
          if (error) {  strcat( menuList[0], "       OL" );   }


         // set which menu items are selectable
         for ( byte i=1; i<=numMenuItems; i++)
         {
              menuItemSelectable[i] = true; 
         }

          
          selected = 0;
          currentItem = 1; 
          if (old_Selected[1] >0) {  currentItem = old_Selected[1];  }  
          
          drawScreen = true;
          boolean done = false;
          while (!done)
          {
              selected = menuSelection();
              old_Selected[1] = selected;
              drawScreen = false;
              
             if (selected == 1) { changePointer2gt();   numDrops[mode] = (int)editNumber(numDrops[mode], 1,16, arrowPos, 1,9);  changePointer2arrow();  drawScreen = false;    }

             //  why like this?  Because I use the menu system to edit the drop times
             else if (selected == 2) { editDropTimings(numDrops[mode]);  old_Selected[1]=2;  strcpy(charMenu,"FM");   done=true;   }
             else if (selected == 3) { editIncTimes(numDrops[mode]);     old_Selected[1]=3;  strcpy(charMenu,"FM");   done=true;   }
             else if (selected == 4) { editTriggers();                   old_Selected[1]=4;  strcpy(charMenu,"FM");   done=true; }
             else if (selected == 5) { makeDrops();                      drawScreen = true;  }
             else if (selected == 6) { strcpy(charMenu,"main");          old_Selected[1] = 0;                         done=true;}
             
             if (selected >0 && selected <5 )
             {
                error = checkOverlap();
                lcd.setCursor(18,0);
                if (error) {  lcd.print(F( "OL" ));  }  
                else       {  lcd.print(F( "  " )); } 
             }

         } // done
  
  }  //  if ( strcmp(charMenu, "FM" )==0 )









 
/*
   _____  ______  _______  _______  _____  _   _   _____   _____ 
  / ____||  ____||__   __||__   __||_   _|| \ | | / ____| / ____|
 | (___  | |__      | |      | |     | |  |  \| || |  __ | (___  
  \___ \ |  __|     | |      | |     | |  | . ` || | |_ | \___ \ 
  ____) || |____    | |      | |    _| |_ | |\  || |__| | ____) |
 |_____/ |______|   |_|      |_|   |_____||_| \_| \_____||_____/

*/

  if ( strcmp(charMenu, "SET" )==0 )
  {

       // copy the menu from program memory to the menu array
       numMenuItems = 8;
       for (int i=0; i<numMenuItems+1; i++)    {   strcpy_P(menuList[i], (char*)pgm_read_word(&menu_set[i]));        }
       
       if (mode==FULL_MODE) { strcat(menuList[1], " FULL"); }     
       else                 { strcat(menuList[1], "QUICK"); }
       if (repeatLoop[0])   { strcat(menuList[2], "YES"); }        
       else                 { strcat(menuList[2], " NO"); }
       if (mirrorLockup)    { strcat(menuList[4], "YES"); }         
       else                 { strcat(menuList[4], " NO"); }
       numberToString (loopDelay[0], 4);          strcat(menuList[3], numberString );   
       numberToString (mirrorLockupDelay, 4);     strcat(menuList[5], numberString );
       numberToString (camTriggerDuration, 3);    strcat(menuList[6], numberString );
       numberToString (flashTriggerDuration, 3);  strcat(menuList[7], numberString );    

         // set which menu items are selectable
         for ( byte i=1; i<=numMenuItems; i++)
         {
              menuItemSelectable[i] = true; 
         }
 
       selected = 0;
       currentItem = 1;
       drawScreen = true;    
     
         while ( selected != numMenuItems)
         {       
              selected = menuSelection();
        
              drawScreen = false;
              if (selected == 1) 
              {    
                  changePointer2gt(); mode = editFlag(mode, QM_FM, 15, arrowPos);  changePointer2arrow(); 
                  //strcpy(menuList[1],"Mode:         "); 
                  strcpy_P(menuList[1], (char*)pgm_read_word(&menu_set[1]));    
                  if ( mode==FULL_MODE) { strcat(menuList[1], " FULL"); }    else   { strcat(menuList[1], "QUICK"); }  
              }  
              
              if (selected == 2) 
              { 
                   changePointer2gt(); repeatLoop[0] = editFlag(repeatLoop[0], YES_NO, 17, arrowPos); changePointer2arrow();  
                   //strcpy(menuList[2],"Loop:           ");
                   strcpy_P(menuList[2], (char*)pgm_read_word(&menu_set[2]));    
                   if (repeatLoop[0]) { strcat(menuList[2], "YES"); }   else  { strcat(menuList[2], " NO"); }  } 
        
              if (selected == 3) 
              { 
                  changePointer2gt(); loopDelay[0]  = editNumber(loopDelay[0], 4, 16, arrowPos,0,9999);   changePointer2arrow();  
                  //strcpy(menuList[3],"Loop delay:    "); 
                  strcpy_P(menuList[3], (char*)pgm_read_word(&menu_set[3]));   
                  numberToString (loopDelay[0], 4);    strcat(menuList[3], numberString ); 
              } 
              
              if (selected == 4) 
              { 
                   changePointer2gt(); mirrorLockup  = editFlag(mirrorLockup, YES_NO, 17, arrowPos);    changePointer2arrow();
                   strcpy_P(menuList[4], (char*)pgm_read_word(&menu_set[4]));
                   if (mirrorLockup) { strcat(menuList[4], "YES"); }         else { strcat(menuList[4], " NO"); }   
              } 
              
              if (selected == 5) 
              { 
                  changePointer2gt(); mirrorLockupDelay   =  editNumber(mirrorLockupDelay, 4, 16, arrowPos,0,9999);  changePointer2arrow();  
                  //strcpy(menuList[5],"Mir lock time: ");   
                  strcpy_P(menuList[5], (char*)pgm_read_word(&menu_set[5]));
                  numberToString (mirrorLockupDelay, 4);     strcat(menuList[5], numberString );  
              } 
              
              if (selected == 6) 
              { 
                  changePointer2gt(); camTriggerDuration  =  editNumber(camTriggerDuration, 3, 17, arrowPos,0,999);  changePointer2arrow();  
                  //strcpy(menuList[6],"Cam trig len:  ");  
                  strcpy_P(menuList[6], (char*)pgm_read_word(&menu_set[6]));
                  numberToString (camTriggerDuration, 3);    strcat(menuList[6], numberString );  
              } 
              
              if (selected == 7) 
              { 
                  changePointer2gt(); flashTriggerDuration = editNumber(flashTriggerDuration, 3, 17, arrowPos,0,999);changePointer2arrow();  
                  //strcpy(menuList[7],"Flsh trig len: ");
                  strcpy_P(menuList[7], (char*)pgm_read_word(&menu_set[7]));  
                  numberToString (flashTriggerDuration, 3);  strcat(menuList[7], numberString );  
              } 
              
              if (selected == 8) { strcpy(charMenu,"main");  }  
         } // while ( selected != numMenuItems)    
        
        loadSaveSettings(SAVE_DATA); 
  }   




/*
  _____   _____             _____  _   _ 
 |  __ \ |  __ \     /\    |_   _|| \ | |
 | |  | || |__) |   /  \     | |  |  \| |
 | |  | ||  _  /   / /\ \    | |  | . ` |
 | |__| || | \ \  / ____ \  _| |_ | |\  |
 |_____/ |_|  \_\/_/    \_\|_____||_| \_|

Very simply operation. 
Select the valve then press the OK button to start the drain. Press the OK button again to stop.
Displays the valve open time while draining.
Not really needed now that I have added hardware switches for draining.
*/
  if ( strcmp(charMenu, "drain" )==0 )
  {

       // copy the menu from program memory to the menu array
       numMenuItems = 4; 
       for (int i=0; i<numMenuItems+1; i++)
       {   strcpy_P(menuList[i], (char*)pgm_read_word(&menu_drain[i])); // Necessary casts and dereferencing. 
       }

       // set which menu items are selectable
       for ( byte i=1; i<=numMenuItems; i++)
       {
            menuItemSelectable[i] = true; 
       }
        
        
       currentItem = 1;
       selected = 0; 
       
       drawScreen = true;  
       boolean done = false;
       while (!done)
       {       
            selected = menuSelection();
            drawScreen = false; 
            if (selected >0 && selected <4) 
            { 
                 changePointer2gt();
                 lcd.setCursor(14, arrowPos);   lcd.print(F("00000"));
                 unsigned long timePast = 0;
                 unsigned long startTime = millis(); 
                 openSolenoid(selected);
                 //if (checkButtonPress( false)==BUTTON_SELECT ) { aborted = true; done2 = true;}  
                 boolean done2 = false;
                 while (!done2)
                 {
                      timePast =   millis() - startTime;              
                      numberToString( timePast,5 );   lcd.setCursor(14, arrowPos);   lcd.print(numberString);
                      if (checkButtonPress( true)==BUTTON_SELECT ) { done2 = true;} 
                 }
                 closeSolenoid(selected); 
                 delay (100);
                 changePointer2arrow();   lcd.setCursor(14, arrowPos);   lcd.print(F("     "));
            } 
            if (selected == 4) { strcpy(charMenu,"main"); done = true; }  
    
       } // while (!done)
       
  } // if ( strcmp(charMenu, "drain" )==0 )
  
  





/*
  _       ____            _____         __    _____      __      __ ______ 
 | |     / __ \    /\    |  __ \       / /   / ____|   /\\ \    / /|  ____|
 | |    | |  | |  /  \   | |  | |     / /   | (___    /  \\ \  / / | |__   
 | |    | |  | | / /\ \  | |  | |    / /     \___ \  / /\ \\ \/ /  |  __|  
 | |____| |__| |/ ____ \ | |__| |   / /      ____) |/ ____ \\  /   | |____ 
 |______|\____//_/    \_\|_____/   /_/      |_____//_/    \_\\/    |______|

ATmega328 EEPROM memory = 1024 bytes.
This safely gives me space for 6 saved data sets. 3 for Quick Mode and 3 for Full Mode

// LOAD/SAVE MENU
*/

if ( strcmp(charMenu, "L/S" )==0 )
   {

        // copy the menu from program memory to the menu array
        numMenuItems = 5; 
        for (int i=0; i<numMenuItems+1; i++)
        {  strcpy_P(menuList[i], (char*)pgm_read_word(&menu_loadSave[i]));    }        

        selected = 0;
        currentItem = 1;
        if (old_Selected[1] >0) {  currentItem = old_Selected[1];  }  
        
        drawScreen = true;
        boolean done = false;
        while (!done)
        {
             selected = menuSelection();
             old_Selected[1] = selected;
             if (selected ==1) { strcpy(charMenu,"load");  done=true;   old_Selected[1] = 1; }
             if (selected ==2) { strcpy(charMenu,"save");  done=true;   old_Selected[1] = 2; }
             if (selected ==3) { strcpy(charMenu,"dflt");  done=true;   old_Selected[1] = 3; }
             if (selected ==4) { strcpy(charMenu,"eras");  done=true;   old_Selected[1] = 4; }
             if (selected ==5) { strcpy(charMenu,"main");  done=true;   old_Selected[1] = 0; }
        }
   } // if ( strcmp(charMenu, "L/S" )==0 )


      
      
/*
   _____        __      __  ______ 
  / ____|    /\ \ \    / / |  ____|
 | (___     /  \ \ \  / /  | |__   
  \___ \   / /\ \ \ \/ /   |  __|  
  ____) | / ____ \ \  /    | |____ 
 |_____/ /_/    \_\ \/     |______|

Save the drop data to eeprom.
There are 3 memory locations for each mode. Allows to save 3 different sets of data
*/


 if ( strcmp(charMenu, "save" )==0 )
    {

        // copy the menu from program memory to the menu array
        strcpy(menuList[0],"SAVE"); 
        createLoadSaveMenuStrings();     
        
        int dataSetNum = 0;
        
        numMenuItems = 4; 
        // set which menu items are selectable
        for ( byte i=1; i<=numMenuItems; i++)
        {
             menuItemSelectable[i] = true; 
        }

        selected = 0;
        currentItem = 1;
        drawScreen = true;
        boolean done = false;
        while (!done)
        {
             selected = menuSelection();
             if (selected ==1) { dataSetNum = 1; }
             if (selected ==2) { dataSetNum = 2; }
             if (selected ==3) { dataSetNum = 3; }
             
              
             if (selected >0 && selected <4)
             {
                  boolean savedOK = false;
                  strcpy(menuList[0], "SAVE TO BANK: "); numberToString (dataSetNum, 1);  strcat(menuList[0], numberString );
                  strcpy(menuList[1], "CONFIRM SAVE");
                  strcpy(menuList[2], "EXIT WITHOUT SAVING");
                  numMenuItems = 2; 

                  menuItemSelectable[1] = true;
                  menuItemSelectable[2] = true; 
                                      
                  selected = 0; 
                  currentItem = 1; 
                  drawScreen = true;
                   
                  boolean finshedSave = false;
                  while (!finshedSave)
                  {
                       selected = menuSelection();
                       if (selected==1) { loadSaveDropData( SAVE_DATA, mode, dataSetNum, SHOW_SCREEN); finshedSave = true;  }
                       if (selected==2) { finshedSave = true;  }
                  }
                  
                  selected=4;   // jump back to the load/save menu - need to redo the menuList[] values
             }

            if (selected ==4) { strcpy(charMenu,"L/S");  done=true;   }
        } // while (!done

    } // if ( strcmp(charMenu, "save" )==0 )
 
 
 
 
/*
  _         ____               _____  
 | |       / __ \      /\     |  __ \ 
 | |      | |  | |    /  \    | |  | |
 | |      | |  | |   / /\ \   | |  | |
 | |____  | |__| |  / ____ \  | |__| |
 |______|  \____/  /_/    \_\ |_____/

Loads drop data from the eeprom memory
*/


 if ( strcmp(charMenu, "load" )==0 )
    {


        // copy the menu from program memory to the menu array
        strcpy(menuList[0],"LOAD: PICK LOCATION"); 
        createLoadSaveMenuStrings();    
        numMenuItems = 4;
        
        
        int dataSetNum = 0;

       // set which menu items are selectable
       for ( byte i=1; i<=numMenuItems; i++)
       {
            menuItemSelectable[i] = true; 
       }

        currentItem = 1;
        selected = 0; 

        drawScreen = true;
        boolean done = false;
        while (!done)
        {
             selected = menuSelection();
           
             // check to see the selected memory bank has data.
             // may change this so that only memory banks with data are displayed.
             // Don't really need 3 memory banks. One is probably enough.
           
             // The below 3 statements can be done in a single statement. Have left to make things easier to read.
             if (selected ==1) { 
                                   if (eepromReadInt( eep_NUM_TIMES_WRITTEN_ADDRESS[mode][selected] ) != 0 ) { dataSetNum = 1;}  else { selected = 0; drawScreen = false; }
                               }
             if (selected ==2) { 
                                   if (eepromReadInt( eep_NUM_TIMES_WRITTEN_ADDRESS[mode][selected] ) != 0 ) { dataSetNum = 2; }  else { selected = 0; drawScreen = false;}
                               }
             if (selected ==3) { 
                                   if (eepromReadInt( eep_NUM_TIMES_WRITTEN_ADDRESS[mode][selected] ) != 0 ) { dataSetNum = 3; }  else { selected = 0; drawScreen = false;}
                               }
           

             if (selected >0 && selected <4)
                {
                     boolean loadedOK = false;
                     strcpy(menuList[0], "LOAD SET: "); numberToString (dataSetNum, 1);  strcat(menuList[0], numberString );
                     strcpy(menuList[1], "CONFIRM LOAD");
                     strcpy(menuList[2], "EXIT WITHOUT LOAD");
                     numMenuItems = 2;
                     menuItemSelectable[1] = true;
                     menuItemSelectable[2] = true; 
                   
                     selected = 0;  
                     currentItem = 1; 
                     drawScreen = true;
                   
                     boolean finshedLoad = false;
                     while (!finshedLoad)
                     {
                          selected = menuSelection();
                          if (selected==1) { loadedOK = loadSaveDropData( LOAD_DATA, mode, dataSetNum, SHOW_SCREEN); finshedLoad = true;  }
                          if (selected==2) { finshedLoad = true;  }
                     }
                  
                     selected=4;   // need to redo the menuList[] values. Easiest way is to jump back to the load/save menu.
                }
              
           
             if (selected ==4) { strcpy(charMenu,"L/S");  done=true;  }
           
        } // while (!done)

    }





/*
   _____  ______  _______     _____   ______  ______       _    _  _    _______ 
  / ____||  ____||__   __|   |  __ \ |  ____||  ____|/\   | |  | || |  |__   __|
 | (___  | |__      | |      | |  | || |__   | |__  /  \  | |  | || |     | |   
  \___ \ |  __|     | |      | |  | ||  __|  |  __|/ /\ \ | |  | || |     | |   
  ____) || |____    | |      | |__| || |____ | |  / ____ \| |__| || |____ | |   
 |_____/ |______|   |_|      |_____/ |______||_| /_/    \_\\____/ |______||_|  

3 separate sets of drop data can be stored (3 for QM, 3 for FM)
Sets the default saved drop data. The values stored in the default memory set will be loaded automatically on startup

*/
 if ( strcmp(charMenu, "dflt" )==0 )
    {

        // create the menu 
        strcpy(menuList[0],"SET DEFAULT"); 
        createLoadSaveMenuStrings(); 

       // if new default != old default - write the value to eeprom.
       // rerun the function to show the new default

        int tmp1 = eepromReadInt( eep_NUM_TIMES_WRITTEN_ADDRESS[mode][1] );
        int tmp2 = eepromReadInt( eep_NUM_TIMES_WRITTEN_ADDRESS[mode][2] );
        int tmp3 = eepromReadInt( eep_NUM_TIMES_WRITTEN_ADDRESS[mode][3] );
        int tmp4 = eepromReadByte( eep_DEFAULT_SAVED_DATA_ADDRESS[mode] );

        int dataSetNum = 0;


       // set which menu items are selectable
       for ( byte i=1; i<=numMenuItems; i++)
       {
            menuItemSelectable[i] = true; 
       }


        currentItem = 1;
        selected = 0; 
        
        drawScreen = true;
        boolean done = false;

        while (!done)
        {
             selected = menuSelection();
             drawScreen = false;
             if  (selected != tmp4)   // if not already set
             {
                  // check that the memory bank has data. Stops an empty memory bank being selected.
                  if (  (selected ==1  && tmp1 > 0) ||  (selected ==2  && tmp2 > 0)  || (selected ==3  && tmp3 > 0)   )
                  {
                       // show confirmation message
                       lcd.clear();
                       lcd.setCursor(0,0);  lcd.print(F("SET DEFAULT"));
                       eepromWriteByte(selected, eep_DEFAULT_SAVED_DATA_ADDRESS[mode]);
                    
                       lcd.setCursor(1, 1);   lcd.print(F("Default now = set ")); lcd.print( selected); 
                       lcd.setCursor(1, 3);   lcd.print(F("EXIT"));                    
                       lcd.setCursor(0, 3);   lcd.write(rightArrow);
            
                       selected = 4;
                  }            
             }
             if (selected ==4) { strcpy(charMenu,"L/S");  done=true;   }
        } // while (!done)

    }

 
      
/*
  ______  _____              _____  ______      ______  ______  _____   _____    ____   __  __ 
 |  ____||  __ \     /\     / ____||  ____|    |  ____||  ____||  __ \ |  __ \  / __ \ |  \/  |
 | |__   | |__) |   /  \   | (___  | |__       | |__   | |__   | |__) || |__) || |  | || \  / |
 |  __|  |  _  /   / /\ \   \___ \ |  __|      |  __|  |  __|  |  ___/ |  _  / | |  | || |\/| |
 | |____ | | \ \  / ____ \  ____) || |____     | |____ | |____ | |     | | \ \ | |__| || |  | |
 |______||_|  \_\/_/    \_\|_____/ |______|    |______||______||_|     |_|  \_\ \____/ |_|  |_|
                                                                                               
// ERASE MEMORY. Resets all the settings and user drop data.
// Fills eeprom with zeros
*/
 if ( strcmp(charMenu, "eras" )==0 )
    {

         numMenuItems = 3;
         // copy the menu from program memory to the menu array 
         for (int i=0; i<numMenuItems+1; i++)
         {  strcpy_P(menuList[i], (char*)pgm_read_word(&menu_erase[i]));    }        

         // set which menu items are selectable
         menuItemSelectable[1] = false;
         menuItemSelectable[2] = true;
         menuItemSelectable[3] = true;



         currentItem = 1;
         selected = 0;
                  
         drawScreen = true;
         boolean done = false;
         while (!done)
         {
               selected = menuSelection();
               if (selected ==1) { drawScreen = false; } // do nothing
               if (selected ==3) {                strcpy(charMenu,"L/S");  done=true;   }
               if (selected ==2) { eraseEeprom(); strcpy(charMenu,"L/S");  done=true;   }            
         }
    }


    
    



/*
            ____    ____   _    _  _______ 
     /\    |  _ \  / __ \ | |  | ||__   __|
    /  \   | |_) || |  | || |  | |   | |   
   / /\ \  |  _ < | |  | || |  | |   | |   
  / ____ \ | |_) || |__| || |__| |   | |   
 /_/    \_\|____/  \____/  \____/    |_|

*/

if ( strcmp(charMenu, "about" )==0 )
   {

        numMenuItems = 7;
        // copy the menu from program memory to the menu array 
        for (int i=0; i<numMenuItems+1; i++)
        {  strcpy_P(menuList[i], (char*)pgm_read_word(&about[i]) );    
        }        
         
        strcat(menuList[4], ver );        // version
        strcat(menuList[5], __DATE__ );   // Date
        numberToString( getFreeMemory(), 4); strcat(menuList[6], numberString );  // freeMem
        //strcpy_P(menuList[6], (char*)pgm_read_word(&common[2]) );  // EXIT  


        // set which menu items are selectable
        for ( byte i=1; i<=numMenuItems; i++)
        {
             menuItemSelectable[i] = false; 
        }
        menuItemSelectable[numMenuItems] = true; 

        currentItem = 1;
        selected = 0;
         
        drawScreen = true;
        boolean done = false;
        while (!done)
        {
             selected = menuSelection();
             drawScreen = false;
             if (selected ==numMenuItems) { done=true;   }            
        }
         
        strcpy(charMenu,"main"); 

       
   } // if ( strcmp(charMenu, "about" )==0 )






/*
  _____  _____   ____  _____    _____ _   _ ______ ____  
 |  __ \|  __ \ / __ \|  __ \  |_   _| \ | |  ____/ __ \ 
 | |  | | |__) | |  | | |__) |   | | |  \| | |__ | |  | |
 | |  | |  _  /| |  | |  ___/    | | | . ` |  __|| |  | |
 | |__| | | \ \| |__| | |       _| |_| |\  | |   | |__| |
 |_____/|_|  \_\\____/|_|      |_____|_| \_|_|    \____/

Displays the drop times relative to the start time
Only useful for Full Mode. I don't use this when in Quick Mode

Uses the menu function even though only EXIT is selectable.

*/

if ( strcmp(charMenu, "DI" )==0 )
   {

        long dropStopTime[10]      = {0,    0,   0,   0,   0,   0,   0,   0,   0,   0}; 
        byte i=0;

        strcpy(menuList[0]," D   S STRT STOP SZE");
        byte loopSize=3; 
        if(mode==FULL_MODE) {loopSize = 9;}

        for (i=1; i <= loopSize; i++)
        {
             strcpy(menuList[i], "");
             if (mode==QUICK_MODE) {strcpy(menuList[i], "Q");} else  {strcpy(menuList[i], "F");}
               
             numberToString (i, 1);                              strcat(menuList[i], numberString);    strcat(menuList[i],"  "); 
             numberToString (sol[i], 1);                         strcat(menuList[i], numberString);    strcat(menuList[i]," ");
             numberToString (dropStartTime[i], 4);               strcat(menuList[i], numberString);    strcat(menuList[i]," ");
             dropStopTime[i] = dropStartTime[i] + dropSize[i];
             numberToString (dropStopTime[i], 4);                strcat(menuList[i], numberString);    strcat(menuList[i]," "); 
             numberToString (dropSize[i], 3);                    strcat(menuList[i], numberString);            
        }

        strcpy(menuList[i  ],"CAM   "); numberToString (camTrigStart[mode], 4);   strcat(menuList[i], numberString);     strcat(menuList[i],"      ");   numberToString (camTriggerDuration, 3);   strcat(menuList[i], numberString);                 
        strcpy(menuList[i+1],"FLSH  "); numberToString (flashTrigStart[mode], 4); strcat(menuList[i+1], numberString);   strcat(menuList[i+1],"      "); numberToString (flashTriggerDuration, 3); strcat(menuList[i+1], numberString);    

        strcpy(menuList[i+2],"EXIT");
  
        numMenuItems = i+2;

       // set which menu items are selectable
       for ( byte i=1; i<=numMenuItems; i++)
       {
            menuItemSelectable[i] = false; 
       }
       menuItemSelectable[numMenuItems] = true; 


        currentItem = 1;
        selected = 0;
        
        drawScreen = true;
        while (selected != i+2)
        {
             selected = menuSelection();
             drawScreen = false;
        }
        strcpy(charMenu,"main");
        
   }







} // void loop
// -==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==--==-


// END

