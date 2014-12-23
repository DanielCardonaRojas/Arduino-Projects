/* ----------------- Selection Logic ------------------
    Selection modes must be tested onUP events because simulteaneous button presses
    are monitored in button down events.
    
*/
void presetModeSelection(int pin){//called on button up

  switch(pin){
    case P1B:
     if(!LOOP5_HAS_CHANGED){
      LOOP1 = !LOOP1; //Toggle state off is bypass.
      PRESET_READ_ADDRESS = 1 + NUM_LOOPS*BANK_IDX;
      bitmaskRead();
      LOOP2=LOOP3=LOOP4=LOOP5=LOOP6=LOOP7=LOOP8=false; //only one at a time in preset mode 
     }
    break;
    case P2B:
     if((!LOOP5_HAS_CHANGED) && (!LOOP6_HAS_CHANGED)){
      LOOP2 = !LOOP2;
      PRESET_READ_ADDRESS = 2 + NUM_LOOPS*BANK_IDX;
      bitmaskRead();
      LOOP1=LOOP3=LOOP4=LOOP5=LOOP6=LOOP7=LOOP8=false;

     }
    break;
    case P3B:
     if((!LOOP6_HAS_CHANGED) && (!LOOP7_HAS_CHANGED)){
      LOOP3 = !LOOP3;
      PRESET_READ_ADDRESS = 3 + NUM_LOOPS*BANK_IDX;
      bitmaskRead();
      LOOP2=LOOP1=LOOP4=LOOP5=LOOP6=LOOP7=LOOP8=false;

     }
    break;
    case P4B:
     if((!LOOP7_HAS_CHANGED) && (!LOOP8_HAS_CHANGED)){
      LOOP4 = !LOOP4;
      PRESET_READ_ADDRESS = 4 + NUM_LOOPS*BANK_IDX;
      bitmaskRead();//only read preset if on
      LOOP2=LOOP3=LOOP1=LOOP5=LOOP6=LOOP7=LOOP8=false;
     }
    break;
  }
  hasPresetBeenToggled = true;
  hasPresetBeenSelected = LOOP1|LOOP2||LOOP3|LOOP4|LOOP5|LOOP6|LOOP7|LOOP8;
  
  if(LOOP5 && (!BUTTON1) && (!BUTTON2)){ PRESET_READ_ADDRESS = 5 + NUM_LOOPS*BANK_IDX;bitmaskRead();}
  else if(LOOP6 && (!BUTTON2) && (!BUTTON3)){ PRESET_READ_ADDRESS = 6 + NUM_LOOPS*BANK_IDX;bitmaskRead();}
  else if(LOOP7 && (!BUTTON3) && (!BUTTON4)){ PRESET_READ_ADDRESS = 7 + NUM_LOOPS*BANK_IDX;bitmaskRead();}
  else if(LOOP8 && (!BUTTON4) && (!BU)){ PRESET_READ_ADDRESS = 8 + NUM_LOOPS*BANK_IDX;bitmaskRead();}
  
}

void doublePressPresetModeSelection(){//on down
  
   //Select other loops 5,6,7,8 - 5 from Button 1&2,6 from 2&3, 8 from 4&BNK_U
     
   if(BUTTON1 & BUTTON2){//Preset 5 selected?
   LOOP5 = !LOOP5; LOOP5_HAS_CHANGED = true;
   LOOP2=LOOP3=LOOP4=LOOP1=LOOP6=LOOP7=LOOP8=false; //only one at a time in preset mode 
   }else LOOP5_HAS_CHANGED = false;
   
   if(BUTTON2 & BUTTON3){//Preset 6 selected?
   LOOP6 = !LOOP6; LOOP6_HAS_CHANGED = true;
   LOOP2=LOOP3=LOOP4=LOOP5=LOOP1=LOOP7=LOOP8=false; 
   
   }else LOOP6_HAS_CHANGED = false;
   
   if(BUTTON3 & BUTTON4){//Preset 7 selected?
   LOOP7 = !LOOP7; LOOP7_HAS_CHANGED = true;
   LOOP2=LOOP3=LOOP4=LOOP5=LOOP6=LOOP1=LOOP8=false;
   }else LOOP7_HAS_CHANGED = false;
   
   if(BUTTON4 & BU){//Preset 8 selected?
   LOOP8 = !LOOP8; LOOP8_HAS_CHANGED = true;
   LOOP2=LOOP3=LOOP4=LOOP5=LOOP6=LOOP7=LOOP1=false;
   }else LOOP8_HAS_CHANGED = false;
}
/* --------------Edit Mode Selection -------------*/
void editModeSelection(int pin){//onUP 

   switch(pin){
    case P1B:
      if(!LOOP5_HAS_CHANGED){
        LOOP1 = !LOOP1; 
        printSelectionStatus();  
      }
    break;
    case P2B:
    if((!LOOP5_HAS_CHANGED) && (!LOOP6_HAS_CHANGED)){
      LOOP2 = !LOOP2;
      printSelectionStatus();  
    }    
    break;
    case P3B:
     if((!LOOP6_HAS_CHANGED) && (!LOOP7_HAS_CHANGED)){
      LOOP3 = !LOOP3;
      printSelectionStatus();    
     }  

    break;
    case P4B:
      if((!LOOP7_HAS_CHANGED) && (!LOOP8_HAS_CHANGED)){
          LOOP4 = !LOOP4;
          printSelectionStatus();      
       }
    break;
  } 
  

  if(LOOP5_HAS_CHANGED && (!BUTTON1) && (!BUTTON2)) printSelectionStatus();  
  if(LOOP6_HAS_CHANGED  && (!BUTTON2) && (!BUTTON3)) printSelectionStatus();    
  if(LOOP7_HAS_CHANGED && (!BUTTON3) && (!BUTTON4)) printSelectionStatus();  
  if(LOOP8_HAS_CHANGED && (!BUTTON4) && (!BU)) printSelectionStatus();  
}

void doublePressEditAndLoopModeSelection(){//on down
  
   //Select other loops 5,6,7,8 - 5 from Button 1&2,6 from 2&3, 8 from 4&BNK_U
     
   if(BUTTON1 & BUTTON2){//Preset 5 selected?
   LOOP5 = !LOOP5; LOOP5_HAS_CHANGED = true;
   }else LOOP5_HAS_CHANGED = false;
   
   if(BUTTON2 & BUTTON3){//Preset 6 selected?
   LOOP6 = !LOOP6; LOOP6_HAS_CHANGED = true;
   
   }else LOOP6_HAS_CHANGED = false;
   
   if(BUTTON3 & BUTTON4){//Preset 7 selected?
   LOOP7 = !LOOP7; LOOP7_HAS_CHANGED = true;
   }else LOOP7_HAS_CHANGED = false;
   
   if(BUTTON4 & BU){//Preset 8 selected?
   LOOP8 = !LOOP8; LOOP8_HAS_CHANGED = true;
   }else LOOP8_HAS_CHANGED = false;
}
/* --------------Loop Mode Selection -------------*/

void loopModeSelection(int pin){
  
    
    switch(pin){
    case P1B:
      if(!LOOP5_HAS_CHANGED){
        LOOP1 = !LOOP1; //Toggle state off is bypass.
        printSelectionStatus();

      }

    break;
    case P2B:
      if((!LOOP5_HAS_CHANGED) && (!LOOP6_HAS_CHANGED)){
        LOOP2 = !LOOP2;
        printSelectionStatus();
      }    
    break;
    case P3B:
      if((!LOOP6_HAS_CHANGED) && (!LOOP7_HAS_CHANGED)){
        LOOP3 = !LOOP3;
        printSelectionStatus();
      }
    break;
    case P4B:
    if((!LOOP7_HAS_CHANGED) && (!LOOP8_HAS_CHANGED)){
      LOOP4 = !LOOP4;
      printSelectionStatus();

    }
    break;
  }
  //This way printing is ensured to happen only once since this may be triggered by sucesive up states.
  if(LOOP5_HAS_CHANGED && (!BUTTON1) && (!BUTTON2)) printSelectionStatus();  
  if(LOOP6_HAS_CHANGED && (!BUTTON2) && (!BUTTON3)) printSelectionStatus();    
  if(LOOP7_HAS_CHANGED && (!BUTTON3) && (!BUTTON4)) printSelectionStatus();  
  if(LOOP8_HAS_CHANGED && (!BUTTON4) && (!BU)) printSelectionStatus();  
  
  
}

void buttonLogic(int pin,boolean setValue){
  switch(pin){
   case BNK_U:
     BU = setValue;
   break; 
   case BNK_D:
     BD = setValue;
   break; 
   case P1B:
     BUTTON1 = setValue;
   break; 
   case P2B:
     BUTTON2 = setValue;
   break; 
   case P3B:
     BUTTON3 = setValue;
   break; 
   case P4B:
     BUTTON4 = setValue;
   break; 
  }
}

void bypassAllLoops(){

      LOOP1=LOOP2=LOOP3=LOOP4=LOOP5=LOOP6=LOOP7=LOOP8=false;    

}
