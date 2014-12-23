/*-------------------Bank and mode change ----------------
  Since Modes are changed by pressing bank up and bank down. Bank up or bank down
  changes are checked on button onUp events while mode changes are checked on down states
  (checking is both have been pressed).
  
  Bank will blink in preset mode if a preset hasnt been selected. IF after moving through banks 
  bank is the same as where current preset was pressed then stop blinking.

*/
void bankChange(){//onUp
      
      if(BD && BUTTON1)return;
      
     //Bank Up or Down - Change Bank first checking that MODE hasn't changed
     if(BU && MODE_HAS_CHANGED ==false && LOOP8_HAS_CHANGED ==false /*&& MODE != LOOP_MODE*/){
       BANK_IDX =(++BANK_IDX)%BANK_MAX;
       Serial.print("BANK Number: ");Serial.println(BANK_IDX);
       hasPresetBeenSelected = false;

     }
     if(BD && MODE_HAS_CHANGED == false && MODE != LOOP_MODE){
        --BANK_IDX;
         if(BANK_IDX < 0)BANK_IDX += BANK_MAX;
         Serial.print("BANK Number: ");Serial.println(BANK_IDX);
         hasPresetBeenSelected = false;

      }
       //has Bank returned to where preset was pressed?
       if(hasPresetBeenToggled)PREV_BANK = BANK_IDX;
       hasBankReturned = (PREV_BANK == BANK_IDX);
       if(hasBankReturned && !MODE_HAS_CHANGED && !LOOP8_HAS_CHANGED && MODE == PRESET_MODE && (BU|BD)){
         Serial.println("bank has Returned to where last preset was selected");
         hasPresetBeenToggled = false;
        }
       
}
void modeChange(){//onDown
     
     MODE_HAS_CHANGED =false;

     if(BD & BU){
         MODE_HAS_CHANGED= true;
         MODE++;
         MODE = MODE % 3;

         if(MODE ==PRESET_MODE){Serial.println("Preset Mode");}    
         if(MODE ==EDIT_MODE) { Serial.println("Edit Mode");bypassAllLoops();}
         if(MODE ==LOOP_MODE) {Serial.println("Loop Mode"); bypassAllLoops();}

     
     }else if(BD | BU){ 
       hasPresetBeenToggled = false;
     }
     
     //Manage the mode display timer
     
     if(MODE_HAS_CHANGED){
       timer.enable(timerId);
       timer.disable(timerId2);
     }
     else {
       timer.enable(timerId2);
       timer.disable(timerId);
       
     }
     
//     hasPresetBeenSaved = false;
//     Serial.println("Mode has changed: ");Serial.println(MODE_HAS_CHANGED);
     
}

void clearPresets(){//On hold
  
  if(BD && BUTTON1){
    
    for (int i = 0; i < 512; i++)
      EEPROM.write(i, 0); 
      
      Serial.println("Presets erased");
  }
  
}

void setWritePresetAddress(int pin){// on hold
     switch(pin){
    case P1B:
      if(!LOOP5_HAS_CHANGED){
        PRESET_WRITE_ADDRESS = 1 + NUM_LOOPS*BANK_IDX;
      }
    break;
    case P2B:
    if((!LOOP5_HAS_CHANGED) && (!LOOP6_HAS_CHANGED)){
      PRESET_WRITE_ADDRESS = 2 + NUM_LOOPS*BANK_IDX;  
    }    
    break;
    case P3B:
     if((!LOOP6_HAS_CHANGED) && (!LOOP7_HAS_CHANGED)){
      PRESET_WRITE_ADDRESS = 3 + NUM_LOOPS*BANK_IDX;
     }  

    break;
    case P4B:
      if((!LOOP7_HAS_CHANGED) && (!LOOP8_HAS_CHANGED)){
          PRESET_WRITE_ADDRESS = 4 + NUM_LOOPS*BANK_IDX;
       }
    break;
  } 
  
  //If loop is on and has been selected
  if(LOOP5 && LOOP5_HAS_CHANGED){ PRESET_WRITE_ADDRESS = 5 + NUM_LOOPS*BANK_IDX;}
  if(LOOP6 && LOOP6_HAS_CHANGED){ PRESET_WRITE_ADDRESS = 6 + NUM_LOOPS*BANK_IDX;}
  if(LOOP7 && LOOP7_HAS_CHANGED){ PRESET_WRITE_ADDRESS = 7 + NUM_LOOPS*BANK_IDX;}
  if(LOOP8 && LOOP8_HAS_CHANGED){ PRESET_WRITE_ADDRESS = 8 + NUM_LOOPS*BANK_IDX;}
  
}


