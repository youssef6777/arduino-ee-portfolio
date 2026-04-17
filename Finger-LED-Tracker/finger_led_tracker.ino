const int ledPins[5] = {8, 9, 10, 11, 12};

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 5; i++) {
    pinMode(ledPins[i], OUTPUT);
    digitalWrite(ledPins[i], LOW);
  }
}

void loop() {
  if (Serial.available() > 0) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();
    // Expect a 5-char string like "10110"
    if (cmd.length() == 5) {
      for (int i = 0; i < 5; i++) {
        digitalWrite(ledPins[i], cmd[i] == '1' ? HIGH : LOW);
      }
    }
  }
}
