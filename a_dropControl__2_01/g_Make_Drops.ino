
/*
  __  __            _  __ ______  _____   _____    ____   _____    _____ 
 |  \/  |    /\    | |/ /|  ____||  __ \ |  __ \  / __ \ |  __ \  / ____|
 | \  / |   /  \   | ' / | |__   | |  | || |__) || |  | || |__) || (___  
 | |\/| |  / /\ \  |  <  |  __|  | |  | ||  _  / | |  | ||  ___/  \___ \ 
 | |  | | / ____ \ | . \ | |____ | |__| || | \ \ | |__| || |      ____) |
 |_|  |_|/_/    \_\|_|\_\|______||_____/ |_|  \_\ \____/ |_|     |_____/ 


uses global variables:





*/

/*
****************************************
* Function makeDrops
* controls the solenoid values to make the drops
* 
* passed:
*  
* global: 
*    camTrigDelay             shutter release delay. Shutter is activated at the start of the drop sequence
*    flashTrigDelay           flash trigger delay. The flash is triggered at the end of the drop sequence
*    camTrigInc               how much time to add to the camera tigger start time every loop
*    flashTrigInc             how much time to add to the flash tigger start time every loop
*    camTriggerTime           Time the flash trigger is active. Need to experient with different times.
*    flashTriggerTime         Time the flash trigger is active. Need to experient with different times.
*    numDrops 
*    dropStartTime[13]        drop start times in milli-seconds
*    dropSize[13]             dropSize in milli-seconds - up to 9 drops
*    dropStopTime[13]         calculated values (start time + size). Used an array to make menu update easier
*    byte sol[13]             the solenoid to use for the drop; 1,2 or 3
*    incDropStartTime[13]     how much time to add or subtract to the drop start time every loop
*    incSize[13]              how much time to add or subtract to the drop size every loop
*
* Returns:
*          
*
*/
void makeDrops()
{
    
    boolean done    = false;
    boolean aborted = false;
    boolean done2   = false;
    byte timerChar[4]  = { 45, 0, 124, 47 };    //   -\|/
    
    byte tmpNumDrops = numDrops[mode]; 
    
    // I use temp arrays in the makeDrop function. 
    // Means I can add INC or subtract DEC values without changing the original values intact
    long tmpDropStartTime[10]  = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    long tmpDropSize[10]       = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; 
    long tmpDropStopTime[10]   = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  
    byte tmpSol[10]            = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    
    long tmpCamTriggerStart   = camTrigStart[mode];
    long tmpCamTrigStop       = camTrigStart[mode] + camTriggerDuration;  
    long tmpFlashTriggerStart = flashTrigStart[mode] ;
    long tmpFlashTrigStop     = flashTrigStart[mode] + flashTriggerDuration;    
    
    long unsigned seqStartTime = 0;  
    int unsigned loopCount = 0;

    boolean CamTriggered   = false;
    boolean camDone = false;
    boolean flashTriggered = false;
    boolean flashDone = false;
    boolean allDropsDone = false;
    
    byte currentDrop = 0;
    boolean dropped[10] = { false, false, false, false, false, false, false, false, false, false} ;
    boolean solOpen[10] = { false, false, false, false, false, false, false, false, false, false} ;
    
    
    // if loopDelay[0] ==0 then use the remote switch
    if (loopDelay[0]==0) {usingRemoteSwitch = true;} else  {usingRemoteSwitch = false;}

    
    #ifdef DEBUG
        Serial.print(F("freeMem=")); Serial.println( freeMemory() );
    #endif    
    
    
    // do not need to care about the number of drops at this time. Simply copy the whole array. 3 or 9 elements
    // element [0] is not used. Wasteful but means drop 1 is element[1]
    if (mode==QUICK_MODE)
    {
        // copy the QM drop time data to the temp arrays. Quick mode drops are at position 10,11,12
        for (int i=1; i<4; i++)
        {
           tmpDropStartTime[i]= dropStartTime[i+9];
           tmpDropSize[i] = dropSize[i+9];
           tmpDropStopTime[i] = dropStartTime[i+9] + dropSize[i+9];
           tmpSol[i]= sol[i+9];
        }         
    }
    else if (mode==FULL_MODE)
    {
        // copy the FM drop time data to the temp arrays. Full mode drops are at position 1 through 9
        for (int i=1; i<10; i++)
        {
           tmpDropStartTime[i]= dropStartTime[i];
           tmpDropStopTime[i] = dropStartTime[i] + dropSize[i];
           tmpDropSize[i] = dropSize[i];
           tmpSol[i]= sol[i];
        }        
    }
    else
    {
        lcd.clear();
        lcd.setCursor(0, 0);  lcd.print(F("ERROR")); 
        lcd.setCursor(0, 1);  lcd.print(F("Not FM. Not QM"));
        lcd.setCursor(0, 2);  lcd.print(F("Need to reboot"));
        while(1) {;}
    }
    
    
    // SCREEN SETUP
    // Full mode and Quick mode use different screen layouts
    if (mode==FULL_MODE)
    {
        lcd.clear();
        lcd.setCursor(0, 0);  lcd.print(F("Full Mode"));  
        lcd.setCursor(1, 1);  lcd.print(F("Drops="));  lcd.print(numDrops[mode]); 
        lcd.setCursor(1, 2);  lcd.print(F("ND=0"));
        lcd.setCursor(11, 1); lcd.print(F("CTS=")); numberToString( tmpCamTriggerStart, 4 );   lcd.print(numberString);
        lcd.setCursor(11, 2); lcd.print(F("FTS=")); numberToString( tmpFlashTriggerStart, 4 ); lcd.print(numberString);
    }
    if (mode==QUICK_MODE)
    {
        lcd.clear();
        lcd.setCursor(1, 0);   lcd.print(F(" D=0"));    
        lcd.setCursor(1, 1);   lcd.print(F("CT="));     lcd.print(tmpCamTriggerStart);
        lcd.setCursor(1, 2);   lcd.print(F("FT="));     lcd.print(tmpFlashTriggerStart);

        lcd.setCursor(10, 0);                            lcd.print(F("1"));  
        if (numDrops[mode] >1)  { lcd.setCursor(10, 1);  lcd.print(F("2"));      }
        if (numDrops[mode] >2)  { lcd.setCursor(10, 2);  lcd.print(F("3"));      }    
    }    
    lcd.setCursor(0, 3);   lcd.write(126);  
    lcd.setCursor(1, 3);   lcd.print(F("STOP      "));

    

    //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    startPos:
    ;
    //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=

    if (checkButtonPress(false)==BUTTON_SELECT ) { aborted = true; } 
    if (aborted) {goto endPos; }
           
    loopCount ++;

    if (mode==QUICK_MODE)
    {
        
        lcd.setCursor(4, 0);   lcd.print(loopCount); 
        lcd.setCursor(4, 1);   lcd.print(tmpCamTriggerStart);
        lcd.setCursor(4, 2);   lcd.print(tmpFlashTriggerStart);

        lcd.setCursor(12, 0); numberToString( tmpDropStartTime[1], 4 );                       lcd.print( numberString );                              
        lcd.setCursor(17, 0); numberToString( tmpDropStopTime[1]-tmpDropStartTime[1], 3 );    lcd.print( numberString );

        if (numDrops[mode] >1)
        {
             lcd.setCursor(12, 1); numberToString( tmpDropStartTime[2], 4 );                      lcd.print( numberString );
             lcd.setCursor(17, 1); numberToString( tmpDropStopTime[2]-tmpDropStartTime[2], 3 );   lcd.print( numberString ); 
        }
        if (numDrops[mode] >2)
        { 
            lcd.setCursor(12, 2); numberToString( tmpDropStartTime[3], 4 );                      lcd.print( numberString );
            lcd.setCursor(17, 2); numberToString( tmpDropStopTime[3]-tmpDropStartTime[3], 3 );   lcd.print( numberString ); 
        }

    }
    
    else if (mode==FULL_MODE)
    {
        lcd.setCursor(4, 2);  lcd.print( loopCount);
        lcd.setCursor(11, 1); lcd.print(F("CTS=")); numberToString( tmpCamTriggerStart, 5 );   lcd.print(numberString);
        lcd.setCursor(11, 2); lcd.print(F("FTS=")); numberToString( tmpFlashTriggerStart, 5 ); lcd.print(numberString); 
    }

    else
    {
         // we have an error
        lcd.clear();
        lcd.setCursor(0, 0);  lcd.print(F("ERROR")); 
        lcd.setCursor(0, 1);  lcd.print(F("Not FM. Not QM"));
        lcd.setCursor(0, 2);  lcd.print(F("Need to reboot"));
        while(1) {;}  
    }


    if (checkButtonPress( false)==BUTTON_SELECT ) { aborted = true; } 
    if (aborted) {goto endPos; }

    
    
    // WAITING
    //
    // if loopDelay[0] ==0 then use the remote switch else use a delay
   
    
    // if loopDelay[0]==0 then use the remote switch
    if (usingRemoteSwitch)
    {
         // wait for the button to be pressed
         lcd.setCursor(11, 3);  lcd.print(F("PRESS REM"));
         done=false; 
         while (  !done  )
         { 
             if (rm_button.uniquePress())  { done=true; }
             if (checkButtonPress( false)==BUTTON_SELECT ) { aborted = true; done = true;} 
         }
        lcd.setCursor(11, 3);  lcd.print(F("         "));         
    }

    // if not using the remote switch we wait the time set by loopDelay 
    else
    {
          // wait for loopDelay
          lcd.setCursor(11, 3);  lcd.print(F("Waiting  ")); 
    
          unsigned long startTime = millis();
          unsigned long startWaiting = startTime;
          unsigned long nowTime = 0;
            
          int count = 0;
          done = false;
          while (!done )
          {
                nowTime = millis();
                if (nowTime - startTime >= loopDelay[0] )    { done = true;  }
                if (checkButtonPress( false)==BUTTON_SELECT ) { aborted = true; done=true;} 

                if (nowTime - startWaiting >250 )            
                { 
                     lcd.setCursor(19, 3);   lcd.write( timerChar[count] );
                     count++; if (count >3) {count = 0;} 
                     startWaiting = millis();
                }
            
                // pause
                if (rm_button.uniquePress() )                
                {    lcd.setCursor(11, 3);  lcd.print(F("   Paused"));         // The space erases the timerChar
                     delay (100);
                     done2 = false;
                     while (!done2)
                     {
                         if (rm_button.uniquePress()) { done2 = true; }
                         //if (ok_button.uniquePress()) { aborted = true; done2 = true;}
                         if (checkButtonPress(false)==BUTTON_SELECT ) { aborted = true; done2 = true;} 
                     }
                     delay (100);
                     lcd.setCursor(11, 3);  lcd.write( timerChar[count] ); lcd.print(F("Waiting")); 
                }            
          } // while (!done )
          lcd.setCursor(19, 3); lcd.write(32);         

    }   
        
        

  
    

    //  most of the time the abort happens in the wait loop.
    //  I can probably just jump to the end.
    
    
    if (checkButtonPress( false)==BUTTON_SELECT ) { aborted = true; } 
    if (aborted)  {  goto endPos ;  }



    //-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    // start the drop sequence

    lcd.setCursor(11, 3);  lcd.print(F("   Active"));


 
    // activate mirror lockup
    if (mirrorLockup && !aborted) 
    { 
         digitalWrite(camTrigFCPin, HIGH);  digitalWrite(camTrigSHPin, HIGH);  delay(camTriggerDuration);  digitalWrite(camTrigSHPin, LOW);  digitalWrite(camTrigFCPin, LOW);
         done = false;
         long unsigned startTime = millis();
         while (startTime == millis())  // This makes sure we are starting at the beginning of a millisecond
         {;}
         startTime = millis();
         
         while (!done)
         {

            if (millis() - startTime >= mirrorLockupDelay ) { done = true;  }
            //if (ok_button.uniquePress() )                   { aborted = true; done=true; }   
            if (checkButtonPress(false)==BUTTON_SELECT ) { aborted = true; done = true;}  
         }
        
         //  if abort while mirror is up - does not catch
    }
 
    if (checkButtonPress( false)==BUTTON_SELECT ) { aborted = true; } 
    if (aborted)  {  goto endPos ;  }


 
     // if camTrigSTart = 0 then use bulb mode. Open the shutter at the beginning and close at the end.
    //  Find out when to close the shutter. Do it here so that any INCs are included.
    
    if ( tmpCamTriggerStart == 0)  { tmpCamTrigStop = tmpFlashTrigStop + 200;     }      // close the camera 200 ms after the flash. 
    if ( tmpCamTriggerStart == 0 ) { triggerStart(focus); triggerStart(camera);  CamTriggered = true;  }  
 
 
    
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-   
    // FROM THIS POINT KEEP EVERYTHING TO ESSENTIAL OPERATIONS ONLY
 
    
    CamTriggered = false;
    camDone = false;
    flashTriggered = false; 
    flashDone = false;
    allDropsDone = true;
    
   
    // set the drops as not dropped
    // set the solenoid valve for each drop as not open
    for (int i=1; i<9; i++)   { dropped[i] = false;  solOpen[i] = false; }

    done=false;
    seqStartTime = millis(); 
    while (seqStartTime == millis())  // Make sure we are starting at the beginning of a millisecond
    {}
    seqStartTime = millis();
    
    while (!done)
    {
        if (!CamTriggered && !camDone)     { if (millis() - seqStartTime >= tmpCamTriggerStart   )  { triggerStart(camera);  CamTriggered = true;    }     }
        if (!flashTriggered && !flashDone) { if (millis() - seqStartTime >= tmpFlashTriggerStart )  { triggerStart(flash);   flashTriggered = true;  }     }
        
        // assume all drops are done  then check to see if any are not.
        allDropsDone = true;
        
        // loop through the drops and see if we need to start or stop the solenoids
        for ( currentDrop=1; currentDrop <= tmpNumDrops; currentDrop++)
        {
             if ( !dropped[currentDrop] && !solOpen[currentDrop] )  
             { 
                  if ( millis() - seqStartTime >= tmpDropStartTime[currentDrop] )   { openSolenoid(  tmpSol[currentDrop]);  solOpen[currentDrop] = true;     } 
             }  
             
             if (solOpen[currentDrop]) 
             { 
                  if ( millis() - seqStartTime >= tmpDropStopTime[currentDrop]  )   
                  { 
                       closeSolenoid( tmpSol[currentDrop] );  
                       solOpen[currentDrop] = false;   
                       dropped[currentDrop] = true; 
                  }   
             }
              
             if ( !dropped[currentDrop] ) { allDropsDone = false;}              
              
        } // for ( currentDrop=1; currentDrop < tmpNumDrops; currentDrop++)
        
        
        // check to see if need to stop the flash trigger or the camera triggers
        if (!flashDone) { if (flashTriggered && ( millis() - seqStartTime >= tmpFlashTrigStop ))  { triggerStop(flash);   flashDone = true;  }     }
        if (!camDone)   { if (CamTriggered   && ( millis() - seqStartTime >= tmpCamTrigStop ))    { triggerStop(focus);   triggerStop(camera);  camDone = true;  }    }
        
        if (allDropsDone && flashDone && camDone) {done = true;}
        
    } // while (!done)


    // at this point all drops and triggers should have been activated.


    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
    // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-  
    
    
    
    
/*
    // show old values
    if (showOldValues && !aborted)
    {
          lcd.setCursor(1, 0);   lcd.print(F("TD="));
          lcd.setCursor(13, 3);  lcd.print(F("Waiting")); 
         
          byte timerChar[4]  = { 45, 0, 124, 47 };    
          int count = 0;
          unsigned long startTime = millis();
          unsigned long startWaiting = startTime;
          unsigned long nowTime = 0;
          boolean timerDone = false;
          while (!timerDone )
          {
              nowTime = millis();
              if (nowTime - startTime >= 3000 ) { timerDone = true;  }
              if (ok_button.uniquePress() )                {  aborted = true; timerDone=true; } 
            
              if (nowTime - startWaiting >250 )            {   lcd.setCursor(19, 3);   lcd.write( timerChar[count] );
                                                               count++; if (count >3) {count = 0;} 
                                                               startWaiting = millis();
                                                            }
          }
          lcd.setCursor(19, 3); lcd.write(32);    
    }    
*/    
    


     // add INC times and goto the start
     if (repeatLoop[0] && !aborted )   
     {    
         byte j=0;
         
         for (int i=1; i<=numDrops[mode]; i++)
         {
              if (mode==QUICK_MODE) { j=i+9; } else { j=i; }
            
              //  0 = INC.  1= DEC
              if ( incDecFlag[j] ==0 ) 
              { 
                   tmpDropStartTime[i]  = tmpDropStartTime[i] +  incDropStartTime[j];  
                   tmpDropSize[i] = tmpDropSize[i] + incSize[j];
              }         
              else                     
              { 
                   tmpDropStartTime[i]  = tmpDropStartTime[i] -  incDropStartTime[j];
                   tmpDropSize[i] = tmpDropSize[i] - incSize[j];
                   if (tmpDropStartTime[i] <0 ){ tmpDropStartTime[i] = 0;  }   // sound buzzer
                   if (tmpDropSize[i] <0 ){ tmpDropSize[i] = 0; }              // sound buzzer
               }   


              tmpDropStopTime[i]   = tmpDropStartTime[i] + tmpDropSize[i];
         }
        
         
         tmpFlashTriggerStart = tmpFlashTriggerStart + flashTrigInc[mode];
         tmpFlashTrigStop     = tmpFlashTriggerStart + flashTriggerDuration;
         
         
         if (tmpCamTriggerStart != 0 )
         {
             tmpCamTriggerStart = tmpCamTriggerStart + camTrigInc[mode];
             tmpCamTrigStop =   tmpCamTriggerStart + camTriggerDuration;  
         }
             
     } // if (repeatLoop[0] && !aborted )   // add INC times and goto the start


     goto startPos; 
     // I know. GOTOs are bad.

     
    ;
    endPos:
    ;
     // close all 
     triggerStop(camera);
     triggerStop(focus);
     triggerStop(flash);
     closeSolenoid(1);
     closeSolenoid(2);
     closeSolenoid(3);
          

   
      
          
}   // void makeDrops()







