#include <TFT_eSPI.h>
#include <SPI.h>
#include <Wire.h>
#include "MAX30102_lib_intg.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

TFT_eSPI tft = TFT_eSPI();

#define BLUE      0x001F
#define TEAL      0x0438
#define GREEN     0x07E0
#define CYAN      0x07FF
#define RED       0xF800
#define MAGENTA   0xF81F
#define YELLOW    0xFFE0
#define ORANGE    0xFC00
#define PINK      0xF81F
#define PURPLE    0x8010
#define GREY      0xC618
#define WHITE     0xFFFF
#define BLACK     0x0000

#define selectButton 35
#define downButton 0
int pilihan = 1;
int modepilihan = 1;
int spo2Data, hrData;

// Global variables
  int valueBlock[500];
  int timeBlock[500];
  int locationBlock[500];
  int valuePos;
  int blockPos;
  
// Editable Variables

  uint16_t graphColor = GREEN;
  uint16_t pointColor = WHITE;
  uint16_t lineColor = PURPLE;

  int graphRange = 100;
  int markSize = 3;

// Calculate Values
  const int numberOfMarks = 5;
  const int originX = 30;
  const int originY = 120;
  const int sizeX = 190;
  const int sizeY = 30;
  const int deviation = 10;
  
  int boxSize = (sizeX / numberOfMarks);
  int mark[] = {(boxSize + deviation), ((boxSize * 2) + deviation), ((boxSize * 3) + deviation), ((boxSize * 4) + deviation), ((boxSize * 5) + deviation)};

void drawGraph()
{  
  // draw outline
  tft.drawLine(originX, originY, (originX + sizeX), originY, graphColor);
  tft.drawLine(originX, originY, originX, (originY - sizeY), graphColor);

}

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t value = 0;

MAX30102_LIB_INTG particleSensor;

uint32_t irBuffer[100]; //infrared LED sensor data
uint32_t redBuffer[100];  //red LED sensor data

double spo2,ratio,correl;  //SPO2 value
int8_t spo2_valid;  //indicator to show if the SPO2 calculation is valid
int32_t heart_rate; //heart rate value
int8_t  hr_valid;  //indicator to show if the heart rate calculation is valid

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID        "a18f3592-ec32-11eb-9a03-0242ac130003"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"


class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

void graph()
{
  int temp = heart_rate;
  timeBlock[valuePos] = ((millis() - 4500) / 1000);

  valueBlock[valuePos] = temp;
  
  
  if(blockPos < 5)
  {
    
    // map the value
    locationBlock[valuePos] = map(temp, 0, graphRange, originY, (originY - sizeY));

    // draw point
    tft.fillRect((mark[valuePos] - 1), (locationBlock[valuePos] - 1), markSize, markSize, pointColor);

    // try connecting to previous point
    if(valuePos != 0)
    {
      tft.drawLine(mark[valuePos], locationBlock[valuePos], mark[(valuePos - 1)], locationBlock[(valuePos - 1)], lineColor);
    }

    blockPos++;
  }
  else
  {
    // clear the graph's canvas
    tft.fillRect((originX + 2), (originY - sizeY), sizeX, sizeY, TFT_BLACK);

    // map the value - current point
    locationBlock[valuePos] = map(temp, 0, graphRange, originY, (originY - sizeY));

    // draw point - current point
    tft.fillRect((mark[4]), (locationBlock[valuePos] - 1), markSize, markSize, pointColor);

    // draw all points
    for(int i = 0; i < 5; i++)
    {
      tft.fillRect((mark[(blockPos - (i + 1))] - 1), (locationBlock[(valuePos - i)] - 1), markSize, markSize, pointColor);
    }

    // draw all the lines
    for(int i = 0; i < 4; i++)
    {
      tft.drawLine(mark[blockPos - (i + 1)], locationBlock[valuePos - i], mark[blockPos - (i + 2)], locationBlock[valuePos - (i + 1)], lineColor);
    }
  }

  valuePos++;
}

void setup() {
  Serial.begin(115200);
  Wire.begin(21, 22);
  pinMode(downButton, INPUT_PULLUP);
  pinMode(selectButton, INPUT_PULLUP);
  tft.begin();
  tft.setRotation(1); //Landscape
  
  updateMenu();
  delay(100);

  while (!particleSensor.begin()) {
    Serial.println("MAX30102 was not found");
    delay(1000);
  }
  particleSensor.sensorConfiguration(/*ledBrightness=*/60, /*sampleAverage=*/SAMPLEAVG_8, \
                                  /*ledMode=*/MODE_MULTILED, /*sampleRate=*/SAMPLERATE_200, \
                                  /*pulseWidth=*/PULSEWIDTH_411, /*adcRange=*/ADCRANGE_16384);

  // Create the BLE Device
  BLEDevice::init("Pasien1");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic(
                      CHARACTERISTIC_UUID,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY |
                      BLECharacteristic::PROPERTY_INDICATE
                    );

  // https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.gatt.client_characteristic_configuration.xml
  // Create a BLE Descriptor
  pCharacteristic->addDescriptor(new BLE2902());

  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
  pAdvertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  //Serial.println("Waiting a client connection to notify...");
}

void loop(){
  if (!digitalRead(downButton)){
    pilihan++;
    updateMenu();
    delay(100);
    while (!digitalRead(downButton));
  }
  if (!digitalRead(selectButton)){
    updateMenu();
    executeAction();
    delay(100);
    while (!digitalRead(selectButton));
  }
}

void updateMenu() {
  int xpos = 0;
  int ypos = 35;
  switch (pilihan) {
    case 0:
      pilihan = 1;
      break;
    case 1:
      tft.fillScreen(TFT_BLACK);
      tft.setTextColor(TFT_GREEN, TFT_BLACK);
      tft.setCursor(xpos,ypos);
      tft.setFreeFont(&Orbitron_Light_24);
      tft.println("Selamat Datang");
      tft.println("Pilih Menu:");
      tft.println(">Mode Normal");
      tft.println(" Mode Data");
      break;
    case 2:
      tft.fillScreen(TFT_BLACK);
      tft.setTextColor(TFT_GREEN, TFT_BLACK);
      tft.setCursor(xpos,ypos);
      tft.setFreeFont(&Orbitron_Light_24);
      tft.println("Selamat Datang");
      tft.println("Pilih Menu:");
      tft.println(" Mode Normal");
      tft.println(">Mode Data");
      break;
    case 3:
      pilihan = 0;
      break;
  }
}

void executeAction() {
  switch (pilihan) {
    case 1:
      action1();
      break;
    case 2:
      action2();
      break;
  }
}

void action1() {
  for(int i=1;i<15;i++){
    particleSensor.heartrateAndOxygenSaturation(&spo2, &spo2_valid, &heart_rate, &hr_valid, &ratio, &correl);
    int spo2fix=round(spo2)+2;
    Serial.print("SpO2 = ");
    Serial.print(spo2fix, DEC);
    Serial.print(" // ");
    Serial.print("SpO2 valid = ");
    Serial.print(spo2_valid);
    Serial.print(" // ");
    Serial.print("HR = ");
    Serial.print(heart_rate, DEC);
    Serial.print(" // ");
    Serial.print("HR valid= ");
    Serial.println(hr_valid);
    
    Serial.print("HR autocorrelation ratio = ");
    Serial.print(ratio);
    Serial.print(" // ");
    Serial.print("Pearson correlation = ");
    Serial.println(correl);

    tft.fillScreen(TFT_BLACK);
    tft.fillRect(0, 0, 250, 100, TFT_BLACK);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setCursor(0, 30);
    tft.setFreeFont(&Orbitron_Light_24);
    tft.println("SpO2         : ");
    tft.drawLine(0, 35, 250, 35, TFT_BLUE);
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.setCursor(0, 60);
    tft.setFreeFont(&Orbitron_Light_24);
    tft.println("HeartRate : ");
    tft.drawLine(0, 70, 250, 70, TFT_BLUE);
    drawGraph();
    
    if (spo2_valid == 1 && hr_valid == 1){
      tft.setTextColor(TFT_PURPLE, TFT_BLACK);
      tft.setCursor(160, 30);
      tft.print(spo2fix);tft.print(F("%"));
      tft.setTextColor(TFT_PURPLE, TFT_BLACK);
      tft.setCursor(160, 60);
      tft.print(heart_rate);
      graph();
    }
  
    String sendToESP = "";
      sendToESP += spo2fix;
      sendToESP += ";";
      sendToESP += heart_rate;
      sendToESP += ";";
      sendToESP += 1;
      sendToESP += '\0';
      
    Serial.println(sendToESP);
      
    // notify changed value
    if (deviceConnected) {
      if (spo2_valid == 1 && hr_valid == 1){
        char txString[16];
        sendToESP.toCharArray(txString,16);
     
        pCharacteristic->setValue(txString);
        pCharacteristic->notify();
  
        Serial.print("*** Sent Data: ");
        Serial.print(txString);
        Serial.println("***");
        delay(500); // bluetooth stack will go into congestion, if too many packets are sent, in 6 hours test i was able to go as low as 3ms
      }
    }
    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
    }
  }
}

void action2(){
  
  for(int i=1; i<8; i++){
    spo2Data=92;
    BLEData();
    delay(14500);
  }
  
  for(int i=1; i<3; i++){
    spo2Data=93;
    BLEData();
    delay(14500);
  }
  
  for(int i=1; i<6; i++){
    spo2Data=94;
    BLEData();
    delay(14500);
  }
  
  for(int i=1; i<3; i++){
    spo2Data=93;
    BLEData();
    delay(14500);
  }
  
  for(int i=1; i<6; i++){
    spo2Data=94;
    BLEData();
    delay(14500);
  }
  
  spo2Data=95;
  BLEData();
  delay(14500);

  spo2Data=94;
  BLEData();
  delay(14500);

  spo2Data=95;
  BLEData();
  delay(14500);

  spo2Data=94;
  BLEData();
  delay(14500);
  
  for(int i=1; i<31; i++){
    spo2Data=95;
    BLEData();
    delay(14500);
  }
}

void BLEData(){
  hrData=random(60, 100);
  tft.fillScreen(TFT_BLACK);
  tft.fillRect(0, 0, 250, 100, TFT_BLACK);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setCursor(0, 30);
  tft.setFreeFont(&Orbitron_Light_24);
  tft.println("SpO2         : ");
  tft.drawLine(0, 35, 250, 35, TFT_BLUE);
  tft.setTextColor(TFT_GREEN, TFT_BLACK);
  tft.setCursor(0, 60);
  tft.setFreeFont(&Orbitron_Light_24);
  tft.println("HeartRate : ");
  tft.drawLine(0, 70, 250, 70, TFT_BLUE);
  tft.setTextColor(TFT_PURPLE, TFT_BLACK);
  tft.setCursor(160, 30);
  tft.print(spo2Data);tft.print(F("%"));
  tft.setTextColor(TFT_PURPLE, TFT_BLACK);
  tft.setCursor(160, 60);
  tft.print(hrData);

  for(int j=1; j<8; j++){
  Serial.print("SpO2 :");
  Serial.println(spo2Data);
  }
  
  String sendToESP = "";
  sendToESP += spo2Data;
  sendToESP += ";";
  sendToESP += hrData;
  sendToESP += ";";
  sendToESP += 1;
  sendToESP += '\0';
    
  //Serial.println(sendToESP);
    
  // notify changed value
  if (deviceConnected) {
    char txString[16];
    sendToESP.toCharArray(txString,16);
 
    pCharacteristic->setValue(txString);
    pCharacteristic->notify();

//    Serial.print("*** Sent Data: ");
//    Serial.print(txString);
//    Serial.println("***");
    delay(500); // bluetooth stack will go into congestion, if too many packets are sent, in 6 hours test i was able to go as low as 3ms
  }
  // disconnecting
  if (!deviceConnected && oldDeviceConnected) {
      delay(500); // give the bluetooth stack the chance to get things ready
      pServer->startAdvertising(); // restart advertising
//      Serial.println("start advertising");
      oldDeviceConnected = deviceConnected;
  }
  // connecting
  if (deviceConnected && !oldDeviceConnected) {
      // do stuff here on connecting
      oldDeviceConnected = deviceConnected;
  }
}
