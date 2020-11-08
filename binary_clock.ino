// Buttons
const int setButton = 1;
const int changeButton = 2;

// LEDs for minutes
const int led3 = 3;
const int led4 = 4;
const int led5 = 5;
const int led6 = 6;
const int led7 = 7;
const int led8 = 8;

// LEDs for hours
const int led9 = 9;
const int led10 = 10;
const int led11 = 11;
const int led12 = 12;
const int led13 = 13;

bool setButtonLow = true;
bool changeButtonLow = true;

bool inSetHourMode = false;
bool inSetMinuteMode = false;

int hour = 0;
int minute = 0;

long prevMillis = 0;

void setup() {
  pinMode(setButton, INPUT);
  pinMode(changeButton, INPUT);

  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(led6, OUTPUT);
  pinMode(led7, OUTPUT);
  pinMode(led8, OUTPUT);

  pinMode(led9, OUTPUT);
  pinMode(led10, OUTPUT);
  pinMode(led11, OUTPUT);
  pinMode(led12, OUTPUT);
  pinMode(led13, OUTPUT);
}

void loop() {
  int setState = digitalRead(setButton);
  if (setState == HIGH && setButtonLow) {
    if (inSetHourMode) {
      inSetHourMode = false;
      inSetMinuteMode = true;
      showMinuteActive();
    } else if (inSetMinuteMode) {
      inSetMinuteMode = false;
      prevMillis = millis() / 60000 * 60000;
      showSetDone();
    } else {
      inSetHourMode = true;
      showHourActive();
    }
    setButtonLow = false;
  } else if (setState == LOW) {
    setButtonLow = true;
  }

  int changeState = digitalRead(changeButton);
  if (changeState == HIGH && changeButtonLow) {
    if (inSetHourMode) {
      hour = (hour + 1) % 24;
      writeHour();
    } else if (inSetMinuteMode) {
      minute = (minute + 1) % 60;
      writeMinute();
    }
    changeButtonLow = false;
  } else if (changeState == LOW) {
    changeButtonLow = true;
  }

  if (!inSetMode()) {
    long currMillis = millis();
    if (currMillis - prevMillis >= 60000) {
      prevMillis = currMillis / 60000 * 60000;
      minute = (minute + 1) % 60;
      writeMinute();

      if (minute == 0) {
        hour = (hour + 1) % 24;
        writeHour();
      }
    }
  }
}

boolean inSetMode() {
  return inSetMinuteMode || inSetHourMode;
}

void writeHour() {
  writeTime(hour, led9, led13);
}

void writeMinute() {
  writeTime(minute, led3, led8);
}

void writeTime(int number, int ledStart, int ledEnd) {
  for (int led = ledStart; led <= ledEnd; led++) {
    digitalWrite(led, bitRead(number, led - ledStart)); 
  }
}

// Show visual confirmation of setting the time
void showHourActive() {
  showActive(led9, led13);
  writeHour();
}

void showMinuteActive() {
  showActive(led3, led8);
  writeMinute();
}

void showSetDone() {
  for (int i = 0; i < 3; i++) {
    writeTime(0, led3, led13);
    delay(200);
    writeHour();
    writeMinute();
    delay(200); 
  }
}

void showActive(int ledStart, int ledEnd) {
  for (int i = 0; i < 3; i++) {
    for (int led = ledStart; led <= ledEnd; led++) {
      digitalWrite(led, HIGH);
    }
    delay(200);
    for (int led = ledStart; led <= ledEnd; led++) {
      digitalWrite(led, LOW);
    }
    delay(200);
  }
}
