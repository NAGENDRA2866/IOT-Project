#include <WiFi.h>
#include <ThingSpeak.h>

const char* ssid = "DARLING";
const char* password = "00000000";
WiFiClient client;
unsigned long channelID = 3148584;
const char* writeAPI = "OPUAXRABODX4ZVZ6";

#define VOLTAGE_PIN 34
#define CURRENT_PIN 35

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); }
  ThingSpeak.begin(client);
}

void loop() {
  int vRaw = analogRead(VOLTAGE_PIN);
  int cRaw = analogRead(CURRENT_PIN);
  float voltage = (vRaw / 4095.0) * 3.3 * 11.0;
  float current = ((cRaw / 4095.0) * 3.3 - 1.65) / 0.1;
  float power = voltage * current;

  ThingSpeak.setField(1, voltage);
  ThingSpeak.setField(2, current);
  ThingSpeak.setField(3, power);
  ThingSpeak.writeFields(channelID, writeAPI);

  Serial.printf("Sent: V=%.2fV  I=%.2fA  P=%.2fW\n", voltage, current, power);
  delay(15000);
}
