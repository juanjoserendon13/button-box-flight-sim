#include <Keypad.h>
#include <Joystick.h>

// MAKE THIS BUTTON BOX | 32 FUNCTION w ENCODERS (https://www.youtube.com/watch?v=Z7Sc4MJ8RPM) code by AMSTUDIO (https://www.youtube.com/channel/UCQS1ZB3BVSrBo3tCs7PyfxQ) is licensed under CC BY-NC-ND 4.0 (https://creativecommons.org/licenses/by-nc-nd/4.0/) 
// / This work "wim_s_button_box_code" (https://drive.google.com/file/d/1dKqA0WRBKmo2w-OuqffqRrVV8e1hs0sM/view) is a derivative of Wim (https://www.youtube.com/channel/UC5tI3LDyFwZEkISuJlUAi4Q)
// / This work "Robust Rotary encoder reading" (https://www.best-microcontroller-projects.com/rotary-encoder.html) is a derivative of John Main (best-microcontroller-projects.com) and is licensed under Copyright
// / Desaturated from original

//LIST_MAX has a default number of simultaneous buttons pressed

//DEFINITIONS
#define ENABLE_PULLUPS
#define NUMROTARIES ? //replace "?" with number of rotary encoders you are using
#define NUMBUTTONS ? //replace "?"with number of buttong you are using
#define NUMROWS ? //replace "?" with number of rows you have
#define NUMCOLS ? //replace "?" with number of columns you have

//BUTTON MATRIX
//first change number of rows and columns to match your button matrix, 
//then replace all "?" with numbers (starting from 0)
byte buttons[NUMROWS][NUMCOLS] = {
  {?,?,?,?},
  {?,?,?,?},
  {?,?,?,?},
  {?,?,?,?}
};

struct rotariesdef {
  byte pin1;
  byte pin2;
  int ccwchar;
  int cwchar;
  volatile unsigned char state;
};

//ROTARY ENCODERS
//each line controls a different rotary encoder
//the first two numbers refer to the pins the encoder is connected to 
//the second two are the buttons each click of the encoder wil press 
//do NOT exceed 31 for the final button number
rotariesdef rotaries[NUMROTARIES] {
  {0,1,22,23,0}, //rotary 1
  {2,3,24,25,0}, //rotary 2
  {4,5,26,27,0}, //rotary 3
  {6,7,28,29,0} //rotary 4


};

#define DIR_CCW 0x10
#define DIR_CW 0x20
#define R_START 0x0

#ifdef HALF_STEP
#define R_CCW_BEGIN 0x1
#define R_CW_BEGIN 0x2
#define R_START_M 0x3
#define R_CW_BEGIN_M 0x4
#define R_CCW_BEGIN_M 0x5
const unsigned char ttable[6][4] = {
  // R_START (00)
  {R_START_M,            R_CW_BEGIN,     R_CCW_BEGIN,  R_START},
  // R_CCW_BEGIN
  {R_START_M | DIR_CCW, R_START,        R_CCW_BEGIN,  R_START},
  // R_CW_BEGIN
  {R_START_M | DIR_CW,  R_CW_BEGIN,     R_START,      R_START},
  // R_START_M (11)
  {R_START_M,            R_CCW_BEGIN_M,  R_CW_BEGIN_M, R_START},
  // R_CW_BEGIN_M
  {R_START_M,            R_START_M,      R_CW_BEGIN_M, R_START | DIR_CW},
  // R_CCW_BEGIN_M
  {R_START_M,            R_CCW_BEGIN_M,  R_START_M,    R_START | DIR_CCW},
};
#else
#define R_CW_FINAL 0x1
#define R_CW_BEGIN 0x2
#define R_CW_NEXT 0x3
#define R_CCW_BEGIN 0x4
#define R_CCW_FINAL 0x5
#define R_CCW_NEXT 0x6

const unsigned char ttable[7][4] = {
  // R_START
  {R_START,    R_CW_BEGIN,  R_CCW_BEGIN, R_START},
  // R_CW_FINAL
  {R_CW_NEXT,  R_START,     R_CW_FINAL,  R_START | DIR_CW},
  // R_CW_BEGIN
  {R_CW_NEXT,  R_CW_BEGIN,  R_START,     R_START},
  // R_CW_NEXT
  {R_CW_NEXT,  R_CW_BEGIN,  R_CW_FINAL,  R_START},
  // R_CCW_BEGIN
  {R_CCW_NEXT, R_START,     R_CCW_BEGIN, R_START},
  // R_CCW_FINAL
  {R_CCW_NEXT, R_CCW_FINAL, R_START,     R_START | DIR_CCW},
  // R_CCW_NEXT
  {R_CCW_NEXT, R_CCW_FINAL, R_CCW_BEGIN, R_START},
};
#endif

//BUTTON MATRIX PART 2
byte rowPins[NUMROWS] = {?,?,?,?}; //change "?" to the pins the rows of your button matrix are connected to
byte colPins[NUMCOLS] = {?,?,?,?}; //change "?" to the pins the rows of your button matrix are connected to

Keypad buttbx = Keypad( makeKeymap(buttons), rowPins, colPins, NUMROWS, NUMCOLS);

//JOYSTICK SETTINGS
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,
  JOYSTICK_TYPE_JOYSTICK,
  32, //number of buttons
  0, //number of hat switches
  //Set as many axis to "true" as you have potentiometers for
  false, // y axis
  false, // x axis
  false, // z axis
  false, // rx axis
  false, // ry axis
  false, // rz axis
  false, // rudder
  false, // throttle
  false, // accelerator
  false, // brake
  false); // steering wheel

const int numReadings = 20;
 
int readings[numReadings];      // the readings from the analog input
int index = 0;              // the index of the current reading
int total = 0;                  // the running total
int currentOutputLevel = 0;

//POTENTIOMETERS PART 1
//add all the axis' which are enabled above
int zAxis_ = 0;
int RxAxis_ = 0;   

//POTENTIOMETERS  PART 2
//Which pins are your potentiometers connected to?
int potentiometerPin1 = ?; //Change "?" to the pin your potentiometer is connected to
int potentiometerPin2 = ?;
const bool initAutoSendState = true;

void setup() {
  Joystick.begin();
  rotary_init();
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
}

void loop() {

  CheckAllEncoders();
  CheckAllButtons();
  CheckAllPotentiometers();
 
}

//POTENTIOMETERS PART 3
//change the details to match the details above for each potentiometer you are using
void CheckAllPotentiometers(){
                           
  //potentiometer 1
  currentOutputLevel = getAverageOutput(potentiometerPin1);
  zAxis_ = map(currentOutputLevel,0,1023,0,255);
  Joystick.setZAxis(zAxis_); 

  //potentiometer 2
  currentOutputLevel = getAverageOutput(potentiometerPin2);
  RxAxis_ = map(currentOutputLevel,0,1023,0,255);
  Joystick.setRxAxis(RxAxis_);


}

int getAverageOutput(int pinToRead){
  index = 0;
  total = 0; 
 
  while (index < numReadings){
    readings[index] = analogRead(pinToRead);
    total = total + readings[index];
    index = index + 1;
    //delay (1);
  }
  return total / numReadings;
}

void CheckAllButtons(void) {
      if (buttbx.getKeys())
    {
       for (int i=0; i<LIST_MAX; i++)   
        {
           if ( buttbx.key[i].stateChanged )   
            {
            switch (buttbx.key[i].kstate) { 
                    case PRESSED:
                    case HOLD:
                              Joystick.setButton(buttbx.key[i].kchar, 1);
                              break;
                    case RELEASED:
                    case IDLE:
                              Joystick.setButton(buttbx.key[i].kchar, 0);
                              break;
            }
           }   
         }
     }
}

void rotary_init() {
  for (int i=0;i<NUMROTARIES;i++) {
    pinMode(rotaries[i].pin1, INPUT);
    pinMode(rotaries[i].pin2, INPUT);
    #ifdef ENABLE_PULLUPS
      digitalWrite(rotaries[i].pin1, HIGH);
      digitalWrite(rotaries[i].pin2, HIGH);
    #endif
  }
}

unsigned char rotary_process(int _i) {
  //Serial.print("Processing rotary: ");
  //Serial.println(_i);
  unsigned char pinstate = (digitalRead(rotaries[_i].pin2) << 1) | digitalRead(rotaries[_i].pin1);
  rotaries[_i].state = ttable[rotaries[_i].state & 0xf][pinstate];
  return (rotaries[_i].state & 0x30);
}

void CheckAllEncoders(void) {
  Serial.println("Checking rotaries");
  for (int i=0;i<NUMROTARIES;i++) {
    unsigned char result = rotary_process(i);
    if (result == DIR_CCW) {
      Serial.print("Rotary ");
      Serial.print(i);
      Serial.println(" <<< Going CCW");
      Joystick.setButton(rotaries[i].ccwchar, 1); delay(50); Joystick.setButton(rotaries[i].ccwchar, 0);
    };
    if (result == DIR_CW) {
      Serial.print("Rotary ");
      Serial.print(i);
      Serial.println(" >>> Going CW");
      Joystick.setButton(rotaries[i].cwchar, 1); delay(50); Joystick.setButton(rotaries[i].cwchar, 0);
    };
  }
  Serial.println("Done checking");
}
