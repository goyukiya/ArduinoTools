# MultiPulseGenerator

Reads the state of the input PIN and outputs several pulses if the input pin is HIGH.

author: yuki.goya@riken.jp

## Settings

### Input PIN

Uses Ch.2 for input.
`INPUT_PIN= 2;`

### Output PINs

Uses Ch.8, 9 & 10 for output.
`OUTPUT_PINS[3]= {8, 9, 10};`

### Output Pulses Parameters

- Frequencies defined in Hertz.
  `PULSE_FREQ[3]= {20.0, 20.0, 40.0};`

- Single pulse widths defined in microseconds.
  `PULSE_WID[3]= {20000, 20000, 16000};`

- Pulse start offsets in microseconds.
  `PULSE_OFFSET[3]= {0, 0, 0};`

- Output initial delays in microseconds
  `INITIAL_DELAY[3]= {0, 25000, 2000};`
