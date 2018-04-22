
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#define WIFI_SSID "janakiact"
#define WIFI_PASSWORD "9840100640"

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
    
    String CH4 = Serial.readStringUntil(',');
    String CO = Serial.readStringUntil(',');
    String CO2 = Serial.readStringUntil(',');
    String Smoke = Serial.readStringUntil(',');
    String Temp = Serial.readStringUntil(',');
    String humi = Serial.readStringUntil('\n');

    entry["CH4"] = CH4;
    entry["CO"] = CO;
    entry["CO2"] = CO2;
    entry["Smoke"] = Smoke;
    entry["temperature"] = Temp;
    entry["humidity"] = humi;

    time_stamp[".sv"] = "timestamp";

    Serial.println(FIREBASE_HOST);
    Serial.println("/proto_entries");
    
    Firebase.push("proto_entries",entry);
    if (Firebase.failed()) {
        Serial.print("setting entry failed:");
        Serial.println(Firebase.error());
        return;
    }
  }
}
