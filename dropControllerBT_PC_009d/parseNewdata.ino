
/*
****************************************
* Function parseNewData
* parses the text help in receivedChars[]
* 
* passed:
*  
* global: 
*         receivedChars[]
*
* Returns:
*          
* Sets:
*       numDrops
*       sol[]
*       dropStartTime[]
*       dropStopTime[]
*       flashCommand[]
*       cameraCommand[]
*       mirrLockupCommand[]
*       haveNewDropData
*
*/
void parseNewData() 
{

          // HELLO message
          if (strcmp(receivedChars, "HELLO")  == 0)
          {
               // send a HELLO message back to the controller app
               if (DEVICE_TYPE==1) { Serial.print("<HELLO>");    }
               if (DEVICE_TYPE==2) { BTserial.print("<HELLO>");  }
             
               // turn on the waiting LED to show we are connected
               digitalWrite(LED_WAITING, HIGH); 
          }
          
          
           // BYE message
          if (strcmp(receivedChars, "BYE")  == 0)
          {
             // turn off the waiting LED to show we are no longer connected
             digitalWrite(LED_WAITING, LOW); 
          }         
           
           
          // Firmware version
          if (strcmp(receivedChars, "VERSION")  == 0)
          {
            
               if (DEVICE_TYPE==1) 
               { 
                  Serial.print("<V,");
                  Serial.print(verNum);        Serial.print(",");
                  Serial.print(fileName);      Serial.print(",");
                  Serial.print(compileDate);   Serial.print(">");
               }
               
               if (DEVICE_TYPE==2) 
               { 
                  BTserial.print("<V,");
                  BTserial.print(verNum);        BTserial.print(",");
                  BTserial.print(fileName);      BTserial.print(",");
                  BTserial.print(compileDate);   BTserial.print(">");
               }            
          }              
           
           
          // Drain Valve
          // V11 - Valve 1 open
          // V10 - Valve 1 close
          if ( receivedChars[0]=='V'   )
          {  
                   byte valve = receivedChars[1] - 48;  //  -48 converts an ascii "1" in to the value 1
                   byte flag =  receivedChars[2] - 48;
                  
                   // open valve
                   if (flag == 1) {  openSolenoid(valve);    }
                  
                   // close valve
                   if (flag == 0) {  closeSolenoid(valve);   }
          }   



          // NDx  Number of drops
          if (  (receivedChars[0]=='N') && (receivedChars[1]=='D')     )
          {  
                    numDropsCheck = receivedChars[2] - 48;
          }


          // copy the drop data to dropCommand[]
          if (receivedChars[0]=='D') 
          {   
                   numDrops++;
                   strcpy(dropCommand[numDrops], receivedChars);
          }    

               
          // copy the flash trigger command to flashCommand[]
          // FT0540,050
          if (receivedChars[0] == 'F'  ) 
          {    
                   strcpy(flashCommand, receivedChars);
                   haveFT = true;
          }
     


          // copy the camera trigger command to cameraCommand[]
          // CT0000,050    
          if (receivedChars[0] == 'C'  ) 
          {  
                   strcpy(cameraCommand, receivedChars);
                   haveCT = true;
          }          
            
          // copy the mirror lockup trigger command to mirrLockupCommand[]
          // MN1000                                                 
          if (receivedChars[0] == 'M'  ) 
          {  
                   strcpy(mirrLockupCommand, receivedChars);
                   haveML = true;
          }   
               
              
          // EOD  
          if (  (receivedChars[0]=='E') && (receivedChars[1]=='O') && (receivedChars[2]=='D')    )
          {  
                    haveNewDrop = true; 
                   // TO DO: make beep or flash something
          }

          haveNewData = false;  



} // void parseNewData() 







