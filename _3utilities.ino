
/* ----------------- Utilities ----------------- */

void printSelectionStatus(){
  
  Serial.print("Selected Loops: ");
  Serial.print(LOOP1);
  Serial.print(LOOP2);
  Serial.print(LOOP3);
  Serial.print(LOOP4);
  Serial.print(LOOP5);
  Serial.print(LOOP6);
  Serial.print(LOOP7);
  Serial.println(LOOP8);
}

/*---------- Bitmask coding ------------*/

void bitmaskWrite(){
 
  byte preset = (LOOP8 << 0)|(LOOP7 << 1)|(LOOP6 << 2)|(LOOP5 << 3)|(LOOP1 << 7)|(LOOP2 << 6)|(LOOP3 << 5)|(LOOP4 << 4);

  EEPROM.write(PRESET_WRITE_ADDRESS,preset);
  Serial.print("Saved preset with bitmask: ");
  Serial.print(preset);
  Serial.println(" at address: ");Serial.println(PRESET_WRITE_ADDRESS);

}

void bitmaskRead(){//replace with bitRead()
  
  int k;//stompboxes = 8 
  PRESET_BITMASK = EEPROM.read(PRESET_READ_ADDRESS);
  Serial.print("Selected Preset: ");
  Serial.print(PRESET_BITMASK,BIN);
  Serial.print(" at address: ");Serial.println(PRESET_READ_ADDRESS);
  digitalWrite(relayLatchPin, 0);
  shiftOut(relayDataPin,clockPin,LSBFIRST,PRESET_BITMASK);
  digitalWrite(relayLatchPin, 1);
  
}

/*------------------- Mode display --------------------*/

//void displayMode(){
//
//  digitalWrite(PRESET_PIN,LOW);
//  digitalWrite(LOOP_PIN,LOW);
//  digitalWrite(EDIT_PIN,LOW);
//  switch(MODE){
//    case PRESET_MODE:
//     digitalWrite(PRESET_PIN,HIGH); 
//    break;
//    case EDIT_MODE:
//      digitalWrite(EDIT_PIN,HIGH);
//    break;
//    case LOOP_MODE:
//      digitalWrite(LOOP_PIN,HIGH);
//    break;    
//  }
//
//
//}


/*-------- Shift out loop selection in Loop mode ----------*/

void shiftOutLoopModeSelection(){
    byte loopSelection = (LOOP8 << 0)|(LOOP7 << 1)|(LOOP6 << 2)|(LOOP5 << 3)|(LOOP1 << 7)|(LOOP2 << 6)|(LOOP3 << 5)|(LOOP4 << 4);
   digitalWrite(relayLatchPin, 0);
   shiftOut(relayDataPin,clockPin,LSBFIRST,loopSelection);
   digitalWrite(relayLatchPin,1);
  
}
void clearLoopSelection(){
 LOOP1=LOOP2=LOOP3=LOOP4=LOOP5=LOOP6=LOOP7=LOOP8=false; 
}

/*----- Utility tha will time elpased time between function calls -------
skips a function call within a certain time interval returns a bool so it can be enbedded in an if statement
*/

boolean hasElapsedTimeInterval(int intervalInMillis) {
  
  static long previousMillis =0;
   unsigned long currentMillis = millis();
   //Notice on first call current millis is probably > than interval
   long elapsedTime = currentMillis - previousMillis; 
   if(elapsedTime > intervalInMillis){
          previousMillis = currentMillis;

     return true;
     
   }
   else{
     return false;
   }
  
}





