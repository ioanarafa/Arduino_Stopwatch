#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

const int startButton = 22;     
const int stopButton = 23;      
const int resetButton = 25;    
const int setMinutesButton = 31; 
const int setSecondsButton = 33; 
const int modeButton = 27;   
const int buzzerPin = 35;      
const int ledPin = 37;       


enum Mode { CRONOMETRU, TIMER };
Mode currentMode = CRONOMETRU;

// Variabile Cronometru
unsigned long startTime = 0;
unsigned long elapsedTime = 0;
bool runningCronometru = false;

// Variabile Timer
unsigned int setMinutes = 0;    
unsigned int setSeconds = 0;    
unsigned long remainingTime = 0; 
bool runningTimer = false;


bool paused = false;          


void handleCronometru();
void handleTimer();
void alarm();
void playBuzzerShort();
void displayTime(unsigned long time, const char* label);

void setup() {

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Mode: Cronometru");


  pinMode(startButton, INPUT_PULLUP);
  pinMode(stopButton, INPUT_PULLUP);
  pinMode(resetButton, INPUT_PULLUP);
  pinMode(setMinutesButton, INPUT_PULLUP);
  pinMode(setSecondsButton, INPUT_PULLUP);
  pinMode(modeButton, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(buzzerPin, LOW); 
  digitalWrite(ledPin, LOW);   
}

void loop() {
  // Apasare buton de mod
  if (digitalRead(modeButton) == LOW) {
    currentMode = (currentMode == CRONOMETRU) ? TIMER : CRONOMETRU;
    lcd.clear();
    if (currentMode == CRONOMETRU) {
      lcd.print("Mode: Cronometru");
      elapsedTime = 0;
      runningCronometru = false;
    } else {
      lcd.print("Mode: Timer");
      setMinutes = 0;
      setSeconds = 0;
      runningTimer = false;
    }
    delay(300);
  }


  if (currentMode == CRONOMETRU) {
    handleCronometru();
  } else if (currentMode == TIMER) {
    handleTimer();
  }
}

// Functia pentru activarea alarmei la final cu LED-ul în stil girofar
void alarm() {
  for (int i = 0; i < 8; i++) { 

    digitalWrite(ledPin, HIGH);
    digitalWrite(buzzerPin, HIGH);
    delay(200);

    digitalWrite(ledPin, LOW);
    digitalWrite(buzzerPin, LOW);
    delay(100);

    digitalWrite(ledPin, HIGH);
    digitalWrite(buzzerPin, HIGH);
    delay(200);

    digitalWrite(ledPin, LOW);
    digitalWrite(buzzerPin, LOW);
    delay(100);
  }
}

// Funcția pentru un sunet scurt (start/stop)
void playBuzzerShort() {
  digitalWrite(buzzerPin, HIGH);
  digitalWrite(ledPin, HIGH); 
  delay(100); 
  digitalWrite(buzzerPin, LOW);
  digitalWrite(ledPin, LOW);
}

// Afisarea timpului pe LCD
void displayTime(unsigned long time, const char* label) {
  unsigned long seconds = (time / 1000) % 60;
  unsigned long minutes = (time / 60000);

  lcd.setCursor(0, 1);
  lcd.print(label);
  lcd.print(minutes);
  lcd.print(":");
  if (seconds < 10) {
    lcd.print("0"); 
  }
  lcd.print(seconds);
}

// Functia pentru cronometru
void handleCronometru() {

  if (digitalRead(startButton) == LOW && !runningCronometru) {
    runningCronometru = true;
    startTime = millis() - elapsedTime; 
    lcd.setCursor(0, 0);
    lcd.print("Cronometru: RUN ");
    playBuzzerShort(); 
    delay(300);
  }

  // STOP
  if (digitalRead(stopButton) == LOW && runningCronometru) {
    runningCronometru = false;
    elapsedTime = millis() - startTime;
    lcd.setCursor(0, 0);
    lcd.print("Cronometru: PAUSE");
    playBuzzerShort();
    delay(300);
  }

  // RESET
  if (digitalRead(resetButton) == LOW) {
    runningCronometru = false;
    elapsedTime = 0;
    lcd.clear();
    lcd.print("Mode: Cronometru");
    delay(300); 
  }

  // Actualizare timp
  if (runningCronometru) {
    elapsedTime = millis() - startTime;
  }
  displayTime(elapsedTime, "Elapsed: ");
}

// Functia pentru timer
void handleTimer() {
  // Setare Minute
  if (digitalRead(setMinutesButton) == LOW && !runningTimer) {
    setMinutes++;
    if (setMinutes > 99) { 
      setMinutes = 0;
    }
    lcd.setCursor(0, 1);
    lcd.print("Set Timer: ");
    lcd.print(setMinutes);
    lcd.print(":");
    if (setSeconds < 10) {
      lcd.print("0");
    }
    lcd.print(setSeconds);
    delay(300);
  }

  // Setare Secunde
  if (digitalRead(setSecondsButton) == LOW && !runningTimer) {
    setSeconds += 5; 
    if (setSeconds >= 60) { 
      setSeconds = 0;
    }
    lcd.setCursor(0, 1);
    lcd.print("Set Timer: ");
    lcd.print(setMinutes);
    lcd.print(":");
    if (setSeconds < 10) {
      lcd.print("0");
    }
    lcd.print(setSeconds);
    delay(300); 
  }

  // START
  if (digitalRead(startButton) == LOW && !runningTimer) {
    runningTimer = true;
    remainingTime = (setMinutes * 60000) + (setSeconds * 1000);
    lcd.clear();
    lcd.print("Timer Running...");
    playBuzzerShort(); 
    delay(300); 
  }

  // STOP
  if (digitalRead(stopButton) == LOW && runningTimer) {
    paused = !paused; 
    lcd.setCursor(0, 0);
    lcd.print(paused ? "Timer Paused    " : "Timer Running...");
    playBuzzerShort();
    delay(300); 
  }

  // RESET
  if (digitalRead(resetButton) == LOW) {
    runningTimer = false;
    paused = false;
    setMinutes = 0;
    setSeconds = 0;
    remainingTime = 0;
    lcd.clear();
    lcd.print("Mode: Timer");
    delay(300); 
  }

  // Actualizare Timer
  if (runningTimer && !paused) {
    if (remainingTime > 0) {
      if (remainingTime <= 10000) {
        digitalWrite(ledPin, HIGH);
        delay(500);
        digitalWrite(ledPin, LOW);
        delay(500);
      }
      remainingTime -= 1000;
      displayTime(remainingTime, "Time Left: ");
      delay(1000); 
    } else {
      // Timpul a expirat
      runningTimer = false;
      digitalWrite(ledPin, LOW); 
      alarm();
      lcd.setCursor(0, 0);
      lcd.print("Time's Up!      ");
    }
  }
}
