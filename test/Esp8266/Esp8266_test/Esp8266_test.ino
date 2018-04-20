
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define WIFI_SSID "JJ home"
#define WIFI_PASSWORD "akshay003"

#define FIREBASE_HOST "air-quality-dashboard.firebaseio.com"
#define FIREBASE_AUTH "muqRjG3idPKRueg1X4DWFuwKtbOs07siYj9CPJwo"

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  // Begin Connection to Firebase
  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
}

void loop() {
  
  while(Serial.available() > 0){
    
    StaticJsonBuffer<200> jsonBuffer;
    JsonObject& entry = jsonBuffer.createObject();
    JsonObject& time_stamp = entry.createNestedObject("timestamp");
    
    String gas1 = Serial.readStringUntil(',');
    String gas2 = Serial.readStringUntil(',');
    String gas3 = Serial.readStringUntil(',');
    String gas4 = Serial.readStringUntil('\n');

    entry["gas1"] = gas1;
    entry["gas2"] = gas2;
    entry["gas3"] = gas3;
    entry["gas4"] = gas4;

    time_stamp[".sv"] = "timestamp";

    Serial.println(FIREBASE_HOST);
    Serial.println("/entries");
    
    Firebase.push("entries",entry);
    if (Firebase.failed()) {
        Serial.print("setting /entry failed:");
        Serial.println(Firebase.error());
        return;
    }
  }
}
