

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
   if (trig ==1) {  digitalWrite(cam_TrigShutterPin , HIGH);  }
   if (trig ==2) {  digitalWrite(cam_TrigFocusPin, HIGH);     }
   if (trig ==3) {  digitalWrite(flash_TrigPin, HIGH);        }
}


/*
****************************************
* Function triggerStop
* stops the trigger; camera focus, camera shutter, flash
* 
* passed: 
*    trig.   1=SHUTTER.  2=FOCUS.  3=FLASH.
*         
* global:  
* 
* Returns:
* 
* I can remove this function and write the pin in the main routine.        
*/
void triggerStop(int trig)
{
   if (trig ==1) {  digitalWrite(cam_TrigShutterPin, LOW);   }
   if (trig ==2) {  digitalWrite(cam_TrigFocusPin, LOW);     }
   if (trig ==3) {  digitalWrite(flash_TrigPin, LOW);        }
}




/*
****************************************
* Function openSolenoid
* opens a solenoid valve
* 
* passed: 
*    s. The value to open
*         
* global:  
* 
* Returns:
* 
*/
void openSolenoid(byte s)
{
    // done this way so I can use a constant for the sol pin
    if      (s==1) { digitalWrite(sol1_TriggerPin, HIGH);    }
    else if (s==2) { digitalWrite(sol2_TriggerPin, HIGH);    }
    else if (s==3) { digitalWrite(sol3_TriggerPin, HIGH);    }
    else if (s==4) { digitalWrite(sol4_TriggerPin, HIGH);    }    
    else if (s==5) { digitalWrite(sol5_TriggerPin, HIGH);    }   
    else if (s==6) { digitalWrite(sol6_TriggerPin, HIGH);    }    
    
}


/*
****************************************
* Function closeSolenoid
* closes a solenoid valve
* 
* passed: 
*    s. The value to close
*         
* global:  
* 
* Returns:
* 
*/
void closeSolenoid(byte s)
{
     // done this way so I can use a constant for the sol pin
    if      (s==1) { digitalWrite(sol1_TriggerPin, LOW);    }
    else if (s==2) { digitalWrite(sol2_TriggerPin, LOW);    }
    else if (s==3) { digitalWrite(sol3_TriggerPin, LOW);    }
    else if (s==4) { digitalWrite(sol4_TriggerPin, LOW);    }    
    else if (s==5) { digitalWrite(sol5_TriggerPin, LOW);    }   
    else if (s==6) { digitalWrite(sol6_TriggerPin, LOW);    }   
}







