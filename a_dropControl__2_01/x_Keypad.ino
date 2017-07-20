





/*
****************************************
* Function checkButtonPress
* Read the keypad. Either digital or analogue
* 
* passed: boolean waitForRealse - If true wait for key to be released
* global:
* 
* Returns: the key pressed or 0 if no key is pressed
*
* type of keypad is selected via the define statements at the start of the sketch
* only the one defined will be compiled
*/
byte checkButtonPress(boolean waitForRelease)
{
        byte key = 0;
        //#ifdef KEYPAD_DIGITAL
          //  key = getKeyPressDigital(true);
        //#endif 
        #ifdef KEYPAD_ANALOGUE
            key = getKeyPressAnalogue(true);
        #endif 
        
        return key; 
}







#ifdef KEYPAD_ANALOGUE
/*
****************************************
* Function getKeyPressAnalogue
* Read the keypad pin
* 
* passed: waitForRealse - If true wait for keys to be released
* 
* global: analogueKeypadPin
*         BUTTON_NONE   = 0;
*         BUTTON_SELECT = 5;
*         BUTTON_RIGHT  = 4;
*         BUTTON_DOWN   = 3;
*         BUTTON_UP     = 2;
*         BUTTON_LEFT   = 1;
*         
* Returns: the key pressed or 0 if no key is pressed
*
*
* Different keypads give different values. If all fit in the same range then will be OK. If not, I may need to calibrate them somehow.
* may use use key-20 / key+20.
* Get key values. Calculate min and max. Use min and max in the script. this should be slightly faster
*/

byte getKeyPressAnalogue( boolean waitForRelease )
{

   int adc_key_in = analogRead(0);
 if (adc_key_in > 1000) return BUTTON_NONE;
 if (adc_key_in < 50)   return BUTTON_RIGHT; 
 if (adc_key_in < 250)  return BUTTON_UP;
 if (adc_key_in < 450)  return BUTTON_DOWN;
 if (adc_key_in < 650)  return BUTTON_LEFT;
 if (adc_key_in < 850)  return BUTTON_SELECT; 
 
  return BUTTON_NONE;

  
/*    int key[3];
    int pinVal[3];

  pinVal[0] = analogRead(analogueKeypadPin);
  pinVal[1] = analogRead(analogueKeypadPin);
  pinVal[2] = analogRead(analogueKeypadPin);
*/
/*
      button values
      NONE =   910
      SELECT = 679
      RIGHT =  476
      DOWN =   324
      UP =     164
      LEFT =    38
*/
/*
  for(byte i=0; i<3; ++i)
  {
    key[i] = BUTTON_NONE;
    
    // select =679
    if ( (pinVal[i] >679-20) &&  (pinVal[i] <679+20)  )    {  key[i] = BUTTON_SELECT; }
 
    // Right = 476. 
    if ( (pinVal[i] > 476-20) &&  (pinVal[i] < 476+20)  )   {  key[i] = BUTTON_RIGHT;  }

    // Down = 324
    if ( (pinVal[i] > 324-20) &&  (pinVal[i] < 324+20)  )   {  key[i] = BUTTON_DOWN;  }
    
    // Up = 164
    if ( (pinVal[i] > 164-20) &&  (pinVal[i] < 164+20)  )   {  key[i] = BUTTON_UP;  }

    // Left = 38
    if ( (pinVal[i] > 38-20) &&  (pinVal[i] < 38+20)  )     {  key[i] = BUTTON_LEFT;  }

  } 
    delay(2);
    if ( (key[0] != key[1]) || (key[0] != key[2]) )   { return BUTTON_NONE;    }
    

    #ifdef DEBUG  
        if (( pinVal[0] < 878) && (pinVal[1] <878)  ) {Serial.print(pinVal[0]); Serial.print(" - ");  Serial.print(pinVal[1]); Serial.print(" - ");  Serial.println(pinVal[2]);    }    
    #endif 

    
    if ( (key[0] != BUTTON_NONE) && waitForRelease )
    {  
         // wait for the button to be released. Only need to check to see if the value < 882.  
         while ( (analogRead(analogueKeypadPin) < 878 )  )    {   delay(1);  }
         buttonDebounce(10);  // 10 ms debounce 
    }
    return key[0]; 
    */
}  



/*
****************************************
* Function buttonDebounce
* Test that no buttons are pressed for at least cont_time milliseconds 
* 
* passed: t. Time in milliseconds to wait
* global:
* 
* Returns: 
*
*/
/*void buttonDebounce(int t)
{
   int count = t;

   while ( count > 0 )
   {
       count --;
       if ((analogRead(analogueKeypadPin) < 750 ) )  { count = t; }   // if value != no key then reset the count
       delay(1);
   }
}
*/
#endif











