void turnOn(int);
void turnOff(int);
void turnOffAll(int);
void flashOK();
void flashWrong();
void clearKeyedSequence();
void testSequence(int lvl);

#define DEBUG 0

// From tutorial: https://create.arduino.cc/projecthub/ronbentley1/button-switch-using-an-external-interrupt-7879df
#define switched                            true // value if the button switch has been pressed
#define debounce                              10 // time to wait in milli secs

// Set up LEDs and Buttons on pins
#define RLED 9         //    Red LED connected to digital pin 9
#define GLED 8         //  Green LED connected to digital pin 8
#define BLED 7         //   Blue LED connected to digital pin 7
#define YLED 6         // Yellow LED connected to digital pin 6
#define RBUT 12
#define GBUT 11
#define BBUT 10
#define YBUT 5
const int nPins = 4;
int RGBYPins[] = {RLED,GLED,BLED,YLED};
int ButtonPins[] = {RBUT,GBUT,BBUT,YBUT};

int ON_DELAY  = 300;    // Time that a LED is turned on [ms]
int OFF_DELAY = 100;    // Time between LED flashes [ms]

const int MAX_LEVEL = 10;  
int rndSeq[MAX_LEVEL];
int keySeq[MAX_LEVEL];
int key_index;                 // Current key sequence index

void setup() {
  // Set up pin states
  for (int i = 0; i < nPins; i++) {
    pinMode(RGBYPins[i], OUTPUT);
    pinMode(ButtonPins[i], INPUT);
    // digitalWrite(ButtonPins[i], HIGH);
  }

  // Game setup
  randomSeed(analogRead(0));
  rndSeq[MAX_LEVEL];
  for (int i = 0; i < MAX_LEVEL; i++)
    rndSeq[i] = random(0,nPins);
#ifdef DEBUG  
  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
#endif

  flashStart();
}

// Adapted from tutorial: https://create.arduino.cc/projecthub/ronbentley1/button-switch-using-an-external-interrupt-7879df
bool read_button(int button_switch) {
  // static variables because we need to retain old values between function calls
  static bool     switch_pending[nPins] = {false,false,false,false};
  static long int elapse_timer[nPins];

  // Use an input outside the generic bounds to reset the pending attributes of all switches
  if (button_switch >= nPins)
  {
    for (int i = 0; i < nPins; i++) {
      switch_pending[i] = false;
      elapse_timer[i] = 0;
    }
    return !switched;
  }

  int button_reading = digitalRead(ButtonPins[button_switch]);
  // Check if button is pressed, and if this is observed for the first time start 
  // the debounce timer (to absorb any spurious fluctuations)

  if ((button_reading == HIGH) && (!switch_pending[button_switch])) {
      // High signal so presumed button press - set the pending + debounce cycle
      switch_pending[button_switch] = true;
      elapse_timer[button_switch] = millis(); // start elapse timing for debounce checking
#ifdef DEBUG  
      Serial.print("Input pin values 0-3: ");
      for (int i = 0; i < nPins; i++) {
        Serial.print(digitalRead(ButtonPins[i]));
        Serial.print("  ");
      }
      Serial.print(" for button: ");
      Serial.println(button_switch);
#endif
    }


  // Switch was pressed, now released, so check if debounce time elapsed. If this check is not
  // met, it is likely that the HIGH signal detected earlier was a spurious potential spike. Either
  // way, we release the pending attribute for this switch.
  if (switch_pending[button_switch] && button_reading == LOW) {
    switch_pending[button_switch] = false;   // reset for next button press
    if (millis() - elapse_timer[button_switch] >= debounce) {
      // debounce time elapsed, so switch press cycle complete
#ifdef DEBUG  
      Serial.print("Button read: ");
      Serial.println(button_switch);
#endif
      keySeq[key_index++] = button_switch;           // Add keypress to keyed sequence
      return switched;                       // advise that switch has been pressed
    }
  }
  return !switched; // either no press request or debounce period not elapsed
} // end of read_button function

void loop() {
  bool isCorrectSequence = true;
  int level = 1;
  // Game logic
  while ((level < MAX_LEVEL) && (isCorrectSequence)) {
#ifdef DEBUG  
      Serial.print("Level: ");
      Serial.println(level);
#endif
      showSequence(level);
      turnOffAll();

      getKeySequence(level);
          
      isCorrectSequence = testKeySequence(level);
      if (isCorrectSequence)
        flashOK();
      else
        flashWrong();

      // Level up!
      delay(1000);
      level++;
  }
}

bool testKeySequence(int lvl) {
  bool isCorrectSequence = true;
  Serial.print("Key sequence recorded vs rndsequence: ");
  for (int i = 0; i < lvl; i++) {
    isCorrectSequence = (isCorrectSequence && (keySeq[i] == rndSeq[i]));
  }

  return isCorrectSequence;
}

void getKeySequence(int lvl)
// Gets the keyed sequence and tests it against the goal sequence
// Reports back a bool whether the key presses match the goal.
{
  int i;
#ifdef DEBUG  
  Serial.print("In getKeySequence()... ");
  Serial.println(lvl);
#endif
  clearKeyedSequence();

  // Loop over all input pins to detect button presses. Exit if enough key presses have been collected
  while (key_index < lvl) {
    for (i = 0; i < nPins; i++)
     read_button(i);

    delay(10);
  }
}

void clearKeyedSequence () {
  // Reset keyed sequence to nPins (impossible number)
  for (int i = 0; i < MAX_LEVEL; i++)
    keySeq[i] = nPins;
  
  // Reset key sequence index
  key_index = 0;

  // Reset the read_button counters in case one button was still active
  read_button(nPins+1);
}

void showSequence(int lvl) {
  for (int i=0; i < lvl; i++) {
    turnOn(rndSeq[i]);
    turnOff(rndSeq[i]);
  }
}

void flashOK() {
  delay(300);
  digitalWrite(GLED,HIGH);
  delay(2000);
  digitalWrite(GLED,LOW);
  delay(300);
}

void flashWrong() {
  delay(300);
  digitalWrite(RLED,HIGH);
  delay(2000);
  digitalWrite(RLED,LOW);
  delay(300);
}

void turnOn(int led) {
  analogWrite(RGBYPins[led],255);
  delay(ON_DELAY);
}

void turnOff(int led) {
  analogWrite(RGBYPins[led],0);
  delay(OFF_DELAY);
}

void turnOffAll() {
  for (int i = 0; i < nPins; i++)
    analogWrite(RGBYPins[i],0);

  delay(OFF_DELAY);
}

void flashStart()
{
    for (int j = 0; j < nPins; j++) {
      digitalWrite(RGBYPins[j],HIGH);
      delay(50);
    }
    delay(500);
 
    for (int j = 0; j < nPins; j++) {
      digitalWrite(RGBYPins[j],LOW);
      delay(50);
    }

  turnOffAll();
}