const int OUTPUT_PIN=2;
const int INPUT_PULSE_PIN=8;
const int INPUT_STATIC_PIN=10;

double pulseFrequency; // in Hz
double pulseDutyCycle; // in %
unsigned long highDuration; // in microseconds
unsigned long lowDuration; // in microseconds

unsigned long previousTime=0;

void setup() {
  // set pins
  pinMode(OUTPUT_PIN, OUTPUT);   
  pinMode(INPUT_PULSE_PIN, INPUT);
  pinMode(INPUT_STATIC_PIN, INPUT);
  
  // define frequency and duty cycle
  pulseFrequency= 20; // Hz
  pulseDutyCycle= 20; // %
  // compute HIGH and LOW durations   
  double pulsePeriod= 1000.0 * 1000.0/pulseFrequency; // microseconds
  highDuration=(unsigned long)(pulsePeriod * pulseDutyCycle/100);
  lowDuration=(unsigned long)(pulsePeriod - highDuration);

  // initial status
  digitalWrite(OUTPUT_PIN, LOW);
}

void loop(){
  if(digitalRead(INPUT_PULSE_PIN)==HIGH){ // pulse output
    // one pulse
    digitalWrite(OUTPUT_PIN, HIGH);
    // wait for highDuration
    previousTime = micros();
    while(!(micros()-previousTime > highDuration)){} 
    digitalWrite(OUTPUT_PIN, LOW);
    // wait for lowDuration
    previousTime = micros();
    while(!(micros()-previousTime > lowDuration)){}  
  }else if(digitalRead(INPUT_STATIC_PIN)==HIGH){ // static output
    digitalWrite(OUTPUT_PIN, HIGH);
  }
  else{ // zero
    digitalWrite(OUTPUT_PIN, LOW);
  }
}
