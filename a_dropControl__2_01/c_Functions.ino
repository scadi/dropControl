/*
  ______  _    _  _   _   _____  _______  _____  ____   _   _   _____ 
 |  ____|| |  | || \ | | / ____||__   __||_   _|/ __ \ | \ | | / ____|
 | |__   | |  | ||  \| || |        | |     | | | |  | ||  \| || (___  
 |  __|  | |  | || . ` || |        | |     | | | |  | || . ` | \___ \ 
 | |     | |__| || |\  || |____    | |    _| |_| |__| || |\  | ____) |
 |_|      \____/ |_| \_| \_____|   |_|   |_____|\____/ |_| \_||_____/ 
                                                                      

                                                                      
/*




/*                 
****************************************
* Function numberToString
* converts a long number to a string
* puts the string into the global var  numberString
* 
* passed: number - the number to convert
*         digits - the number of digits to use. 1 to 5 digits
*         
* global:  numberString
* 
* Returns:
*          
*
*/
void numberToString(long number, int digits)
{ 
    char tempString1[10] = "\0";
    char tempString2[10] = "\0";
    int numZeros = 0;
    boolean error = false;
    if (digits ==1 && number > 9 )       { strcpy(numberString, "E");     error = true; }
    if (digits ==2 && number > 99 )      { strcpy(numberString, "E2");    error = true; }
    if (digits ==3 && number > 999 )     { strcpy(numberString, "ER3");   error = true; }
    if (digits ==4 && number > 9999 )    { strcpy(numberString, "ERR4");  error = true; }
    if (digits ==5 && number > 99999 )   { strcpy(numberString, "ERRR5"); error = true; }
    if  (digits >5 )   { strcpy(numberString, "ERROR"); error = true; }

    if ( !error ) 
    {
       strcpy(numberString, tempString1);
       itoa (number, tempString1,10);
       numZeros = digits - strlen( tempString1) - 1;
       for (int i=0; i <= numZeros; i++)    { strcat(tempString2,"0");  } 
       strcat(tempString2,tempString1); 
       strcat(numberString,tempString2); 
    }  
    else {  // error
            // maybe add a beep for errors 
            ; 
          }     

} // void numberToString (long number, int digits)



/*
****************************************
* Function changePointer2gt
* changes to cursor to the gt symbol ">"
* 
* passed: 
*         
* global:  
* 
* Returns:
*          
*/
void changePointer2gt()
{
    lcd.setCursor(0, arrowPos); lcd.print(">");
}




/*
****************************************
* Function changePointer2arrow
* changes to cursor to the right arrow character
* 
* passed: 
*         
* global:  
* 
* Returns:
*          
*/
void changePointer2arrow()
{
    lcd.setCursor(0, arrowPos); lcd.write(rightArrow);
}






/*
****************************************
* Function triggerStart
* starts the trigger; camera focus, camera shutter, flash
* 
* passed: trig.   1=SHUTTER.  2=FOCUS.  3=FLASH.
*         
* global:  
* 
* Returns:
* 
* I can remove this function and write the pin in the main routine.        
*/
void triggerStart(int trig)
{
   if (trig ==1) {  digitalWrite(camTrigSHPin, HIGH);   }
   if (trig ==2) {  digitalWrite(camTrigFCPin, HIGH);   }
   if (trig ==3) {  digitalWrite(flashTrigPin, HIGH);   }
}



/*
****************************************
* Function triggerStop
* stops the trigger; camera focus, camera shutter, flash
* 
* passed: trig.   1=SHUTTER.  2=FOCUS.  3=FLASH.
*         
* global:  
* 
* Returns:
* 
* I can remove this function and write the pin in the main routine.        
*/
void triggerStop(int trig)
{
   if (trig ==1) {  digitalWrite(camTrigSHPin, LOW);   }
   if (trig ==2) {  digitalWrite(camTrigFCPin, LOW);   }
   if (trig ==3) {  digitalWrite(flashTrigPin, LOW);   }
}



/*
****************************************
* Function openSolenoid
* opens a solenoid valve
* 
* passed: s. The value to open
*         
* global:  
* 
* Returns:
* 
*/
void openSolenoid(byte s)
{
    if (s==1) { digitalWrite(sol1TriggerPin, HIGH);    }
    //if (s==2) { digitalWrite(sol2TriggerPin, HIGH);    }
    //if (s==3) { digitalWrite(sol3TriggerPin, HIGH);    }
}


/*
****************************************
* Function closeSolenoid
* closes a solenoid valve
* 
* passed: s. The value to close
*         
* global:  
* 
* Returns:
* 
*/
void closeSolenoid(byte s)
{
    if (s==1) { digitalWrite(sol1TriggerPin, LOW);    }
//    if (s==2) { digitalWrite(sol2TriggerPin, LOW);    }
//    if (s==3) { digitalWrite(sol3TriggerPin, LOW);    }
}




                                                                      
/*
****************************************
* Function editNumber
* 
* 
* passed: number - the start value of the number
*         numDigits - number of digits
*         x  - the x position on screen
*         y  - the y position on screen
*         minVal
*         maxVal
*         
* global:  
* 
* Returns:  the new value for number
* 
*/
long editNumber(  long number, byte numDigits, byte x, byte y,  long minVal, long maxVal)
{
    byte keyPress = 0;
    boolean updatePointer = false;
    int pointer = x+numDigits-1;
    int offSet = x+numDigits;
    
    lcd.setCursor(pointer,y); 
    lcd.cursor();
    
    drawScreen = false;
    boolean doneEN=false;
    
    while (!doneEN )
    {
        
        keyPress = checkButtonPress(true);
        
        if ( keyPress == BUTTON_SELECT )  { doneEN = true;} 

        if ( keyPress == BUTTON_LEFT )  
        {   
             pointer--;        if (pointer < x) { pointer = x;}  
             updatePointer = true;
        }
        
        if ( keyPress == BUTTON_RIGHT )  
        {  
              pointer++;       if (pointer > x+numDigits-1  ) { pointer = x+numDigits-1;}  
              updatePointer = true;
        }
        

    
        if ( keyPress == BUTTON_UP )  
        {             
             // units, tens, hundreds, etc
             if (offSet-pointer == 1) {  number++; }                 
             if (offSet-pointer == 2) {  number = number + 10;  }    
             if (offSet-pointer == 3) {  number = number + 100;  }    
             if (offSet-pointer == 4) {  number = number + 1000;  }   
             if (offSet-pointer == 5) {  number = number + 10000; }  
             
             if (number > maxVal) 
             {  
                  if (maxVal < 10) {  number = minVal;    }   else   { number = number - maxVal -1;  }
             }             
             drawScreen = true;
        }  
        
        if ( keyPress == BUTTON_DOWN )  
        {  
             if (offSet-pointer == 1) {  number--; }                  
             if (offSet-pointer == 2) {  number = number - 10;  }    
             if (offSet-pointer == 3) {  number = number - 100;  }    
             if (offSet-pointer == 4) {  number = number - 1000;  }   
             if (offSet-pointer == 5) {  number = number - 10000; }   

             if (number < minVal) 
             {  
                   if (maxVal < 10) {  number = maxVal;    } 
                   else             {  number = maxVal + 1 - abs(number); }
             }
             drawScreen = true;
        }  

        // update the number on screen
        if (drawScreen) 
        {  
             numberToString( number, numDigits );  // sets the String variable timeDisplayFormat
             lcd.setCursor(x, y);  lcd.print(numberString);
             lcd.setCursor(pointer,y);
             //lcd.cursor(); 
             drawScreen = false;
        }
         
        // Cursor
        if (updatePointer) 
        { 
             lcd.setCursor( pointer, y); 
             updatePointer = false;
        }

    } // while (!doneEN )

    lcd.noCursor();
    return number; 
        
} // long editNumber( long number)











/*
****************************************
* Function editDropTimings
* calls functions to edit the drop times
* 
* passed: numberDrops
*         
* global:  
* 
* Returns:  the new value for number
* 
*/
void editDropTimings(byte numberDrops)
{

    char tmpString[2] = "\0";  

    strcpy(menuList[0],"EDIT S STRT SZE STOP");
    strcpy(menuList[numberDrops+1],"EXIT");
    numMenuItems = numberDrops+1;
    
    boolean done1 = false;
    while (!done1)
    {
         createDropMenuStrings(1, 1, numberDrops);
         boolean done2 = false;
         
         
         currentItem = 1;
         selected = 0;
         drawScreen = true;
         while (!done2)
         {
              selected = menuSelection();
              //                                  editDrop(int drop)
              if (selected <= numberDrops)      { editDrop(selected);   createDropMenuStrings(1, 1, numberDrops);  drawScreen = false;     }
              if (selected == (numberDrops+1) ) { strcpy(charMenu,"FM"); done2=true;  done1=true;      }
         } // done2
    } // done1     
} // void editDropTimings()







/*
****************************************
* Function editDrop
* calls functions to edit the time and size for one drop
* 
* passed: drop
*         
* global: sol[]
*         dropStartTime[]
*         dropSize[] 
* 
* Returns:  
* 
*/
void editDrop(byte drop)
{
    
    long dropStopTime = 0; 
    changePointer2gt();
    if (mode==QUICK_MODE)  { drop = drop+9; }
    
    sol[drop] = editNumber(sol[drop], 1, 5, arrowPos, 1, numSolenoids );
    
    dropStartTime[drop]   = editNumber(dropStartTime[drop], 4, 7, arrowPos, 0, 9999 );
    dropSize[drop] = editNumber(dropSize[drop], 3, 12, arrowPos, 0, 999 );
    
    dropStopTime = dropStartTime[drop] + dropSize[drop];
    numberToString (dropStopTime, 4); 
    lcd.setCursor(16,arrowPos);   lcd.print(numberString); 
    
    changePointer2arrow();
   
}







/*
****************************************
* Function editIncTimes
* 
* 
* passed: number of drops
*         
* global: 
* 
* Returns:  
* 
*/
void editIncTimes(byte numberDrops)
{

    strcpy(menuList[0],"EDIT   STRT SZE");
    for (int i=1; i <= numberDrops; i++)
    {
           createIncMenuString(i,i,1);
    }
     
    strcpy(menuList[numberDrops+1],"EXIT");
    numMenuItems = numberDrops+1;  

    boolean done = false;
    while (!done)
    {

         selected = 0;
         currentItem = 1;         
         drawScreen = true;
         selected = menuSelection();
         if (selected >=1  && selected <= numberDrops) 
         {    
              changePointer2gt();
              editInc(selected,arrowPos);  createIncMenuString(selected,selected,1) ; drawScreen = false ; 
              changePointer2arrow();
         }
         if (selected == (numberDrops+1) ) { strcpy(charMenu,"FM");  done=true;      }
         
    } // while !done

}







/*
****************************************
* Function editInc
* edit the increment / decrement values
* 
* passed: drop - drop to edit
*         y - y position on screen
*         
* global: incDecFlag[]
*         incDropStartTime[]
*         incSize[]
* 
* Returns:  
* 
*/
void editInc(byte drop, byte y)
{
     if (mode=QUICK_MODE) { drop = drop+9;   }
    
    // change arrow to show edit mode is active
    changePointer2gt();

     // edit incDecFlag[drop]
     incDecFlag[drop] = editFlag( incDecFlag[drop], INC_DEC, 2,arrowPos);

     //                          long number, int numDigits, int x, int y - x is the start pos of the number
    incDropStartTime[drop] = editNumber(incDropStartTime[drop], 4, 7, arrowPos, 0, 9999 );    
    incSize[drop] = editNumber(incSize[drop], 3, 12, arrowPos, 0, 999 );

    changePointer2arrow();      
}









/*
****************************************
* Function editFlag
* edits one of the system flags
* if type == 1 then YES/NO
* if type == 2 then ON/OFF
* if type == 3 then BEFORE/AFTER - 0-BEFORE. 1=AFTER
* if type == 4 then INC / DEC
* if type == 5 then QM_FM
*
* passed: val
*         type - which flag is it
*         x - the x position on screen
*         y - the y position on screen
*         
* global: 
* 
* Returns: the new value 
* 
*/
int editFlag(byte val, byte type, byte x, byte y)
{
    
     byte keyPress = 0;
     lcd.setCursor(x,y); 
     lcd.cursor();
    
     int done=false;
     while (!done)
     {
         keyPress = checkButtonPress(true);
         
         if ( keyPress == BUTTON_SELECT )  { done = true;} 
         
         if (( keyPress == BUTTON_DOWN ) || ( keyPress == BUTTON_UP ))
         {
             val=1-val; // flip the value
             lcd.setCursor(x, y);
             
            // type 1=YES/NO
            if (type==1) {    if (val==0)   {  lcd.write(" NO");  }     else  { lcd.write("YES");     }      }
            if (type==2) {    if (val==0)   {  lcd.write("OFF");  }     else  { lcd.write(" ON");     }      }
            if (type==3) {    if (val==0)   {  lcd.write("BEFORE");  }  else  { lcd.write(" AFTER");  }      }
            if (type==4) {    if (val==0)   {  lcd.write("INC");  }     else  { lcd.write("DEC");     }      }
            if (type==5) {    if (val==0)   {  lcd.write(" FULL");  }   else  { lcd.write("QUICK");   }      }
         }
         lcd.setCursor(x,y);

     }    
     lcd.noCursor();
     return val;
}



  






/*
****************************************
* Function editTriggers
* edit the trigger start times and the trigger increment times
* passed: 
*         
* global: mode
*         camTrigStart[]
*         camTrigInc[]
*         flashTrigStart[]
*         flashTrigInc[]
* 
* Returns: 
* 
*/
void editTriggers()
  {


     numMenuItems = 5;
     for (int i=0; i<numMenuItems+1; i++)
     {   strcpy_P(menuList[i], (char*)pgm_read_word(&menu_TD[i])); // Necessary casts and dereferencing. 
     }
     numberToString (camTrigStart[mode], 4);        strcat(menuList[1], numberString );  
     numberToString (camTrigInc[mode], 3);          strcat(menuList[2], numberString );
     numberToString (flashTrigStart[mode], 4);      strcat(menuList[3], numberString ); 
     numberToString (flashTrigInc[mode], 3);        strcat(menuList[4], numberString );


     selected = 0;
     currentItem = 1;
          
     drawScreen = true;
     while ( selected != numMenuItems)
     {       
         selected = menuSelection();
         drawScreen = false; 
         if (selected == 1) { changePointer2gt(); camTrigStart[mode]   = editNumber(camTrigStart[mode], 4, 16, arrowPos,0,9999);    changePointer2arrow();  } 
         if (selected == 2) { changePointer2gt(); camTrigInc[mode]     = editNumber(camTrigInc[mode], 3, 17, arrowPos,0,999);       changePointer2arrow();  } 
         if (selected == 3) { changePointer2gt(); flashTrigStart[mode] = editNumber(flashTrigStart[mode], 4, 16, arrowPos,0,9999);  changePointer2arrow();  } 
         if (selected == 4) { changePointer2gt(); flashTrigInc[mode]   = editNumber(flashTrigInc[mode], 3, 17, arrowPos,0,999);     changePointer2arrow();  } 
         if (selected == 5) {   }  
     } // while ( selected != numMenuItems)    

     delay(100);
  }  
  

  
/*
****************************************
* Function editFlashStartAndFlashInc
* 
* passed:  menuRow
*         
* global: mode
*         flashTrigStart[]
*         flashTrigInc[
* 
* Returns: 
* 
*/
void editFlashStartAndFlashInc(int menuRow)
{ 
    changePointer2gt();
    flashTrigStart[mode] = editNumber(flashTrigStart[mode], 4, 7, arrowPos,0,9999); 
    flashTrigInc[mode]   = editNumber(flashTrigInc[mode], 3, 17, arrowPos,0,999);
    changePointer2arrow(); 
    createFlashTriggerMenuLine(menuRow); 
} 



/*
****************************************
* Function editCamStartAndCamInc
* 
* passed: mode
*         camTrigStart[]
*         camTrigInc[]
*         
* global: 
* 
* Returns: 
* 
*/
void editCamStartAndCamInc(int menuRow)
{   
    // Serial.print("MR="); Serial.println(menuRow);
    changePointer2gt();
    camTrigStart[mode] = editNumber(camTrigStart[mode], 4, 7, arrowPos,0,9999); 
    camTrigInc[mode]   = editNumber(camTrigInc[mode], 3, 17, arrowPos,0,999);
    changePointer2arrow(); 
    createCamTriggerMenuLine(menuRow); 
} 




/*
****************************************
* Function  checkOverlap
* checks to see if the drop times overlap each other
* does not check to see if the drops are in chronological order
* 
* passed: 
*         
* global: 
* 
* Returns:  true if overlap. false for no overlap
* 
*/
boolean checkOverlap()
{
     boolean error = false;
     long stopTimeJ = 0;
     long stopTimeI = 0; 
     if (numDrops[mode]>1)
    {
          for ( int j=1; j<=numDrops[mode]-1; j++)
          {
              for ( int i=2; i<=numDrops[mode]; i++)
              {
                  //Serial.print(F("J = "));Serial.print(j);Serial.print(F("     I = ")); Serial.print(i);
                  
                  if (i!=j) 
                  {
                        if (sol[j] == sol[i] )
                        {    
                             stopTimeJ = dropStartTime[j] + dropSize[j];
                             stopTimeI = dropStartTime[i] + dropSize[i];
                             if ( ( dropStartTime[j] >= dropStartTime[i] ) && ( dropStartTime[j] <= stopTimeI ) )  {  error = true;  } 
                             if ( ( stopTimeJ >= dropStartTime[i]        ) && ( stopTimeJ <= stopTimeI )        )  {  error = true;  } 
  
                             //else  {Serial.print("\n"); }   
                        }
                        //else  {Serial.print("\n"); } 
                  }
                  //else {Serial.print(F(" - i = j\n")); }
               }
          }
    } 
return error;  
}




 
 
 


