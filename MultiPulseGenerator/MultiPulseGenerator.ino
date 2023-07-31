/** MULTI PULSE GENERATOR
   reads the state of the input PIN and outputs several pulses if the input pin is HIGH.
   author: yuki.goya@riken.jp
   v23.07.31
 **/

// ******************* SETTINGS **********************************

// number of output pins
const static byte OUTPUT_COUNT = 4;

// input pin
const static int INPUT_PIN = 2;

// output pins
const static int OUTPUT_PINS[OUTPUT_COUNT] = { 8, 12, 13, 13 };

// output pulses frequency in Hz
const static float PULSE_FREQ[OUTPUT_COUNT] = { 20.0, 20.0, 20.0, 20.0 };

// output pulses width in microseconds
const static long PULSE_WIDTH[OUTPUT_COUNT] = { 20000, 20000, 16000, 16000 };

// output pulses offset in microseconds
const static long PULSE_OFFSET[OUTPUT_COUNT] = { 0, 25000, 2000, 27000 };

// output pulses initial delay in microseconds
const static long INITIAL_DELAY[OUTPUT_COUNT] = { 0, 0, 0, 0 };

// ***************************************************************

// pulse pre/high/post durations in microseconds
static unsigned long preDurations[OUTPUT_COUNT];
static unsigned long highDurations[OUTPUT_COUNT];
static unsigned long postDurations[OUTPUT_COUNT];

// timestamps in microseconds
static unsigned long previousTimes[OUTPUT_COUNT];

// current pin status
static byte pinStatus[OUTPUT_COUNT];

// ***************************************************************

// initial setup
void setup() {
  // set input pin
  pinMode(INPUT_PIN, INPUT);
  // set output pins and pulse durations
  for (byte i = 0; i < OUTPUT_COUNT; i++) {
    pinMode(OUTPUT_PINS[i], OUTPUT);
    // max period in microseconds
    double maxPulsePeriod = 1e6 / PULSE_FREQ[i];
    // set HIGH and LOW durations.
    // 50% duty cycle of maxPulsePeriod if requested HIGH duration is too high.
    highDurations[i] = (unsigned long)(PULSE_WIDTH[i] >= maxPulsePeriod) ? maxPulsePeriod / 2 : PULSE_WIDTH[i];
    // 0 OFFSET if requested value too high.
    preDurations[i] = (unsigned long)(PULSE_OFFSET[i] >= (maxPulsePeriod - highDurations[i])) ? 0 : PULSE_OFFSET[i];
    postDurations[i] = (unsigned long)(maxPulsePeriod - highDurations[i] - preDurations[i]);
    // initial status
    digitalWrite(OUTPUT_PINS[i], LOW);

    previousTimes[i] = 0;
    pinStatus[i] = 0;
  }
}

// Main loop
void loop() {
  unsigned long currentMicros = micros();

  // check input
  if (digitalRead(INPUT_PIN) == LOW)  // No input
  {
    // stop all ongoing outputs
    for (byte i = 0; i < OUTPUT_COUNT; i++) {
      if (pinStatus[i]) {
        digitalWrite(OUTPUT_PINS[i], LOW);
        pinStatus[i] = 0;
      }
      // update timestamp
      previousTimes[i] = currentMicros;
    }
  } else  // Input ON
  {
    for (byte i = 0; i < OUTPUT_COUNT; i++) {
      unsigned long dt = currentMicros - previousTimes[i];
      switch (pinStatus[i]) {
        case 0:
          if (dt > INITIAL_DELAY[i]) {
            pinStatus[i] = 1;  // INI done, move to PRE
            // update timestamp
            previousTimes[i] = currentMicros;
          }
          break;
        case 1:
          if (dt > preDurations[i]) {
            pinStatus[i] = 2;  // PRE done, move to HIGH
            digitalWrite(OUTPUT_PINS[i], HIGH);
            // update timestamp
            previousTimes[i] = currentMicros;
          }
          break;
        case 2:
          if (dt > highDurations[i]) {
            pinStatus[i] = 3;  // HIGH done, move to POST
            digitalWrite(OUTPUT_PINS[i], LOW);
            // update timestamp
            previousTimes[i] = currentMicros;
          }
          break;
        case 3:
          if (dt > postDurations[i]) {
            pinStatus[i] = 1;  // POST done, move to PRE
            // update timestamp
            previousTimes[i] = currentMicros;
          }
          break;
        default:  // ERRROR
          // cancel output
          digitalWrite(OUTPUT_PINS[i], LOW);
          pinStatus[i] = 0;
          // update timestamp
          previousTimes[i] = currentMicros;
      }
    }
  }
}

// eof
