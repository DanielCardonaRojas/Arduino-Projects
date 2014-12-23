
/* 
Super8 stompbox relay switcher Sketch by Daniel Cardona Rojas 2014

  -------------------------------------- Features --------------------------------------------
-Write presets to EEPROM in banks of 8 presets: 1rst bank [1 8] 2nd bank [9 16] etc.
-Have three state program: preset mode, loop mode and edit mode (Choose a mecanism to enter each state)
-Has to implement 2 button press and hold event for saving to impair presets
-Use analog pins 0-5 as digital footswitch inputs. Have a LED status current activated preset.
-Have Led status of current loops activated in preset.
-Only one preset must be on at a time so if on is selected others have to be negated
-store a preset in a by using bitwise operators
-Preset 5,6,7,8 doing doble press on (1,2),(2,3),(3,4),(4,ST)....
-BnkUp and BnkDown simultaneous press changes mode.
-Blink bank number until preset is selected i.e dont change preset even though bank has changed.
- Blink dot when preset is saved.
-Shows momentary (1 sec) letters L,P,E on the 7 segment display to indicate the mode.
-Use shift registers for both outputing to relays and to change the 7 segment LED bank number.

  -------------------------------- Missing Features ----------------------------------
 
 - Mute output. If no preset or loop is selected this defaults to bypass all loops. 
  It could be acomplished using a tuner after the Super8 switcher.
 - Output exclusive for tuner. 
 - Buffered input. 
 
 ------------------------------------- Notes -----------------------------------------
 - Unedited presets default to all loops on. Because EEPROM comes with 255 written in by deafult.



 ------------------------------- Basic Layout of buttons -----------------------------

                          BnkUp ^ -
  P1 --- P2 --- P3 -- P4          I Mode
                          BnkDn v -
                          
*/
//#include "declarations.h"
#include <ButtonEvent.h>
#include <EEPROM.h>
#include <SimpleTimer.h>

/*--------Preset, Loop pins and mode Pins */
//Pins 1 through 9 are for loop relays 
#define P1B 14
#define P2B 15
#define P3B 16
#define P4B 17
#define BNK_U 18
#define BNK_D 19
#define L1 5
#define L2 4
#define L3 3
#define L4 2
#define PRESET_PIN 2
#define EDIT_PIN 3
#define LOOP_PIN 4
/*----------------- Operation Modes ------------------ */
#define EDIT_MODE 0
#define PRESET_MODE 1
#define LOOP_MODE 2
#define NUM_LOOPS 8
/*---------------Shift Registers ----------------*/

//7 segment display shift register note: these are not fisical pins
int clockPin = 12;
int dataPin = 11;
int latchPin = 8;
//Relay shift register
int relayDataPin = 7;
int relayLatchPin = 6;

/*----------------- Logic ------------------ */

//Toggle Logic - Make toggles from momentary switches ST is save toggle
boolean LOOP1 = false,LOOP2=false,LOOP3=false,LOOP4=false, ST = false; 
boolean LOOP5 = false,LOOP6=false,LOOP7=false,LOOP8=false;
boolean LOOP5_HAS_CHANGED=true,LOOP6_HAS_CHANGED=true,LOOP7_HAS_CHANGED=true,LOOP8_HAS_CHANGED=true;
//Button Logic - Used for multi button press 
boolean BUTTON1 =false, BUTTON2=false, BUTTON3=false, BUTTON4=false; 
boolean BU=false,BD=false;
int MODE = PRESET_MODE; 
boolean MODE_HAS_CHANGED=false;
/* ----------------- Presets and Banks --------------------*/
int PRESET_WRITE_ADDRESS,PRESET_READ_ADDRESS = 1;
byte PRESET_BITMASK = B00000000; 
int BANK_IDX = 0;
int PREV_BANK = 0;
#define BANK_MAX 4
boolean hasBankReturned = false;
boolean hasPresetBeenSaved = false;
boolean hasPresetBeenSelected = false;
boolean hasPresetBeenToggled = false;
boolean hasAltPresetBeenToggled = false;//Refering to presets 5,6,7,8
//numbering LEDs starting from the top (1) in clockwise direction 7nth segment is middle.
//Use ~ to negate bits in case display is common anode or cathode
byte BANK_DISPLAY[5]={6,91,79,102,0}; 
byte MODE_DISPLAY[3]={121,115,56};//E,P,L

/*--------------Function Prototypes ----------------- */
void editModeSelection(int pin);
void presetModeSelection(int pin);
void bitmaskWrite();
void bitmaskRead();
void displayMode();
void buttonLogic(int pin,boolean setValue);
/*------------------Timers ------------------------*/
SimpleTimer timer;
int timerId;
int timerId2;

/* -------------------- Setup ----------------------*/
void setup() {
  int i;
  for(i=1;i< 9;i++){
  pinMode(i,OUTPUT);
  }
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(latchPin, OUTPUT);

  ButtonEvent.initialCapacity = sizeof(ButtonInformation)*6;
  ButtonEvent.addButton(P1B, onDown,onUp, onHold, 1500, onDouble, 70);   
  ButtonEvent.addButton(P2B, onDown, onUp, onHold, 1500, onDouble, 70); 
  ButtonEvent.addButton(P3B, onDown, onUp, onHold, 1500, onDouble, 70); 
  ButtonEvent.addButton(P4B, onDown, onUp, onHold, 1500, onDouble, 70); 
  ButtonEvent.addButton(BNK_U, onDown, onUp, onHold, 1500, onDouble, 70); 
  ButtonEvent.addButton(BNK_D, onDown, onUp, onHold, 1500, onDouble, 70); 
  timerId = timer.setInterval(500,displayMode);
  timerId2 = timer.setInterval(500,displayBank);
  timer.disable(timerId);
  
  Serial.begin(9600);
  
}

/* -------------------- Loop -----------------------*/
void loop() {
  ButtonEvent.loop();
  timer.run();


  
}

/* --------   Button Event Callbacks --------*/
void onDown(ButtonInformation* Sender) {
 
  // Serial.print("On Down with sender: ");Serial.println(Sender->pin);
  buttonLogic(Sender->pin,true);
  
  //Change Mode Pressing Bank Up and Down Buttons at same time
  //Disable Bank Up and Down when both are pressed  
  modeChange();
  switch (MODE){
   case PRESET_MODE:
     doublePressPresetModeSelection();
   break;
   
   case EDIT_MODE:
   case LOOP_MODE:
       doublePressEditAndLoopModeSelection();

   break;
   
    
  }
    
}

void onUp(ButtonInformation* Sender) {
  


     //Bank Up or Down - Change Bank first checking that MODE hasn't changed
     //If Bank has changed retrigger preset
      bankChange();
      buttonLogic(Sender->pin,false);//not shure if this should before or after the switch

//----------- Selection modes --------------
  boolean isAltPresetCurrentlySelected = LOOP5_HAS_CHANGED || LOOP6_HAS_CHANGED || LOOP7_HAS_CHANGED || LOOP8_HAS_CHANGED;
   
   switch (MODE){
     case PRESET_MODE: 
           presetModeSelection(Sender->pin);
     break;
     case EDIT_MODE:
           if(!hasPresetBeenSaved){
             //Serial.println("No preset saved");
             editModeSelection(Sender->pin); 
           }else {
           }
     break;
     case LOOP_MODE:
           loopModeSelection(Sender->pin);
          shiftOutLoopModeSelection(); 
     break;       
   }
 
   hasPresetBeenSaved = false;
  
      


}

void onHold(ButtonInformation* Sender) {//Save a preset if on edit mode
  //Serial.print("On hold with sender: ");Serial.println(Sender->pin);
  if(MODE == EDIT_MODE){
    
    setWritePresetAddress(Sender->pin);
    //If preset 5,6,7 or 8 are long pressed dont count in saved selection    

    doublePressEditAndLoopModeSelection();
    hasPresetBeenSaved = true;
    
    // How to garantee bitmaskwrite onley gets called once? when double press and hold
    if(hasElapsedTimeInterval(100)){
          bitmaskWrite(); 

    }

    //Clear selection for a new preset to be saved
    clearLoopSelection();
    }

}

void onDouble(ButtonInformation* Sender) {

}

