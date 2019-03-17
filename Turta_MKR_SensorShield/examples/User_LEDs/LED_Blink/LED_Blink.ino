// Define LED connections
#define LED_B 2
#define LED_R 3

void setup() {
  // Initialize LEDs
  pinMode(LED_B, OUTPUT);
  pinMode(LED_R, OUTPUT);
}

void loop() {
  // Turn on Blue LED
  digitalWrite(LED_B, HIGH);
  delay(500);

  // Turn on Red LED
  digitalWrite(LED_R, HIGH);
  delay(500);

  // Turn off Blue LED
  digitalWrite(LED_B, LOW);
  delay(500);

  // Turn off Red LED
  digitalWrite(LED_R, LOW);
  delay(500);
}