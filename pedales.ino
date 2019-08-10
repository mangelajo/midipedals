#include <Bounce.h>  // Bounce library makes button change detection easy

const int channel = 8;
const int velocity = 99;

#define D0 5
#define D1 6
#define D2 7
#define D3 8


 // 5 = 5 ms debounce time which is appropriate for good quality mechanical pushbuttons
#define DEBOUNCE_NORMAL 5
#define SOFT_LONGPRESS 500

typedef struct stPedal {
  Bounce debouncer;
  uint8_t note;
} Pedal;

typedef struct stSoftPedal {
  unsigned long onTime;
  uint8_t sent;
  uint8_t note;
  
} SoftPedal;

Pedal pedals[]  = {
    {Bounce(D0, DEBOUNCE_NORMAL), 60},
    {Bounce(D1, DEBOUNCE_NORMAL), 61},
    {Bounce(D2, DEBOUNCE_NORMAL), 62},
    {Bounce(D3, DEBOUNCE_NORMAL), 63},
};

SoftPedal softPedals [] {
  {0, 0, 64},
  {0, 0, 65},
  {0, 0, 66},
  {0, 0, 67},
};

#define N_PEDALS (sizeof(pedals)/sizeof(pedals[0]))
#define N_SOFTPEDALS (sizeof(softPedals)/sizeof(softPedals[0]))

void setup() {
  pinMode(D0, INPUT_PULLUP);
  pinMode(D1, INPUT_PULLUP);
  pinMode(D2, INPUT_PULLUP);
  pinMode(D3, INPUT_PULLUP);
}

void loop() {

  unsigned long t;

  for (uint8_t i=0; i<N_PEDALS; i++)  {
      pedals[i].debouncer.update();
      if (pedals[i].debouncer.fallingEdge()) {
          usbMIDI.sendNoteOn(pedals[i].note, velocity, channel);
          softPedals[i].onTime = millis();
          softPedals[i].sent = 0;
      } 
      if (pedals[i].debouncer.risingEdge()) {
          usbMIDI.sendNoteOff(pedals[i].note, velocity, channel);
          softPedals[i].onTime = 0;
          if (softPedals[i].sent) {
            usbMIDI.sendNoteOff(softPedals[i].note, velocity, channel);
            softPedals[i].sent = 0;
          }
      }
      t = millis();
      
      if (softPedals[i].onTime != 0 &&
          softPedals[i].sent   == 0 && 
          ((t-softPedals[i].onTime)>SOFT_LONGPRESS)) {
            
        softPedals[i].sent = 1;
        usbMIDI.sendNoteOn(softPedals[i].note, velocity, channel);
      }
      
  }
  
  // MIDI Controllers should discard incoming MIDI messages.
  while (usbMIDI.read()) {
  }

}

