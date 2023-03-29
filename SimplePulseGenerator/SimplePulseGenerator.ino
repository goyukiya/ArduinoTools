/** SIMPLE PULSE GENERATOR
 * reads the state of the input PIN and outputs an PULSE if the input pin is HIGH.
 * 
 * author: yuki.goya@riken.jp 
 * v22.06.03
 **/

// output pin
const int OUTPUT_PIN= 2;
// input pin
const int INPUT_PIN= 7;

// pulse High duration in microseconds
const long PULSE_DUR= 8333; // 60Hz at 50% duty cycle

// pulse frequency in Hz 
const float PULSE_FREQ= 60.0;

// pulse low/high durations in microseconds
unsigned long lowDuration;
unsigned long highDuration;

unsigned long previousTime= 0;

void setup()
{
  // set pins
  pinMode(OUTPUT_PIN, OUTPUT);   
  pinMode(INPUT_PIN, INPUT);

  // period in microseconds
  double pulsePeriod= 1000.0*1000.0/PULSE_FREQ; 
  
  // compute HIGH and LOW durations
  highDuration= (unsigned long) ((PULSE_DUR >= pulsePeriod)? pulsePeriod/2 : PULSE_DUR); // 50% duty cycle if requested duration is too high
  lowDuration= (unsigned long) (pulsePeriod - highDuration);
  
  // initial status
  digitalWrite(OUTPUT_PIN, LOW);
}

void outputOnePulse()
{
  // one pulse
  digitalWrite(OUTPUT_PIN, HIGH);
  // wait for highDuration
  previousTime = micros();
  while(!(micros()-previousTime > highDuration)){}
  digitalWrite(OUTPUT_PIN, LOW);
  // wait for lowDuration
  previousTime= micros();
  while(!(micros()-previousTime > lowDuration)){}  
}

void loop()
{
  // pulse 
  (digitalRead(INPUT_PIN)==HIGH)? outputOnePulse() : digitalWrite(OUTPUT_PIN, LOW);
}
