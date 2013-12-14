#define SAMPLE_TIME 250 
int soundPin = ;
int buttonPin =  ;
int ledPin =  ;

int is_muted = 0;
unsigned long timeOrig;


void play_bit() 
{
  static int bitwise = 1;
  static unsigned char data = 0;
    
    bitwise = (bitwise * 2);
    if (bitwise > 128) {
        bitwise = 1;
        if (Serial.available()>1) {
           data = Serial.read();
        }
    }
    digitalWrite(soundPin, (data & bitwise) );
}

void mute(){
  int boton = digitalRead(buttonPin);
  
  switch(boton){
    case HIGH:
      is_muted = 1;
      digitalWrite(ledPin, is_muted);
      break;
    case LOW:
      is_muted = 0;
      digitalWrite(ledPin, is_muted);
      break;
  }
}

void setup ()
{
    pinMode(soundPin, OUTPUT);
    Serial.begin(115200);
    timeOrig = micros();

    TCCR1B = _BV(WGM12)| _BV(CS10);
    TCCR1A = 0;
    OCR1A=4000;

    TIMSK1= TIMSK1 | _BV(OCIE1A);
    
}

void loop ()
{
    unsigned long timeDiff;

    play_bit();
    timeDiff = SAMPLE_TIME - (micros() - timeOrig);
    timeOrig = timeOrig + SAMPLE_TIME;
    delayMicroseconds(timeDiff);
}
