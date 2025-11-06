#include <WiFi.h>
#include <ThingSpeak.h>


const char* ssid = "DARLING";      
const char* password = "00000000";  
unsigned long channelID = 3148584; 
const char* writeAPI = "HP6G2JGSBXFXT43Q"; 
WiFiClient client;


#define VOLTAGE_PIN 34
#define CURRENT_PIN 35
#define RELAY_PIN 26


float sensitivity = 0.1; 
float voltageCalibration = 11.0; 

void setup() {
  Serial.begin(115200);
  
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
  
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi Connected!");
  Serial.println(WiFi.localIP());
  
  ThingSpeak.begin(client);
}

void loop() {
  // -------- Read Voltage --------
  int rawVoltage = analogRead(VOLTAGE_PIN);
  float voltage = (rawVoltage / 4095.0) * 3.3 * voltageCalibration;

  // -------- Read Current --------
  int rawCurrent = analogRead(CURRENT_PIN);
  float currentVoltage = (rawCurrent / 4095.0) * 3.3;
  float current = (currentVoltage - 1.65) / sensitivity;

  // -------- Calculate Power --------
  float power = voltage * current;
  if (power < 0) power = 0; // Avoid negative noise

  // -------- Print on Serial Monitor --------
  Serial.println("========================");
  Serial.print("Voltage: "); Serial.print(voltage); Serial.println(" V");
  Serial.print("Current: "); Serial.print(current); Serial.println(" A");
  Serial.print("Power: "); Serial.print(power); Serial.println(" W");

  // -------- LED Blinking (Relay Control) --------
  digitalWrite(RELAY_PIN, HIGH); // LED ON
  delay(1000);
  digitalWrite(RELAY_PIN, LOW);  // LED OFF
  delay(1000);

  // -------- Send Data to ThingSpeak --------
  ThingSpeak.setField(1, voltage);
  ThingSpeak.setField(2, current);
  ThingSpeak.setField(3, power);

  int result = ThingSpeak.writeFields(channelID, writeAPI);
  if (result == 200) {
    Serial.println("Data sent to ThingSpeak successfully!");
  } else {
    Serial.print("Error sending data. HTTP error code: ");
    Serial.println(result);
  }

  Serial.println("========================\n");

  delay(15000);
}