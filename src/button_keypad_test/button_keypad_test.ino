#include <Keypad.h>
#include <Joystick.h>

//DEFINITIONS
#define ENABLE_PULLUPS
#define INDEX_HOLD_BUTTONS 1 //replace "?"with the index of buttons hold = (NUMBUTTONS - 1)
#define NUMBUTTONS 2 //replace "?"with number of buttons you are using
#define NUMROWS 1 //replace "?" with number of rows you have
#define NUMCOLS 1 //replace "?" with number of columns you have

// constants won't change. They're used here to set pin numbers:
const int buttonPin = 5;     // the number of the pushbutton pin NODEMCU = D1 connect the other pin to GND
//Pin 15 is D8 in NODEMCU
//Pin 14 is D5 in NODEMCU

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

//BUTTON MATRIX
//first change number of rows and columns to match your button matrix, 
//then replace all "?" with numbers (starting from 0)
byte buttons[NUMROWS][NUMCOLS] = {
  {0}
};

//BUTTON MATRIX PART 2
byte rowPins[NUMROWS] = {15}; //change "?" to the pins the rows of your button matrix are connected to
byte colPins[NUMCOLS] = {14}; //change "?" to the pins the rows of your button matrix are connected to

Keypad buttbx = Keypad( makeKeymap(buttons), rowPins, colPins, NUMROWS, NUMCOLS);

//JOYSTICK SETTINGS
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,
  JOYSTICK_TYPE_JOYSTICK,
  NUMBUTTONS, //number of buttons
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

void setup() {
//  Serial.begin(9600);
  Joystick.begin();
  buttbx.setHoldTime(1000);
  // initialize the pushbutton pin as an input:
//  pinMode(buttonPin,INPUT_PULLUP);
//  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  CheckAllButtons();
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
                              Serial.print((uint8_t) buttbx.key[i].kchar);
                              Serial.print(" Button PRESSED \n");
                              Joystick.setButton(buttbx.key[i].kchar, 1);
                              break;
                    case HOLD:
                              Serial.print((uint8_t) buttbx.key[i].kchar + INDEX_HOLD_BUTTONS);
                              Serial.print("Button HOLD \n");
                              Joystick.setButton(buttbx.key[i].kchar + INDEX_HOLD_BUTTONS, 1);
                              break;
                    case RELEASED:
                              Serial.print((uint8_t) buttbx.key[i].kchar + INDEX_HOLD_BUTTONS);
                              Serial.print("Button RELEASED \n");
                              Joystick.setButton(buttbx.key[i].kchar + INDEX_HOLD_BUTTONS, 0);
                              break;
                    case IDLE:
                              Serial.print((uint8_t) buttbx.key[i].kchar);
                              Serial.print("Button IDLE \n");
                              Joystick.setButton(buttbx.key[i].kchar, 0);
                              break;
            }
           }   
         }
     }
}
