void turnOn(int);
void turnOff(int);
void turnOffAll(int);

int RLED = 9;         //    Red LED connected to digital pin 9
int GLED = 8;         //  Green LED connected to digital pin 8
int BLED = 7;         //   Blue LED connected to digital pin 7
int YLED = 6;         // Yellow LED connected to digital pin 6
int ON_DELAY  = 1000; // Time that a LED is turned on [ms]
int OFF_DELAY = 1000;    // Time that a LED is turned on [ms]

void setup() {
  // put your setup code here, to run once:

}

void loop() {

    turnOn(RLED);
    turnOn(GLED);
    turnOn(BLED);
    turnOn(YLED);
    turnOffAll();
    
     
}

void turnOn(int led) {
  analogWrite(led,255);
  delay(ON_DELAY);
}

void turnOff(int led) {
  analogWrite(led,0);
  delay(OFF_DELAY);
}

void turnOffAll() {
  analogWrite(RLED,0);
  analogWrite(GLED,0);
  analogWrite(BLED,0);
  analogWrite(YLED,0);
  delay(OFF_DELAY);

}
