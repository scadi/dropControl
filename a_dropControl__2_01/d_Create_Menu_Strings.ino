
/*
   _____  _____   ______         _______  ______   
  / ____||  __ \ |  ____|    /\ |__   __||  ____| 
 | |     | |__) || |__      /  \   | |   | |__   
 | |     |  _  / |  __|    / /\ \  | |   |  __|  
 | |____ | | \ \ | |____  / ____ \ | |   | |____    
  \_____||_|  \_\|______|/_/    \_\|_|   |______|    

  __  __  ______  _   _  _    _      _____  _______  _____   _____  _   _   _____   _____ 
 |  \/  ||  ____|| \ | || |  | |    / ____||__   __||  __ \ |_   _|| \ | | / ____| / ____|
 | \  / || |__   |  \| || |  | |   | (___     | |   | |__) |  | |  |  \| || |  __ | (___  
 | |\/| ||  __|  | . ` || |  | |    \___ \    | |   |  _  /   | |  | . ` || | |_ | \___ \ 
 | |  | || |____ | |\  || |__| |    ____) |   | |   | | \ \  _| |_ | |\  || |__| | ____) |
 |_|  |_||______||_| \_| \____/    |_____/    |_|   |_|  \_\|_____||_| \_| \_____||_____/ 

*/






/*
****************************************
* Function createDropMenuStrings 
* 
* passed: 
*         
* global: 
* 
* Returns: 
* 
*/
void createDropMenuStrings(byte startMenuPos, byte startDropPos, byte numberDrops)
{
       int menuPos = 0;
       int dropPos = 0;
      
       for (int i=1; i <= numberDrops; i++)
       {
           menuPos = i-1 + startMenuPos ;
           dropPos = i-1 +  startDropPos ;
           createDropMenuString(menuPos, dropPos);
       }    
}         




/*
****************************************
* Function createDropMenuString 
* 
* passed: 
*         
* global: 
* 
* Returns: 
* 
*/
void createDropMenuString(byte menuRow, byte drop)
{
      long dropStopTime = 0;
      strcpy(menuList[menuRow], "D" );
    
      if (mode==QUICK_MODE) { drop = drop+9; } 
    
      if (mode==QUICK_MODE) { numberToString (drop-9,1); }  else { numberToString (drop,1); } 
                                               strcat(menuList[menuRow], numberString );    strcat(menuList[menuRow],"  ");
      numberToString (sol[drop], 1);           strcat(menuList[menuRow], numberString );    strcat(menuList[menuRow], " ");
      numberToString (dropStartTime[drop], 4); strcat(menuList[menuRow], numberString );    strcat(menuList[menuRow], " ");
      numberToString (dropSize[drop], 3);      strcat(menuList[menuRow], numberString );    strcat(menuList[menuRow], " ");
      dropStopTime = dropStartTime[drop] + dropSize[drop];  numberToString (dropStopTime, 4);  strcat(menuList[menuRow], numberString );   

}



/*
****************************************
* Function  createIncMenuString
* 
* passed: 
*         
* global: 
* 
* Returns: 
* 
*/
void createIncMenuString(byte menuRow, byte drop, byte f)
{
      if (mode==QUICK_MODE) { drop = drop+9; } 
      
      if (f==1) {   
                     if    (incDecFlag[drop] ==0) { strcpy(menuList[menuRow]," INC  ");  }
                     else                         { strcpy(menuList[menuRow]," DEC  ");  }
                }
      else      { 
                    strcpy(menuList[menuRow]," I D");  
                    if (mode==QUICK_MODE) { numberToString (drop-9,1); }  else { numberToString (drop,1); }      
                    strcat(menuList[menuRow], numberString );   
                    strcat(menuList[menuRow]," ");   
                 }
    
      numberToString (incDropStartTime[drop], 4);  strcat(menuList[menuRow], numberString );   strcat(menuList[menuRow]," ");     
      numberToString (incSize[drop], 3);           strcat(menuList[menuRow], numberString );   ;

}



/*
****************************************
* Function createLoadSaveMenuStrings
* creates the menu strings used in load, save, set default.
* 
* 
* passed: 
*         
* global:  menuList
*          numMenuItems
* 
* Returns: 
* 
*/
void createLoadSaveMenuStrings()
{
        strcpy(menuList[1],"1:");    
        strcpy(menuList[2],"2:"); 
        strcpy(menuList[3],"3:"); 
        strcpy_P( menuList[4], (char*)pgm_read_word(&common[2]));  // = "EXIT"
        
        int val=0;
        
        val = eepromReadInt( eep_NUM_TIMES_WRITTEN_ADDRESS[mode][1] );
        if (val == 0 ) 
        {
            strcat(menuList[1],"EMPTY");  
        } 
        else 
        {
            strcat(menuList[1],"DATA  [");  
            numberToString(val,4); 
            strcat(menuList[1],numberString); 
            strcat(menuList[1],"]");  
        }
        
        val = eepromReadInt( eep_NUM_TIMES_WRITTEN_ADDRESS[mode][2] );
        if (val == 0 ) 
        {
            strcat(menuList[2],"EMPTY");  
        } 
        else 
        {
            strcat(menuList[2],"DATA  [");  
            numberToString(val,4); 
            strcat(menuList[2],numberString); 
            strcat(menuList[2],"]");  
        }
        
        val = eepromReadInt( eep_NUM_TIMES_WRITTEN_ADDRESS[mode][3] );
        if (val == 0 ) 
        {
            strcat(menuList[3],"EMPTY");  
        } 
        else 
        {
            strcat(menuList[3],"DATA  [");  
            numberToString(val,4); 
            strcat(menuList[3],numberString); 
            strcat(menuList[3],"]");   
        }
        
        // find the default data set
        val = eepromReadByte(eep_DEFAULT_SAVED_DATA_ADDRESS[mode]);
        if (val >=1 and val <=3) {   strcat(menuList[val]," *") ;    }
        numMenuItems = 4;  
}


/*
****************************************
* Function  createFlashTriggerMenuLine
* 
* passed:  menuRow - the row on the LCD
*         
* global: 
* 
* Returns: 
* 
*/
void createFlashTriggerMenuLine(int menuRow)
{
       strcpy(menuList[menuRow],"FLASH:"); numberToString (flashTrigStart[mode], 4);  strcat(menuList[menuRow], numberString );
       strcat(menuList[menuRow], " INC: " ); numberToString (flashTrigInc[mode], 3);   strcat(menuList[menuRow], numberString );  
}



/*
****************************************
* Function createCamTriggerMenuLine 
* 
* passed:  menuRow - the row on the LCD
*         
* global: 
* 
* Returns: 
* 
*/
void createCamTriggerMenuLine(int menuRow)
{
       strcpy(menuList[menuRow],"  CAM:"); numberToString (camTrigStart[mode], 4);  strcat(menuList[menuRow], numberString );
       strcat(menuList[menuRow], " INC: " ); numberToString (camTrigInc[mode], 3);   strcat(menuList[menuRow], numberString );  
}








