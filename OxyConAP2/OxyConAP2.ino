//----------------------------------------Include the NodeMCU ESP8266 Library
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
//----------------------------------------

#include "PageIndex.h" //--> Include the contents of the User Interface Web page, stored in the same folder as the .ino file

ESP8266WebServer server(80);  //--> Server on port 80

float DataReceive1, DataReceive2, DataReceive3;

//----------------------------------------Make a wifi name and password as access point
const char* ssid = "Oxythes";
const char* password = "";
//----------------------------------------


//----------------------------------------
void getDataString() {
  if(Serial.available()){
    delay(100);
    String msg = "";
    while(Serial.available()){
      msg += Serial.readStringUntil('\n');
    }
//    Serial.print(msg);
    DataReceive1=splitString(msg,';',0).toFloat();
    DataReceive2=splitString(msg,';',1).toFloat();
    DataReceive3=splitString(msg,';',2).toInt();
//    DataReceive4=splitString(msg,';',3);
  }
}

//----------------------------------------This routine is executed when you open NodeMCU ESP8266 IP Address in browser
void handleRoot() {
 String s = MAIN_page; //--> Read HTML contents
 server.send(200, "text/html", s); //--> Send web page
}
//----------------------------------------
//----------------------------------------Procedure for reading the temperature value of a DHT11 sensor
void handleOxthesSpO2() {
  getDataString();
  //float t = dht.readTemperature();  //--> Read temperature as Celsius (the default). If you want to read the temperature sensor in Fahrenheit, use this -> float t = dht.readTemperature (true);
  String SpO2_Value = String(DataReceive1);
 
  server.send(200, "text/plane", SpO2_Value); //--> Send Temperature value only to client ajax request
  
}
//----------------------------------------
//----------------------------------------Procedure for reading humidity values from DHT11 sensors
void handleOxythesLPM() {
  getDataString();
  //float h = dht.readHumidity();
  String LPM_Value = String(DataReceive2);
 
  server.send(200, "text/plane", LPM_Value); //--> Send Humidity value only to client ajax request
}
//----------------------------------------
//----------------------------------------Setup
void setup(void){
  Serial.begin(9600);
  delay(500);
    
  WiFi.softAP(ssid, password);  //--> Start Making ESP8266 NodeMCU as an access point

  IPAddress apip = WiFi.softAPIP(); //--> Get the IP server
  server.on("/", handleRoot); //--> Routine to handle at root location. This is to display web page.
  server.on("/readSpO2", handleOxthesSpO2);  //--> Routine to handle the call procedure handleDHT11Temperature
  server.on("/readLPM", handleOxythesLPM);  //--> Routine to handle the call procedure handleDHT11Humidity

  server.begin(); //--> Start server
}
//----------------------------------------
//----------------------------------------Loop
void loop(){
  server.handleClient();  //Handle client requests
}
//----------------------------------------

String splitString(String data, char separator, int index)
{
    int found = 0;
    int strIndex[] = { 0, -1 };
    int maxIndex = data.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }
    return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}
