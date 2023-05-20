//Arduino side code
//DHT11 Lib
//#include <DHT.h>

//Arduino to NodeMCU Lib
//#include <SoftwareSerial.h>
#include <ArduinoJson.h>

//Initialise Arduino to NodeMCU (5=Rx & 6=Tx)
//SoftwareSerial nodemcu(5, 6);

//Initialisation of DHT11 Sensor
#define pn0 0           // analog 8 (output voltage with offset)
#define pn1 1                  // analog 9 (offset voltage)
#define pn2 2           // analog 8 (output voltage with offset)
#define pn3 3                  // analog 9 (offset voltage)
//#define pn4 4           // analog 8 (output voltage with offset)
//#define pn5 5                  // analog 9 (offset voltage)
//#define pn6 6           // analog 8 (output voltage with offset)
//#define pn7 7                  // analog 9 (offset voltage)
//DHT dht(DHTPIN, DHT11);
float temp;
float hum;

void setup() {
  Serial.begin(9600);
//analogReference(12);
//  dht.begin();
  Serial3.begin(300);
  delay(1000);

  Serial.println("Program started");
}

void loop() {

//  StaticJsonDocument<1000> doc;

  //Obtain Temp and Hum data
//  dht11_func();

  //Assign collected data to JSON Object
const size_t capacity = JSON_OBJECT_SIZE(6);
DynamicJsonDocument doc(capacity);
  int pnO0 = analogRead(pn0)*(33000/1024)*0.1;
//    delay(10);
  int pnO1 = analogRead(pn1)*(33000/1024)*0.1;
//  delay(10);

    int pnO2 = analogRead(pn2)*(33000/1024)*0.1;
//  delay(10);

  int pnO3 = analogRead(pn3)*(33000/1024)*0.1;
//  delay(10);
//
//    int pnO4 = analogRead(pn4)*(33000/1024)*0.1;
//  delay(10);
//
//  int pnO5 = analogRead(pn5)*(33000/1024)*0.1;
//  delay(10);
//
//  int pnO6 = analogRead(pn6)*(33000/1024)*0.1;
//  delay(10);
//
//  int pnO7 = analogRead(pn7)*(33000/1024)*0.1;
doc["data1"] = pnO0;
doc["data2"] = pnO1;
doc["data3"] = pnO2;
doc["data4"] = pnO3;
//doc["data5"] = pnO4;
//doc["data6"] = pnO5;
//doc["data7"] = pnO6;
//doc["data8"] = pnO7;

  //Send data to NodeMCU
  serializeJson(doc, Serial3);
  delay(3000);
}

//void dht11_func() {
//
//  hum = 5;
//  temp = 6;
//  Serial.print("Humidity: ");
//  Serial.println(hum);
//  Serial.print("Temperature: ");
//  Serial.println(temp);

//}
