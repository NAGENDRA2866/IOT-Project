#include <WiFi.h>
const char* ssid = "DARLING";
const char* password = "00000000";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ Connected!");
  Serial.println(WiFi.localIP());
}

void loop() {}
