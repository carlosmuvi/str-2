#define TIEMPO_CICLO 100 
int soundPin = 11;
unsigned long timeOrig;


ISR(TIMER1_COMPA_vect)
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
    if (is_muted) {
        digitalWrite(soundPin, (data & bitwise) );
    } else {
        digitalWrite(soundPin, 0 );
    }
}

void setup ()
{
    pinMode(soundPin, OUTPUT);
    Serial.begin(115200);
    timeOrig = millis();    
}

void loop ()
{
    unsigned long timeDiff;

    play_bit();
    timeDiff = TIEMPO_CICLO - (millis() - timeOrig);
    timeOrig = timeOrig + TIEMPO_CICLO;
    delay(timeDiff);
}
