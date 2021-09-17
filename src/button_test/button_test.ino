
// constants won't change. They're used here to set pin numbers:
const int buttonPin = 5;     // the number of the pushbutton pin NODEMCU = D1 connect the other pin to GND

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
  Serial.begin(9600);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin,INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  //read the pushbutton value into a variable
  int sensorVal = digitalRead(buttonPin);
  //print out the value of the pushbutton
  // Keep in mind the pull-up means the pushbutton's logic is inverted. It goes
  // HIGH when it's open, and LOW when it's pressed. Turn on pin 13 when the
  // button's pressed, and off when it's not:
  if (sensorVal == HIGH) {
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println("ON");
    
  } else {
     digitalWrite(LED_BUILTIN, HIGH);
     Serial.println("OFF");
  }
  Serial.println("--------" + sensorVal);
}
