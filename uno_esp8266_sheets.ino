//#include <PZEM004Tv30.h>
#include <ESP8266WiFi.h>
#include <HTTPSRedirect.h>
#include "DebugMacros.h"
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>

//D6 = Rx & D5 = Tx
SoftwareSerial nodemcu(D6, D5);


//**************************PZEM connection pins*********************************
//PZEM004Tv30 pzem(13,12); // (rx,tx) 12=D6,13=D7, (tx,rx) are connected to (TX,RX) of PZEM module
#define analogPin A0 /* ESP8266 Analog Pin ADC0 = A0 */
//*******************************************************************************
const size_t capacity = JSON_OBJECT_SIZE(6)+10;
DynamicJsonDocument doc(capacity);
//************************ Variables corresponding to NTP************************
const long utcOffsetInSeconds = 19800;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);
//float date_time;
//*******************************************************************************
//******************************Declaration of global variables******************
String sheetTime = "";
String sheetvolt = "";
String sheetcur  = "";
String sheetpowe = "";
String sheetener = "";
String sheetfreq = "";
String sheetpf   = "";
String adcstr1   = "";
String adcstr2   = "";
String adcstr3   = "";
String adcstr4   = "";
//String adcstr5   = "";
//String adcstr6   = "";
//String adcstr7   = "";
//String adcstr8   = "";

//*******************************************************************************

// *****************************WiFi SSID and Password***************************
const char* ssid = "satish";                
const char* password = "jgroup614";
//*******************************************************************************


// *****************************Spreadsheet initialization variables*************
const char* host = "script.google.com";
const char* GScriptId = "AKfycbycJIeRWQRhv-Dr6SCkzRofSpuK3SfFZ3ro_UGGzJ7i9mtoq-0";  // Google script ID 
const int httpsPort = 443;

String url = String("/macros/s/") + GScriptId + "/exec?value=Time";   // if the email id is not iith then remove /a/iith.ac.in
//String url2 = String("/macros/s/") + GScriptId + "/exec?cal";   // if the email id is not iith then remove /a/iith.ac.in

// temp_data is the sheet name

String payload_base =  "{\"command\": \"appendRow\", \  
                    \"sheet_name\": \"Sensor1\", \
                       \"values\": ";
String payload = "";
HTTPSRedirect* client = nullptr;
 

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(2, OUTPUT);

  Serial.begin(9600);
    nodemcu.begin(300);
  //***************************** Wifi connecting ******************************
      delay(500);
  Serial.print("Connecting to wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");  
  }
  //******************************************************************************

  //************ Use HTTPSRedirect class to create a new TLS connection**********
  client = new HTTPSRedirect(httpsPort);
  client->setInsecure();
  client->setPrintResponseBody(true);
  client->setContentTypeHeader("application/json");
  Serial.print("Connecting to ");
  Serial.println(host);          //try to connect with "script.google.com"

  //************ Try to connect for a maximum of 15 times then exit
  bool flag = false;
  for (int i = 0; i < 15; i++) {
    int retval = client->connect(host, httpsPort);
    if (retval == 1) {
      flag = true;
      break;
    }
    else
      Serial.println("Connection failed. Retrying...");
  }

  if (!flag) {
    Serial.print("Could not connect to server: ");
    Serial.println(host);
    Serial.println("Exiting...");
  Serial.println("Halting processor...");
  delete client;
  client = nullptr; 
  Serial.println("Restart after 30 seconds");
  Serial.printf("Final free heap: %u\n", ESP.getFreeHeap());
  Serial.printf("Final stack: %u\n", ESP.getFreeContStack());
  Serial.flush();
  ESP.deepSleep(30e6);
  }
// Finish setup() function in 1s since it will fire watchdog timer and will reset the chip.
//So avoid too many requests in setup()

  Serial.println("\nWrite into cell 'A1'");
  Serial.println("------>");
  // fetch spreadsheet data
  Serial.println(url);
  client->GET(url, host);
 
////  Serial.println("\nGET: Fetch Google Calendar Data:");
////  Serial.println("------>");
////  // fetch spreadsheet data
////  client->GET(url2, host);
//
// Serial.println("\nStart Sending pzem Data to Google Spreadsheet");

 
  // delete HTTPSRedirect object
  delete client;
  client = nullptr;
  timeClient.begin(); // NTP Time client for date and time
  
}

void loop() {

   DeserializationError error = deserializeJson(doc, nodemcu);
if (error) {
Serial.print(F("deserializeJson() failed: "));
Serial.println(error.c_str());
return;}
//else if (doc["data1"]!=0)
int data1 = doc["data1"]; // 100
int data2 = doc["data2"]; // 200
int data3 = doc["data3"]; // 100
int data4 = doc["data4"]; // 200
//int data5 = doc["data5"]; // 100
//int data6 = doc["data6"]; // 200
//int data7 = doc["data7"]; // 100
//int data8 = doc["data8"]; // 200
if (data1!=0 & data2!=0 & data3!=0& data4!=0){
//Serial.println(error.c_str());
//Serial.println();
//Serial.println(data1);

//
//
//
  timeClient.update();
//  date_time=timeClient.getEpochTime(); // Fetch Epoch time from NTP
   static int error_count = 0;
  static int connect_count = 0;
  const unsigned int MAX_CONNECT = 20;
  static bool flag = false;
  
// float adcValue=0.0;
// adcValue = analogRead(analogPin);


//
//  if (isnan(adcValue)) {
//    adcValue = 0;
// }

// Data formatting for sending data to strings spreadsheet
  sheetTime = String(timeClient.getFormattedTime());
 adcstr1=String(data1);
 adcstr2=String(data2);
  adcstr3=String(data3);
 adcstr4=String(data4);
//  adcstr5=String(data5);
// adcstr6=String(data6);
//  adcstr7=String(data7);
// adcstr8=String(data8);
  payload = payload_base+"\"" +sheetTime +"," + adcstr1 +"," + adcstr2 +"," +  adcstr3 +"," +  adcstr4+  "\"}";
  // +"," +  adcstr3 +"," +  adcstr4 +"," +  adcstr5 + "," + adcstr6 + "," + adcstr7 + adcstr8 +"," 
//Serial.println(adcstr2);
Serial.println();

//
//
// Data Updating in spreadhseet

 if (!flag) {
    client = new HTTPSRedirect(httpsPort);
    client->setInsecure();
    flag = true;
    client->setPrintResponseBody(true);
    client->setContentTypeHeader("application/json");
  }

  if (client != nullptr) {
    if (!client->connected()) {
      client->connect(host, httpsPort);
      client->POST(url, host, payload, false);
      Serial.print("Sent : ");  Serial.println("Temp and Humid");
         // Wait for two seconds
    }
  }
  else {
    DPRINTLN("Error creating client object!");
    error_count = 5;
  }

  if (connect_count > MAX_CONNECT) {
    connect_count = 0;
    flag = false;
    delete client;
    return;
  }
//
  Serial.println("POST or SEND Sensor data to Google Spreadsheet:");
  if (client->POST(url, host, payload)) {
    
                 digitalWrite(2, LOW);   // Turn the LED on by making the voltage LOW
  delay(500);            // Wait for a second
  digitalWrite(2, HIGH);  // Turn the LED off by making the voltage HIGH
  delay(500); 
    ;
  }
  else {
    ++error_count;
    Serial.print("Error-count while connecting: ");
    Serial.println(error_count);
  }

  if (error_count > 20) {
      digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on by making the voltage LOW
  delay(500);            // Wait for a second
  digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
  delay(500);            // Wait for two seconds
    Serial.println("Restart after 30 seconds");
  Serial.println("Halting processor...");
  delete client;
  client = nullptr;
  Serial.printf("Final free heap: %u\n", ESP.getFreeHeap());
  Serial.printf("Final stack: %u\n", ESP.getFreeContStack());
  Serial.flush();
  ESP.deepSleep(30e6);
  }
}
  delay(1000);

 
//  Serial.printf("%.1f V    %.3f A    %.1f W    %.3f kWh    %.2f Hz   %.2f pf\n",
//              volt, cur, powe, ener, freq, pf);
  //Serial.printf("%.3f A    %.1f kW    %.2f pf\n",
  //             cur, powe/1000, pf);
 
//  delay(5000);
}


//
//void setup() {
//  // Initialize Serial port
//  Serial.begin(9600);
//  nodemcu.begin(2400);
//  while (!Serial) continue;
//}
//
//void loop() {
//  //Get current time
//// void loop() {
//const size_t capacity = JSON_OBJECT_SIZE(2)+20;
//DynamicJsonDocument doc(capacity);
//DeserializationError error = deserializeJson(doc, nodemcu);
//if (error) {
//Serial.print(F("deserializeJson() failed: "));
//Serial.println(error.c_str());
//return;
//}
//int data1 = doc["data1"]; // 100
//int data2 = doc["data2"]; // 200
