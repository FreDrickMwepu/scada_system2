const int LED_PINS[] = {3, 5, 9, 11};  // Use PWM-capable pins
const int NUM_LEDS = 4;
const int CURRENT_SENSOR_PIN = A0;    // Analog pin for current sensor
const int POWER_BUTTON_PIN = 12;       // Pin for the power button

bool powerState = false;  // Track the power state (off by default)

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < NUM_LEDS; i++) {
    pinMode(LED_PINS[i], OUTPUT);
    digitalWrite(LED_PINS[i], LOW);   // Start with all LEDs off
  }
  pinMode(CURRENT_SENSOR_PIN, INPUT);
  pinMode(POWER_BUTTON_PIN, OUTPUT);
  digitalWrite(POWER_BUTTON_PIN, LOW); // Ensure the power is off initially
}

void loop() {
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command.startsWith("TOGGLE")) {
      int ledIndex = command.charAt(6) - '0';  // Extract LED number
      if (ledIndex >= 0 && ledIndex < NUM_LEDS) {
        int currentState = digitalRead(LED_PINS[ledIndex]);
        digitalWrite(LED_PINS[ledIndex], !currentState);  // Toggle the LED
        Serial.print("BULB ");
        Serial.print(ledIndex);
        Serial.print(" is now ");
        Serial.println(!currentState ? "OFF" : "ON");
      }
    } else if (command == "STATUS") {
      for (int i = 0; i < NUM_LEDS; i++) {
        int state = digitalRead(LED_PINS[i]);
        Serial.print(state ? "OFF" : "ON");
        if (i < NUM_LEDS - 1) Serial.print(",");
      }
      Serial.println();
    } else if (command == "READ") {
      int sensorValue = analogRead(CURRENT_SENSOR_PIN);
      float voltage = sensorValue * (5.0 / 1023.0);
      float current = voltage * 1;  // Replace with actual calibration factor
      Serial.println(current, 3);   // Print the current with 3 decimal places
    } else if (command == "RESET") {
      for (int i = 0; i < NUM_LEDS; i++) {
        digitalWrite(LED_PINS[i], LOW);  // Turn off all LEDs
      }
      Serial.println("All LEDs turned OFF");
    } else if (command == "POWER") {
      powerState = !powerState;
      digitalWrite(POWER_BUTTON_PIN, powerState ? HIGH : LOW);
      Serial.print("POWER is now ");
      Serial.println(powerState ? "ON" : "OFF");
    }
  }
}