#define CLK_PIN  2 // D4 in NODEMCU
#define DATA_PIN 0 // D3 in NODEMCU
#define YLED A1

////////////////////////////////////////////////////
void setup() {
   pinMode(CLK_PIN,INPUT);
   pinMode(DATA_PIN,INPUT);

   Serial.begin(115200);
  
}

////////////////////////////////////////////////////
void loop() {
//  Serial.println("Rotary Encoder");
  static uint16_t state=0,counter=0;

    delayMicroseconds(100); // Simulate doing somehing else as well.

    state=(state<<1) | digitalRead(CLK_PIN) | 0xe000;

    if (state==0xf000){
       state=0x0000;
       if(digitalRead(DATA_PIN))
         counter++;
       else
         counter--;
       Serial.println(counter);
    }
}
