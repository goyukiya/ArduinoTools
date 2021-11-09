// output pin
const int OUTPUT_PIN=2;
// input pins
const int INPUT_PULSE0_PIN=7;
const int INPUT_PULSE1_PIN=8;
const int INPUT_PULSE2_PIN=9;
const int INPUT_PULSE3_PIN=11;
const int INPUT_STATIC_PIN=10;

// pulse High duration in microseconds
const long PULSE0_DUR= 5000;
const long PULSE1_DUR = 5000;
const long PULSE2_DUR = 5000;
const long PULSE3_DUR = 1000000;
// pulse frequencies in Hz 
const float PULSE0_FREQ = 20.0;
const float PULSE1_FREQ = 10.0;
const float PULSE2_FREQ = 4.0;
const float PULSE3_FREQ = 0.333;
 
// pulse low/high durations in microseconds
unsigned long lowDurations[4];
unsigned long highDurations[4];

unsigned long previousTime=0;

void setup()
{
  // set pins
  pinMode(OUTPUT_PIN, OUTPUT);   
  pinMode(INPUT_PULSE0_PIN, INPUT);
  pinMode(INPUT_PULSE1_PIN, INPUT);
  pinMode(INPUT_PULSE2_PIN, INPUT);
  pinMode(INPUT_STATIC_PIN, INPUT);

  // compute HIGH and LOW durations
  // PULSE0
  double pulsePeriod= 1000.0 * 1000.0/PULSE0_FREQ; // microseconds
  highDurations[0]= (unsigned long) ((PULSE0_DUR >= pulsePeriod)? pulsePeriod/2 : PULSE0_DUR);
  lowDurations[0]= (unsigned long) (pulsePeriod - highDurations[0]);
  // PULSE1
  pulsePeriod= 1000.0 * 1000.0/PULSE1_FREQ;
  highDurations[1]= (unsigned long) ((PULSE1_DUR >= pulsePeriod)? pulsePeriod/2 : PULSE1_DUR);
  lowDurations[1]= (unsigned long) (pulsePeriod - highDurations[1]);
  // PULSE2
  pulsePeriod= 1000.0 * 1000.0/PULSE2_FREQ;
  highDurations[2]= (unsigned long) ((PULSE2_DUR >= pulsePeriod)? pulsePeriod/2 : PULSE2_DUR);
  lowDurations[2]= (unsigned long) (pulsePeriod - highDurations[2]);
  // PULSE3
  pulsePeriod= 1000.0 * 1000.0/PULSE3_FREQ;
  highDurations[3]= (unsigned long) ((PULSE3_DUR >= pulsePeriod)? pulsePeriod/2 : PULSE3_DUR);
  lowDurations[3]= (unsigned long) (pulsePeriod - highDurations[3]);
  
  // initial status
  digitalWrite(OUTPUT_PIN, LOW);
}

void outputOnePulse(int idx)
{
  // one pulse
  digitalWrite(OUTPUT_PIN, HIGH);
  // wait for highDuration
  previousTime = micros();
  while(!(micros()-previousTime > highDurations[idx])){} 
  digitalWrite(OUTPUT_PIN, LOW);
  // wait for lowDuration
  previousTime = micros();
  while(!(micros()-previousTime > lowDurations[idx])){}  
}

void loop()
{
  if(digitalRead(INPUT_PULSE0_PIN)==HIGH) outputOnePulse(0); // pulse 0
  else if (digitalRead(INPUT_PULSE1_PIN)==HIGH) outputOnePulse(1); // pulse 1
  else if(digitalRead(INPUT_PULSE2_PIN)==HIGH) outputOnePulse(2); // pulse 2
  else if(digitalRead(INPUT_PULSE3_PIN)==HIGH) outputOnePulse(3); // pulse 3
  else if(digitalRead(INPUT_STATIC_PIN)==HIGH) digitalWrite(OUTPUT_PIN, HIGH); // static  
  else digitalWrite(OUTPUT_PIN, LOW); // none
}
