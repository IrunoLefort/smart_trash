#include <Servo.h>
Servo servomotor;
// external sensor
int const trig1 = 13;
int const echo1 = 12;
// internal sensor
int const trig2 = 9;
int const echo2 = 8;
// motor
int const motor = 11;
//led
int const green = 10;  // indicates that the garbage is open
int const red = 7;     // indicates that the garbage is full

int servoPosition = 0;
bool is_open = false;
unsigned long open_time = 0;
const unsigned long stay_open = 5000; // 5 secondes


void setup () {
   
   pinMode(trig1, OUTPUT);
   pinMode(echo1, INPUT);
   pinMode(trig2, OUTPUT);
   pinMode(echo2, INPUT);
   pinMode(red, OUTPUT);
   pinMode(green, OUTPUT);
   servomotor.attach(motor);
   servomotor.write(93);
   Serial.begin(9600);
   delay(500);
}

void loop() {
   long distance1 = getDistance(trig1, echo1);
   long distance2 = getDistance(trig2, echo2);
   // visualization
   Serial.print("Distance externe: ");
   Serial.println(distance1);
   Serial.print("Distance interne: ");
   Serial.println(distance2);
   // opening the garbage if someone is near (<=25cm)
   if(distance1 <= 25 && !is_open){
      opening();
      is_open = true;
      open_time = millis(); // store the moment
      digitalWrite(green, HIGH);
      digitalWrite(red, LOW);
   } 
   //closing the garbage after some time 
   if (is_open && millis() - open_time >= stay_open) {
      closing();
      is_open = false;
      digitalWrite(green, LOW);
    }
   // garbage full
    if (distance2 <= 5) {
      digitalWrite(red, HIGH);
    }
    else digitalWrite(red, LOW);
    delay(200);
}


long getDistance(int trig, int echo) {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  long duration = pulseIn(echo, HIGH);

  long distance = (duration / 2) * 0.0344;
  
  return distance;
}


void opening(){
for (int angle = 90; angle <= 180; angle++){
    servomotor.write(angle); 
    delay(1);  
    }
}

void closing(){
for (int angle = 180; angle >= 93; angle--) {
    servomotor.write(angle);  
    delay(1);  
   }
}