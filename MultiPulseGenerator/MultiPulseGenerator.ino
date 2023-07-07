/** MULTI PULSE GENERATOR
 * reads the state of the input PIN and outputs several pulses if the input pin is HIGH.
 * author: yuki.goya@riken.jp 
 * v23.07.07
 **/

// ******************* SETTINGS **********************************

// input pin
const int INPUT_PIN= 2;
// output pins
const int OUTPUT_PINS[3]= {8, 9, 10};

// output pulses frequency in Hz 
const float PULSE_FREQ[3]= {20.0, 20.0, 40.0};

// output pulses width in microseconds
const long PULSE_WID[3]= {20000, 20000, 16000};

// output pulses offset in microseconds
const long PULSE_OFFSET[3]= {0, 0, 0};

// output pulses initial delays in microseconds
const long INITIAL_DELAY[3]= {0, 25000, 2000};

// ***************************************************************

// pulse pre/high/post durations in microseconds
unsigned long preDurations[3];
unsigned long highDurations[3];
unsigned long postDurations[3];

// timestamps in milliseconds
unsigned long previousTimes[3];
// current pin status
short pinStatus[3];

void setup()
{
  // set input pin
  pinMode(INPUT_PIN, INPUT);
  // set output pins and pulse durations
  for(short i=0;i<3;i++) 
  {
    pinMode(OUTPUT_PINS[i], OUTPUT);
    // max period in microseconds
    double maxPulsePeriod= 1e6/PULSE_FREQ[i];
    // set HIGH and LOW durations. 
    // 50% duty cycle of maxPulsePeriod if requested HIGH duration is too high.
    highDurations[i]= (unsigned long) (PULSE_WID[i]>=maxPulsePeriod)? maxPulsePeriod/2 : PULSE_WID[i];
    // 0 OFFSET if requested value too high.
    preDurations[i]= (unsigned long)(PULSE_OFFSET[i]>=(maxPulsePeriod - highDurations[i]))? 0: PULSE_OFFSET[i];
    postDurations[i]= (unsigned long) (maxPulsePeriod - highDurations[i] - preDurations[i]);
    // initial status
    digitalWrite(OUTPUT_PINS[i], LOW);
    previousTimes[i]= 0;
    pinStatus[i]= 0;
  }
}

void updatePIN(short idx)
{
  if(digitalRead(INPUT_PIN)==LOW) // DO NOTHING
  {
    if(pinStatus[idx]) digitalWrite(OUTPUT_PINS[idx], LOW);
    pinStatus[idx]= 0;
    previousTimes[idx]= micros();
  }
  else
  {
    if(pinStatus[idx]== 1) // OFFSET
    {
      if(!(micros()-previousTimes[idx] > preDurations[idx])) return;
      // OFFSET done!
      pinStatus[idx]= 2; // -> HIGH
      digitalWrite(OUTPUT_PINS[idx], HIGH);
      previousTimes[idx]= micros();
    }
    else if(pinStatus[idx]== 2) // HIGH
    {
      if(!(micros()-previousTimes[idx] > highDurations[idx])) return;
      // HIGH done!
      pinStatus[idx]= 3; // -> LOW
      digitalWrite(OUTPUT_PINS[idx], LOW);
      previousTimes[idx]= micros();
    }
    else if(pinStatus[idx]== 3) // LOW
    {
      if(!(micros()-previousTimes[idx] > postDurations[idx])) return;
      // LOW done!
      pinStatus[idx]= 1; // -> OFFSET
      digitalWrite(OUTPUT_PINS[idx], LOW);
      previousTimes[idx]= micros();
    }
    else // 0
    {
      if(!(micros()-previousTimes[idx] > INITIAL_DELAY[idx])) return;
      // DELAY done!
      pinStatus[idx]= 1; // -> OFFSET
      digitalWrite(OUTPUT_PINS[idx], LOW);
      previousTimes[idx]= micros();
    }
  }
}

void loop()
{ 
  for(short i=0;i<3;i++) updatePIN(i); 
}
