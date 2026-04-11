void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  // turn the LED on
  digitalWrite(LED_BUILTIN, HIGH);
  delay(1000); // Wait for 1000 ms
  // turn the LED off
  digitalWrite(LED_BUILTIN, LOW);
  delay(1000); // Wait for 1000 ms
} 
