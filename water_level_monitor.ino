int sensorPin = A0;
int BUZZER = 8;
int RED = 9;
int YELLOW = 10;
int GREEN = 11;

int high_water = 300;
int low_water = 250;

void setup() {
  pinMode(sensorPin, INPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(YELLOW, OUTPUT);
  pinMode(GREEN, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int water_level = analogRead(sensorPin);

  Serial.println(water_level);
  
  if (water_level > high_water){
    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, LOW);
    digitalWrite(YELLOW, LOW);
    tone(BUZZER, 2500, 200); // beep for 200ms
    delay(100);              // silence for 100ms
  } else if(water_level > low_water){
    digitalWrite(RED, LOW);
    digitalWrite(GREEN, LOW);
    digitalWrite(YELLOW, HIGH);
    tone(BUZZER, 1000, 200);
    delay(300);
  } else {
    digitalWrite(RED, LOW);
    digitalWrite(GREEN, HIGH);
    digitalWrite(YELLOW, LOW);
    noTone();
  }

}
