// Przypisanie pinów zgodnie z fizycznym układem ATTINY84 DIP14
const int pinStartBtn = 0;   // PA0 – pin 13 DIP – przycisk START (chwilowy, z kondensatorem 100nF)
const int pinStopBtn  = 1;   // PA2 – pin 12 DIP – przycisk STOP (chwilowy, aktywne LOW, z kondensatorem 100nF)
const int pinLedStart = 2;   // PA1 – pin 11 DIP – zielona dioda LED START
const int pinLedStop  = 3;   // PA3 – pin 10 DIP – czerwona dioda LED STOP
const int pinRelayOut = 7;   // PA7 – pin 6 DIP – przekaźnik / silnik (poprawione!)

bool machineRunning = false;
unsigned long previousMillis = 0;
const unsigned long blinkInterval = 1500;
bool ledState = false;

void setup() {
  pinMode(pinStartBtn, INPUT_PULLUP);
  pinMode(pinStopBtn, INPUT_PULLUP);

  pinMode(pinLedStart, OUTPUT);
  pinMode(pinLedStop, OUTPUT);
  pinMode(pinRelayOut, OUTPUT);

  digitalWrite(pinRelayOut, LOW);     // przekaźnik wyłączony
  digitalWrite(pinLedStart, LOW);     // zielona LED wyłączona (wejdzie w tryb migania)
  digitalWrite(pinLedStop, HIGH);     // czerwona świeci – tryb oczekiwania
}

void loop() {
  bool startPressed = (digitalRead(pinStartBtn) == LOW);
  bool stopPressed  = (digitalRead(pinStopBtn) == LOW);

  if (!machineRunning) {
    // Miganie zielonej diody w trybie oczekiwania
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= blinkInterval) {
      previousMillis = currentMillis;
      ledState = !ledState;
      digitalWrite(pinLedStart, ledState);
    }

    // START aktywuje tylko jeśli STOP nie jest wciśnięty
    if (startPressed && !stopPressed) {
      machineRunning = true;
      digitalWrite(pinRelayOut, HIGH);
      digitalWrite(pinLedStart, HIGH);
      digitalWrite(pinLedStop, LOW);
    }

  } else {
    // STOP zatrzymuje maszynę i wraca do migania
    if (stopPressed) {
      machineRunning = false;
      digitalWrite(pinRelayOut, LOW);
      digitalWrite(pinLedStop, HIGH);
      digitalWrite(pinLedStart, LOW);

      // natychmiastowe rozpoczęcie migania
      previousMillis = millis() - blinkInterval;
      ledState = false;
    }
  }
}
