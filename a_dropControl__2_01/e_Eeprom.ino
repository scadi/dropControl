/*
  ______  ______  _____   _____    ____   __  __ 
 |  ____||  ____||  __ \ |  __ \  / __ \ |  \/  |
 | |__   | |__   | |__) || |__) || |  | || \  / |
 |  __|  |  __|  |  ___/ |  _  / | |  | || |\/| |
 | |____ | |____ | |     | | \ \ | |__| || |  | |
 |______||______||_|     |_|  \_\ \____/ |_|  |_|
           
*/



/*
****************************************
* Function eraseEeprom
* erases the saved data in the eeprom
* 
* passed: 
* global:  menuList[0] - used as the screen title
* 
* Returns:
*          
*
*/
void eraseEeprom()
{
    lcd.clear();
    lcd.setCursor(0,0);  lcd.print( menuList[0] );
    lcd.setCursor(3,2);  lcd.print(F("Erasing...."));         
    
    // I don't really need to write to every memory location but I feel it will alleviate any possible problems due to random values.      
    int percent = 0;
    for (long i=0; i<=1023; i++)
    {
        eepromWriteByte(0,i);
        percent = i*100 / 1023;
        lcd.setCursor(15,2);  lcd.print( percent ); lcd.print( "%" );
    }
    
    // need to set the eeprom initialized flag
    eepromWriteByte(99, eep_INIT_FLAG_ADDRESS);  
    
    delay(750);
}



/*
****************************************
* Function eepromWriteByte
* writes a byte value to eeprom
* 
* passed:  val - the value to write.
*          addr - the eeprom memory address/location
* global:  
* 
* Returns:  1 for success
*           0 for failed
*          
*
*/
boolean eepromWriteByte( byte val, int addr )
{   


    // check to see if we need to write the new value. 
    // There is no need to use a separate variable for this. Could use  if (EEPROM.read(addr) != val)
    // but it is easier to have a variable while debugging.
    byte readVal;
    readVal = EEPROM.read(addr);  
    
    // If the value is already correct then no need to update
    if (readVal != val)  
    {    
         EEPROM.write(addr, val) ;   
         delay(2);
         // check that the write was successful
         readVal = EEPROM.read(addr);
   }      

    if (readVal == val)  { return 1; }    else { return 0; }
    
}



/*
****************************************
* Function eepromWriteInt
* writes a 16bit int  value to eeprom. 
* writes the high byte first
* 
* passed:  val - the value to write.
*          addr - the first memory address in eeprom. ints are 2 bytes. So need 2 eeprom memory locations
* global:  
* 
* Returns:  1 for success
*           0 for failed
*          
*
*/
boolean eepromWriteInt( int val, int addr )
{

      byte flag = 0;
      byte buf[2];  
      buf[0] = (byte) ((val >> 8) & 0xff);
      buf[1] = (byte) (val & 0xff); 
      flag = eepromWriteByte(buf[0], addr );  
      flag = flag + eepromWriteByte(buf[1], addr+1 );
      if (flag==2)  { return 1; }    else { return 0; }
}



/*
****************************************
* Function eepromWriteLong
* writes a 32bit long  value to eeprom. 
* writes the high byte first
* 
* passed:  val - the value to write.
*          addr - the first memory address in eeprom. longs are 4 bytes. So need 4 eeprom memory locations
* global:  
* 
* Returns:  1 for success
*           0 for failed
*          
*
*/
boolean eepromWriteLong( long val, int addr )
{   

    byte flag = 0;
    byte buf[4];  
    buf[0] = (byte) ((val >> 24) & 0xff);
    buf[1] = (byte) ((val >> 16) & 0xff);
    buf[2] = (byte) ((val >> 8) & 0xff);
    buf[3] = (byte) (val & 0xff);
    flag = eepromWriteByte(buf[0], addr );
    flag = flag + eepromWriteByte(buf[1], addr+1 );
    flag = flag + eepromWriteByte(buf[2], addr+2 );  
    flag = flag + eepromWriteByte(buf[3], addr+3 );  
    if (flag==4)  { return 1; }    else { return 0; };
}





/*
****************************************
* Function eepromReadByte
* reads a single value (byte) from eeprom
* 
* passed:  addr - the memory address in eeprom. 
* global:  
* 
* Returns:  the value read from eeprom
*          
*
*/ 
byte eepromReadByte(int addr)
{
  byte val = EEPROM.read(addr);
  return val;
} 



/*
****************************************
* Function eepromReadInt
* reads an int value (16bit) from eeprom
* read 2 bytes and combine in to one int. 
* 
* passed:  addr - the memory address in eeprom. 
* global:  
* 
* Returns:  the value read from eeprom
* 
*/
int eepromReadInt(int addr)
{
    byte hiByte = EEPROM.read(addr);
    byte loByte = EEPROM.read(addr+1);
    int val = word(hiByte, loByte);
    return val;
} 


/*
****************************************
* Function eepromReadLong
* reads an long value (32bit) from eeprom
* read 4 bytes and combine in to one long 
* 
* passed:  addr - the start address in eeprom. 
* global:  
* 
* Returns:  the value read from eeprom
* 
*/             
long eepromReadLong(int addr)
{
  // hibyte first
    byte buff[3];
    buff[0] = EEPROM.read(addr);
    buff[1] = EEPROM.read(addr+1);
    buff[2] = EEPROM.read(addr+2);
    buff[3] = EEPROM.read(addr+3);  

    long val = 0;
    val = buff[0];
    val = (val <<8) + buff[1];
    val = (val <<8) + buff[2];
    val = (val <<8) + buff[3];
    return val;
}






/*

****************************************
* Function loadSaveSettings
* loads or saves the main settings
* 
* passed:  saveset
*          
* global:  eep_SETTINGS_START_ADDRESS - start address for the setting memory area
* 
* Returns:
*
* byte saveSet: 0=load. 1=save
*/

/*
****************************************
* Function loadSaveSettings
* save or load the main settings to or from eeprom 
* 
* 
* passed:  saveSet. If saveSet==true then save settings else load the settings
* global:  
* 
* Returns:  allSavedOK. Flag to show if the operation was successful or not (not implemented yet)
* 
*/ 
boolean loadSaveSettings( byte saveSet)
{
        
        int address = eep_SETTINGS_START_ADDRESS;
        // savedOK and allSavedOk not currently used
        boolean savedOK = false;
        boolean allSavedOK = true;
        
        // BOOLEAN mode
        if (saveSet) {  savedOK = eepromWriteByte( mode, address );    }   
        else         {  mode = eepromReadByte( 0 );              }
        address = address + 1;
        
         //  BOOLEAN mirrorLockup   
        if (saveSet) {  savedOK = eepromWriteByte(mirrorLockup, address );    }
        else         {  mirrorLockup = eepromReadByte( address );             }
        address = address + 1;
        
        // INT mirrorLockupDelay  
        if (saveSet) {  savedOK = eepromWriteInt( mirrorLockupDelay, address ); }
        else         {  mirrorLockupDelay = eepromReadInt( address );           }
        address = address + 2;
        
        // INT camTriggerDuration 
        if (saveSet) {  savedOK = eepromWriteInt( camTriggerDuration, address ); }
        else         {  camTriggerDuration = eepromReadInt( address );           }
        address = address + 2;
        
        // INT flashTriggerDuration 
        if (saveSet) {  savedOK = eepromWriteInt( flashTriggerDuration, address ); }
        else         {  flashTriggerDuration = eepromReadInt( address );           }
        address = address + 2;
        
        //  BOOLEAN repeatLoop    
        if (SAVE_DATA) {  savedOK = eepromWriteByte(repeatLoop[0], address );    }
        else          {  repeatLoop[0] = eepromReadByte( address );             }
        address = address + 1;
        
        //  LONG loopDelay    
        if (saveSet) {  savedOK = eepromWriteLong(loopDelay[0], address );     }
        else         {  loopDelay[0] = eepromReadLong( address );              }
        address = address + 4;
        
        //  BOOLEAN showOldValues    
        if (SAVE_DATA){  savedOK = eepromWriteByte(showOldValues, address );    }
        else          {  showOldValues = eepromReadByte( address );             }
        address = address + 1;
        
        // saved settings check flag
        savedOK = eepromWriteByte(99, eep_HAVE_SETTINGS_FLAG_ADDRESS ); 
        
        
        return allSavedOK;
}






/*

byte SAVE_DATA; 0=load, 1=save
byte dataSet. The data bank/area to use. 

*/
/*
****************************************
* Function loadSaveDropData
* save or load the user drop data to or from eeprom 
* 
* 
* passed:  savingData. If saveSet==true then save data else load data
*          mode - full mode or quick mode
*          dataSet - which memory area to use. There are 3 memory areas 
*          displayScreen
* global:  
* 
* Returns:  true or false. True means successful.  False means failed
* 
*/

byte loadSaveDropData(byte savingData, byte mode, byte dataSet, byte displayScreen)
{
  
    if (displayScreen)
    {
            lcd.clear();
            if (savingData)   {  lcd.setCursor(0,0); lcd.print(F("SAVING DATA TO LOC ")); lcd.print( dataSet );    }
            else              {  lcd.setCursor(0,0); lcd.print(F("LOADING DATA: SET "));  lcd.print( dataSet );    }
            lcd.setCursor(1,1);  lcd.print(F("Done = 0%"));         
    }

    boolean savedOK = false;
    int arrSize = 0; 
    int arrPos = 0;
    byte j=0;
    int localAddress = 0;
    int address = 0;
    int percent = 0;
    boolean allSavedOK = true;
   
    address = eep_MEM_LOCATIONS_ADDRESS[mode][dataSet];

    if (mode == QUICK_MODE)  { arrSize = 3;  arrPos = 10; }
    else                     { arrSize = 9;  arrPos = 1;  }



//  LONG camTrigStart    
    if (savingData) {  savedOK = eepromWriteLong(camTrigStart[mode], address );    }
    else            {  camTrigStart[mode] = eepromReadLong( address );             }
    address = address + 4;

    if (savedOK ==0) { allSavedOK = false; }

//  LONG flashTrigStart    
    if (savingData) {  savedOK = eepromWriteLong( flashTrigStart[mode], address );    }
    else            {  flashTrigStart[mode] = eepromReadLong( address );              }
    address = address + 4;

    if (savedOK ==0) { allSavedOK = false; }

//  INT camTrigInc    
    if (savingData) {  savedOK = eepromWriteInt( camTrigInc[mode], address );    }
    else            {  camTrigInc[mode] = eepromReadInt( address );              }
    address = address + 2;  

    if (savedOK ==0) { allSavedOK = false; }
    if (displayScreen) { lcd.setCursor(8, 1);   lcd.print(F("5%  ")); }
  
//  INT flashTrigInc    
    if (savingData) {  savedOK = eepromWriteInt(flashTrigInc[mode], address );   }
    else            {  flashTrigInc[mode] = eepromReadInt( address );            }
    address = address + 2;

//  byte numDrops.    
    localAddress = address;
    if (savingData)     {    savedOK = eepromWriteByte( numDrops[mode], address );      }  
    else                {    numDrops[mode] = eepromReadByte( address );                }
    address = address + 1; 

    if (savedOK ==0) { allSavedOK = false; } 

 // long 4 - dropStartTime[].    
    for (byte i=arrPos; i< arrPos+arrSize; i++)
    {
        if (savingData)    { savedOK = eepromWriteLong( dropStartTime[i], address );    }
        else               { dropStartTime[i] = eepromReadLong( address );              }
        address = address + 4;
    }
    
    if (savedOK ==0) { allSavedOK = false; }     
    if (displayScreen) { lcd.setCursor(8, 1);   lcd.print(F("25% ")); }
 
 // dropSize[].    
    for (byte i=arrPos; i< arrPos + arrSize; i++)    
    { 
        if (savingData) { savedOK = eepromWriteLong( dropSize[i], address );     }   
        else            { dropSize[i] = eepromReadLong( address );               } 
        address = address + 4; 
    }

    if (savedOK ==0) { allSavedOK = false; }
 
//  sol[].      
    for (byte i=arrPos; i< arrPos+arrSize; i++) 
    { 
        if (savingData) { savedOK = eepromWriteByte( sol[i], address );   }
        else            {  sol[i] = eepromReadByte( address );            }
        address = address + 1;
    }    
    
    if (savedOK ==0) { allSavedOK = false; }    
    if (displayScreen)  {  lcd.setCursor(8, 1);   lcd.print(F("50% "));     } 


//  incDropStartTime[].   
    for (byte i=arrPos; i< arrPos+arrSize; i++)  
    { 
        if (savingData) { savedOK = eepromWriteLong( incDropStartTime[i], address ); }
        else            { incDropStartTime[i] = eepromReadLong( address );           } 
        address = address + 4;    
    }   

    if (savedOK ==0) { allSavedOK = false; } 
    if (displayScreen)   { lcd.setCursor(8, 1);  lcd.print(F("75% "));  }
   
   
        

//  incSize[].    
    for (byte i=arrPos; i< arrPos+arrSize; i++) 
    { 
        if (savingData) { savedOK = eepromWriteLong( incSize[i], address );  }
        else            { incSize[i] = eepromReadLong( address );            }
        address = address + 4;
    }

    if (savedOK ==0) { allSavedOK = false; } 

    // update the number of times written
    if (savingData)
    {
        
        address = eep_NUM_TIMES_WRITTEN_ADDRESS[mode][dataSet];
        int tmp = 0;
        tmp = eepromReadInt(address);

      
        tmp++;
        eepromWriteInt(tmp, address);        
    }


    if (displayScreen)   { lcd.setCursor(8, 1);  lcd.print(F("100%"));  }


    if (displayScreen)
    {
            lcd.setCursor(1, 2);
            if (savingData)  
            {
                if (allSavedOK)   {  lcd.print(F("SAVE COMPLETE"));    }
                else              {  lcd.print(F("CANNOT SAVE"));      }
            }
            else                  {  lcd.print(F("LOAD COMPLETE"));    }   
          
            lcd.setCursor(1, 3);   lcd.print(F("OK to EXIT"));                    
            lcd.setCursor(0, 3);   lcd.write(rightArrow);
       

     }

    // if load data then return 1.
    // if save data return the success or failure flag
    if (savingData) { return allSavedOK;}  else {return 1;}
    
}






 

    



