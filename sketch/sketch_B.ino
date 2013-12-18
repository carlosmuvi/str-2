#define TIEMPO_CICLO 100 
int soundPin = 11;
int buttonPin =  13;
int ledPin =  7;

int is_muted = 0;
unsigned long timeOrig;


ISR(TIMER1_COMPA_vect)
{
  static unsigned char data = 0;
  if (Serial.available()>1) {
    data = Serial.read();
    if (is_muted) {
      OCR2A = (byte) data;
    } else {
      OCR2A = (byte) 0;
    }
  }
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
    pinMode(ledPin, OUTPUT);
    pinMode(buttonPin, INPUT);
    Serial.begin(115200);

    //TIMER_1
    TCCR1B = _BV(WGM12)| _BV(CS10);
    TCCR1A = 0;
    OCR1A=4000;
    TIMSK1= TIMSK1 | _BV(OCIE1A);

    //TIMER_2
    TCCR2A = _BV(COM2A1) | _BV(COM2B1) | _BV(WGM21) | _BV(WGM20);
    TCCR2B = _BV(CS20);

    timeOrig = millis();
}

void loop ()
{
    unsigned long timeDiff;

    mute();
    timeDiff = TIEMPO_CICLO - (millis() - timeOrig);
    timeOrig = timeOrig + TIEMPO_CICLO;
    delay(timeDiff);
}
