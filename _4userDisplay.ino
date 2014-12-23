/* ---------------- Display Current Bank ----------------------

Blink bank only if preset mode is selected and a preset in the current bank hasnt been selected i.e

Blink dot led several times if a preset has been saved in edit mode
*/
void displayBank(){
  
   static int blinkCount=0;
   byte bitmask = ~(BANK_DISPLAY[BANK_IDX]);
   static byte shiftVal = bitmask;

   
   
   if(MODE == PRESET_MODE ){
        
        if(!hasBankReturned){ //if preset hasnt returned to where preset was selected blink 
            shiftVal = (shiftVal == 255) ? bitmask : 255;
        }
         else shiftVal = bitmask;
      
           digitalWrite(latchPin, 0);
           shiftOut(dataPin,clockPin,MSBFIRST,shiftVal);
           digitalWrite(latchPin, 1);

     
   }else if(MODE == EDIT_MODE){
       
        if(hasPresetBeenSaved && blinkCount < 6  ) {
            shiftVal = shiftVal ^ (1 << 7);//toggle dot bit
            //shiftVal = (shiftVal == 254) ? bitmask : 254;
             blinkCount++;
             
           digitalWrite(latchPin, 0);
           shiftOut(dataPin,clockPin,MSBFIRST,shiftVal);
           digitalWrite(latchPin, 1);
        }
        else if(!hasPresetBeenSaved) { 
          shiftVal = bitmask; blinkCount =0;
           digitalWrite(latchPin, 0);
           shiftOut(dataPin,clockPin,MSBFIRST,bitmask);
           digitalWrite(latchPin, 1);
        }
        
     
   }else if(MODE == LOOP_MODE){
     
           shiftVal = ~(MODE_DISPLAY[LOOP_MODE]);//Show an L in Loop as there are no banks
           digitalWrite(latchPin, 0);
           shiftOut(dataPin,clockPin,MSBFIRST,shiftVal);
           digitalWrite(latchPin, 1); 
   }
  
 
}


//Blink Mode Letter - [L,P,E]

void displayMode(){
  if(!MODE_HAS_CHANGED) return;
  static int callCount = 0;
  callCount++;
  if(callCount > 2){
    timer.disable(timerId);
    timer.enable(timerId2);
    
  }
  
  Serial.println("Display mode callback number: ");Serial.println(callCount);
   if(MODE_HAS_CHANGED){
       
       byte bitmask = ~(MODE_DISPLAY[MODE]);
       byte shiftVal = bitmask;   
                 
       digitalWrite(latchPin, 0);
       shiftOut(dataPin,clockPin,MSBFIRST,shiftVal);
       digitalWrite(latchPin, 1);
       
   }else {
   }
   
   if(!timer.isEnabled(timerId)) callCount = 0;
 
  
}
//Blink selection LED when preset stored

void blinkLEDs(byte preset, int time){
  int k,elapsedTime=0;
  static int startTime;
  
  while(elapsedTime < time ){
       elapsedTime = millis()-startTime;

     //Turn All ON
     for(k=0;k< 5;k++){
        if(bitRead(preset,k) ) digitalWrite(k+1,HIGH); 
      }
      delay(300);
      
    //Turn All Off
    for(k=0;k< 5;k++){
        if(bitRead(preset,k)) digitalWrite(k+1,HIGH);
  
    }
    delay(300);
  }

}
