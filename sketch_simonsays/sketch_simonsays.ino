void turnOn(int);
void turnOff(int);
void turnOffAll(int);
void flashOK();
void flashWrong();
void clearKeyedSequence();
void testSequence(int lvl);

// From tutorial: https://create.arduino.cc/projecthub/ronbentley1/button-switch-using-an-external-interrupt-7879df
#define switched                            true // value if the button switch has been pressed
#define triggered                           true // controls interrupt handler
#define interrupt_trigger_type            RISING // interrupt triggered on a RISING input
#define debounce                              10 // time to wait in milli secs
volatile  bool interrupt_process_status = {
  !triggered                                     // start with no switch press pending, ie false (!triggered)
};
bool initialisation_complete =            false; // inhibit any interrupts until initialisation is complete

const int nPins = 4;
int RGBYPins[] = {9,8,7,6};
int ButtonPins[] = {12,11,10,5};
int RLED = 9;         //    Red LED connected to digital pin 9
int GLED = 8;         //  Green LED connected to digital pin 8
int BLED = 7;         //   Blue LED connected to digital pin 7
int YLED = 6;         // Yellow LED connected to digital pin 6
int RBUTTON = 12;
int GBUTTON = 11;
int BBUTTON = 10;
int YBUTTON = 5;
int ON_DELAY  = 400; // Time that a LED is turned on [ms]
int OFF_DELAY = 80;    // Time that a LED is turned on [ms]
const int MAX_LEVEL = 20;  
int level;
int rndSeq[MAX_LEVEL];
int keySeq[MAX_LEVEL];
int key_index;                 // Current key sequence index

void setup() {
  // Set up interrupt handlers and pin states
  // Able to can update the LED
  for (int i = 0; i < nPins; i++) {
    pinMode(RGBYPins[i], OUTPUT);
    pinMode(ButtonPins[i], INPUT);
  }
  // Internal pull-up resistor
  // pinMode(RBUTTON, INPUT);
  // attachInterrupt(digitalPinToInterrupt(RBUTTON),
  //                 redButtonPressed,
  //                 interrupt_trigger_type);

  // Game setup
  randomSeed(analogRead(0));
  rndSeq[MAX_LEVEL];
  for (int i = 0; i < MAX_LEVEL; i++)
    rndSeq[i] = random(0,nPins);
  level = 1;
  
  initialisation_complete = true; // open interrupt processing for business

  Serial.begin(9600); // opens serial port, sets data rate to 9600 bps
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
  }

  int button_reading = digitalRead(ButtonPins[button_switch]);
  // Check if button is pressed, and if this is observed for the first time start 
  // the debounce timer (to absorb any spurious fluctuations)
  if ((button_reading == HIGH) && (!switch_pending[button_switch])) {
      // High signal so presumed button press - set the pending + debounce cycle
      switch_pending[button_switch] = true;
      elapse_timer[button_switch] = millis(); // start elapse timing for debounce checking
    }

  // Switch was pressed, now released, so check if debounce time elapsed. If this check is not
  // met, it is likely that the HIGH signal detected earlier was a spurious potential spike. Either
  // way, we release the pending attribute for this switch.
  if (switch_pending[button_switch] && button_reading == LOW) {
    switch_pending[button_switch] = false;   // reset for next button press
    if (millis() - elapse_timer[button_switch] >= debounce) {
      // debounce time elapsed, so switch press cycle complete
      keySeq[key_index++] = button_switch;           // Add keypress to keyed sequence
      return switched;                       // advise that switch has been pressed
    }
  }
  return !switched; // either no press request or debounce period not elapsed
} // end of read_button function

void loop() {
  bool isCorrectSequence = true;
  // Game logic
  while ((level < MAX_LEVEL) && (isCorrectSequence)) {
      Serial.println(level);
      showSequence(level);
      
      delay(3000);
      turnOffAll();

      getKeySequence(level);
          
      isCorrectSequence = testKeySequence(level);
      if (isCorrectSequence)
        flashOK();
      else
        flashWrong();

          // // Level up!
          // delay(1000);
      level++;
  }
}

bool testKeySequence(int lvl) {
  bool isCorrectSequence;

  for (int i = 0; i < lvl; i++)
    isCorrectSequence == (isCorrectSequence && (keySeq[i] == rndSeq[i]));

  return isCorrectSequence;
}

void getKeySequence(int lvl)
// Gets the keyed sequence and tests it against the goal sequence
// Reports back a bool whether the key presses match the goal.
{
  int i;
  clearKeyedSequence();

  // Loop over all input pins to detect button presses. Exit if enough key presses have been collected
  while (key_index < lvl) {
    for (i = 0; i < nPins; i++)
     read_button(i);
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
  for (int i=0; i < level; i++) {
    turnOn(rndSeq[i]);
    turnOff(rndSeq[i]);
  }
}

void flashOK() {
  for (int i = 0; i < 20; i++) {
    turnOn(RGBYPins[1]);
    delay(50);
    turnOff(RGBYPins[1]);
  }
  delay(3000);
}

void flashWrong() {
  for (int i = 0; i < 10; i++) {
    turnOn(RGBYPins[0]);
    delay(150);
    turnOff(RGBYPins[0]);
  }
  delay(3000);
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
