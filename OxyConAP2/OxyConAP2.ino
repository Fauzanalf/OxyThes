//----------------------------------------Include the NodeMCU ESP8266 Library
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
//----------------------------------------

#include "PageIndex.h" //--> Include the contents of the User Interface Web page, stored in the same folder as the .ino file

ESP8266WebServer server(80);  //--> Server on port 80

int DataReceive1;   //SpO2
int DataReceive2;   //O2
float DataReceive3; //LPM
int DataReceive4;   //HR
int DataReceive5;   //dummy

//float DataReceive1, DataReceive2, DataReceive3, DataReceive4, DataReceive5; //SpO2, O2, LPM, HR, 1

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
    DataReceive1=splitString(msg,';',0).toInt();
    DataReceive2=splitString(msg,';',1).toInt();
    DataReceive3=splitString(msg,';',2).toFloat();
    DataReceive4=splitString(msg,';',3).toInt();
    DataReceive5=splitString(msg,';',4).toInt();
  }
}

//----------------------------------------This routine is executed when you open NodeMCU ESP8266 IP Address in browser
void handleRoot() {
 String s = MAIN_page; //--> Read HTML contents
 server.send(200, "text/html", s); //--> Send web page
}
//----------------------------------------
//----------------------------------------Procedure for reading the SpO2
void handleOxthesSpO2() {
  getDataString();
  String SpO2_Value = String(DataReceive1);
 
  server.send(200, "text/plane", SpO2_Value); //--> Send SpO2 value only to client ajax request
  
}
//----------------------------------------
//----------------------------------------Procedure for reading the O2
void handleOxthesO2() {
  getDataString();
  String O2_Value = String(DataReceive2);
 
  server.send(200, "text/plane", O2_Value); //--> Send O2 value only to client ajax request
  
}
//----------------------------------------
//----------------------------------------Procedure for reading LPM
void handleOxythesLPM() {
  getDataString();
  String LPM_Value = String(DataReceive3);
 
  server.send(200, "text/plane", LPM_Value); //--> Send LPM value only to client ajax request
}
//----------------------------------------
//----------------------------------------Procedure for reading HR
void handleOxythesHR() {
  getDataString();
  String HR_Value = String(DataReceive4);
 
  server.send(200, "text/plane", HR_Value); //--> Send LPM value only to client ajax request
}
//----------------------------------------
//----------------------------------------Setup
void setup(void){
  Serial.begin(9600);
  delay(500);
    
  WiFi.softAP(ssid, password);  //--> Start Making ESP8266 NodeMCU as an access point

  IPAddress apip = WiFi.softAPIP(); //--> Get the IP server
  server.on("/", handleRoot); //--> Routine to handle at root location. This is to display web page.
  server.on("/readSpO2", handleOxthesSpO2);  //--> Routine to handle the call procedure
  server.on("/readO2", handleOxthesO2);  //--> Routine to handle the call procedure
  server.on("/readLPM", handleOxythesLPM);  //--> Routine to handle the call procedure
  server.on("/readHR", handleOxythesHR);  //--> Routine to handle the call procedure

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
