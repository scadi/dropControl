/*


  __  __  ______  _   _  _    _ 
 |  \/  ||  ____|| \ | || |  | |
 | \  / || |__   |  \| || |  | |
 | |\/| ||  __|  | . ` || |  | |
 | |  | || |____ | |\  || |__| |
 |_|  |_||______||_| \_| \____/ 
                                
                                

*/




/*
****************************************
* Function menuSelection
* display the menu. Select a menu item
* 
* passed:  currentItem. The menu item to start at
* 
* global:  menuList[]. The list of menu options. menuItem[0] is the menu title
*          numMenuItems. The number of menu elements
*          drawScreen.   Flag, if true then draw the screen. False. do not draw the screen - TODO make this local
*          
* Returns: currentItem. The menu item that has been selected
*
*/

int menuSelection()
{
     
    if (currentItem > numMenuItems)
    {
          lcd.clear();
          lcd.setCursor(0,0);  lcd.print(F("ERROR"));
          lcd.setCursor(0,1);  lcd.print(F("CI>MI"));
          while (1) {;}
    }


    byte keyPress = 0;

 
    if (drawScreen)  
    {
         lcd.clear();
         lcd.setCursor(0,0);  lcd.print(menuList[0]);   
         delay (250);
         if (currentItem<4) { topPos=1; } else { topPos = currentItem-2; } 
         if (currentItem<4) { arrowPos=currentItem; } else { arrowPos = 3; }
         
         drawMenu( topPos, arrowPos, numMenuItems);
         moveCursor(arrowPos, arrowPos);
         // lcd.setCursor(0,arrowPos);  lcd.write(rightArrow);
          
    } // if (drawScreen) 


  
    boolean done = false;
    while ( !done )
    {
         keyPress = checkButtonPress(true);
         
        #ifdef DEBUG  
            if (keyPress != 0 ) { Serial.print("key = "); Serial.println(keyPress);      }
        #endif           
        
        
         if ( keyPress == BUTTON_SELECT )  { done = true;} 
         
             
         else if ( keyPress == BUTTON_DOWN )
         { 
             if (currentItem < numMenuItems) // can we move down?
             {
                  if (arrowPos < 1)   // just move the cursor
//                  {
//                       //lcd.setCursor(0, arrowPos);  lcd.write(32);  // clear existing arrow
//                       arrowPos++; 
//                       currentItem++;
//                       // no change to topPos
//                       moveCursor(arrowPos-1, arrowPos);
//                       //lcd.setCursor(0, arrowPos);  lcd.write(rightArrow);
//                       
//                  }
//                  else
//                  // scroll the menu list down. Leave the cursor at line 3
//                  {
                    currentItem++;
                    topPos++;
                    drawMenu(topPos, arrowPos, numMenuItems);
//                  }  
             }

         } // if ( keyPress == BUTTON_DOWN )



         else if ( keyPress == BUTTON_UP)
         {    
              if (currentItem > 1) // can we go up?
              {
                
//                   if (arrowPos > 1) // just move the cursor up
//                   {
//                        //lcd.setCursor(0, arrowPos);  lcd.write(32);  // clear existing arrow
//                        arrowPos--; 
//                        currentItem--;
//                        // no change to topPos
//                        moveCursor(arrowPos+1, arrowPos);
//                        //lcd.setCursor(0, arrowPos);  lcd.write(rightArrow);
//                   }            
//                   else
//                   // need to scroll the menu up one
//                   {
                        // no change to arrowPos
                        currentItem--;
                        topPos--;
                        drawMenu( topPos, arrowPos, numMenuItems);
//                   }               
              }

         } // if ( keyPress == BUTTON_UP)
         
    } //while ( !done )  



    return currentItem; 
}



/*
****************************************
* Function drawMenu
* prints the menu items at LCD line 1 to 3
* 
* passed:  top - item at the top position
*          ap - the position of the cursor
*          nmi - the number of menu items.
*          
* global:  tenSpaces. Used to clear the line
* 
* Returns:
*
*/
// top=topPos, ap=arrowPos, nmi = numMenuItems
void drawMenu(byte top, byte ap, byte nmi)
{
      if (nmi>3) { nmi=1;} 
      for (int i = 1; i <= nmi; i++)
      {
            lcd.setCursor(0, i);  lcd.print(tenSpaces); lcd.print(tenSpaces);  // 20 spaces to clear the line
            lcd.setCursor(1, i);  lcd.print(menuList[top+(i-1)]);
      }
      lcd.setCursor(0, ap);  lcd.write(rightArrow);
      moveCursor(ap, ap);
}

                
/*
****************************************
* Function moveCursor
* moves the menu cursor
* 
* passed:  old position. New position
* 
* global:  currentItem
*          menuItemSelectable[]
*          rightArrow (the value of the right arrow chr
* 
* Returns:
*
*/
void moveCursor(byte oldyPos, byte newyPos)
{
     lcd.setCursor(0, oldyPos);  lcd.write(32);  
     lcd.setCursor(0, newyPos);  
     if ( menuItemSelectable[currentItem]==true )   { lcd.write(rightArrow); }  else   { lcd.write(1); } 

}




/*
****************************************
* Function printMenuList
* prints all items in the current menuList to the serial monitor
* used for debugging
* 
* passed:  
* 
* global:  menuList
* 
* Returns:
*
*/
void printMenuList()
{
     for (int i=0; i<= numMenuItems; i++)    
     {
        Serial.print(i); Serial.print(" - ");  Serial.println(menuList[i]);

     }
    
     Serial.println(" ");
    
    
}

