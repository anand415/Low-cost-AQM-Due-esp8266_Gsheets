//Include Lib for Arduino to Nodemcu
#include <SoftwareSerial.h>
#include <ArduinoJson.h>

//D6 = Rx & D5 = Tx
SoftwareSerial nodemcu(D6, D5);

//Timer to run Arduino code every 5 seconds
unsigned long previousMillis = 0;
unsigned long currentMillis;
const unsigned long period = 10000;  

void setup() {
  // Initialize Serial port
  Serial.begin(9600);
  nodemcu.begin(2400);
  while (!Serial) continue;
}

void loop() {
  //Get current time
// void loop() {
const size_t capacity = JSON_OBJECT_SIZE(2)+20;
DynamicJsonDocument doc(capacity);
DeserializationError error = deserializeJson(doc, nodemcu);
if (error) {
Serial.print(F("deserializeJson() failed: "));
Serial.println(error.c_str());
return;
}
int data1 = doc["data1"]; // 100
int data2 = doc["data2"]; // 200
serializeJson(doc, Serial);
Serial.println(data1,data2);
delay(500);
Serial.println();
}
