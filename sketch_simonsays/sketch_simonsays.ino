
int RLED = 9;         //    Red LED connected to digital pin 9
int GLED = 8;         //  Green LED connected to digital pin 8
int BLED = 7;         //   Blue LED connected to digital pin 7
int YLED = 6;         // Yellow LED connected to digital pin 6
int ON_DELAY  = 1000; // Time that a LED is turned on [ms]
int OFF_DELAY = 0;    // Time that a LED is turned on [ms]

void setup() {
  // put your setup code here, to run once:

}

void loop() {

    analogWrite(RLED,255);         
    delay(ON_DELAY);            
    analogWrite(RLED,0);         
    
    analogWrite(GLED,255);         
    delay(ON_DELAY);            
    analogWrite(GLED,0);         
    
    analogWrite(BLED,255);         
    delay(ON_DELAY);            
    analogWrite(BLED,0);         
    
    analogWrite(YLED,255);           
    delay(ON_DELAY);            
    analogWrite(YLED,0);         
     
}

void turnOn(led) {
  analogWrite(led,255);
  delay(ON_DELAY);
}

void turnOff(led) {
  analogWrite(led,0);
  delay(OFF_DELAY);
}

