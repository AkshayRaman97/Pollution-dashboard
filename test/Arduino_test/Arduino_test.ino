void setup() {
  // Beginning Serial communication
  Serial.begin(115200);
  
}

// Test data
int gas1,gas2,gas3,gas4;

void loop() {
  // Generating random data
  gas1 = random(1,6);
  gas2 = random(1,6);
  gas3 = random(1,6);
  gas4 = random(1,6);

  // Generatng output string
  String output = String(gas1) + "," + String(gas2) + "," + String(gas3) + "," + String(gas4);
  
  // Send to Esp8266
  Serial.print(output + "\n");

  delay(5000);
}
