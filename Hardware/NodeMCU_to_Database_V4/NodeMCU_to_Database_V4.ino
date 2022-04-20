
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <Wire.h>

#include <SoftwareSerial.h>
#include <ArduinoJson.h>

// Replace with your network credentials
const char* ssid     = "CALVIN-Student";
const char* password = "CITStudentsOnly";

// REPLACE with your Domain name and URL path or IP address with path
const char* serverName = "http://10.252.240.119:3000/formnext/Mobil1";


SoftwareSerial arduino(D6, D5); // Rx, Tx

int Counter = -1;
int httpResponseCode;
String payload;
int Counterr;
String IdUsed = "Mobil1";
String IdMobil;

HTTPClient http;
WiFiClient client;

void setup() {
  Serial.begin(115200);
  arduino.begin(9600);

  
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) { 
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  // (you can also pass in a Wire library object like &Wire2)
}
void loop() {
  //Check WiFi connection status
  if(WiFi.status()== WL_CONNECTED){
    // Your Domain name with URL path or IP address with path
    http.begin(client, serverName);
    
    // Specify content-type header
    http.addHeader("Content-Type", "application/json");
    
    
    httpResponseCode = http.GET();
    httpResponseCode = http.GET();
    payload = http.getString();
    payload.remove(0,1);
    payload.remove(payload.length()-1,1);

    Serial.println(payload);

//    String data = getValue(payload, ',', 0);
//    Serial.println(data);

    if (httpResponseCode == 200) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
//      StaticJsonDocument<250> doc;
      DynamicJsonDocument doc(1024);
//      deserializeJson(doc,payload);
//      deserializeJson(doc, data);

      DeserializationError error = deserializeJson(doc, payload);
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        return;
      }

      String IdMobil = doc["IdMobil"]; // "IEE"
      int Movement = doc["Movement"]; // "0"
      Counterr = doc["Sequence"]; // 8
      Serial.println(Counter);
      if (Counterr != Counter && IdMobil == IdUsed) {
        Serial.println("_______________________________________________________________________________________________________");
        Serial.print("IdMobil : ");
        Serial.println(IdMobil);
        Serial.print("Movement : ");
        Serial.println(Movement);
        Serial.print("Counterr : ");
        Serial.println(Counterr);
        Serial.println("\n");
        Serial.println("_______________________________________________________________________________________________________");
        Counter = Counterr;
        arduino.write(Movement);
      }
    }  
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();    
  
  }
  else {
    Serial.println("WiFi Disconnected");
  }
  
//  Serial.println("end");
//  Serial.println("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++");
  
  //Send an HTTP POST request every 1 seconds
  delay(1000);  
}


// http://saptaji.com/2020/11/09/cara-mudah-parsing-split-teks-string-data-di-arduino-nodemcu-esp8266/
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;
 
  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  } 
 
  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}
