#include <Wire.h>
#include "BLEDevice.h"
#include "BLEScan.h"
#include <string.h>
#include <TFT_eSPI.h> // Graphics and font library for ILI9341 driver chip
#include <SPI.h>
#include <Stepper.h>

#define TFT_GREY 0x5AEB // New colour
TFT_eSPI tft = TFT_eSPI();  // Invoke library

unsigned long myTime;

const int stepsPerRevolution = 2048;  // change this to fit the number of steps per revolution

// ULN2003 Motor Driver Pins
#define IN1 33
#define IN2 25
#define IN3 26
#define IN4 27

// initialize the stepper library
Stepper myStepper(stepsPerRevolution, IN1, IN3, IN2, IN4);

#define RXD2 16
#define TXD2 17

int DataReceive1;
int DataReceive2;
int DataReceive3;
int DataO2x,DataO2xx,DataO2xxx, DataLPMx,DataLPMxx;
int DataO2Fixx;
float DataLPMFixx;

// The remote service we wish to connect to.
static BLEUUID serviceUUID("a18f3592-ec32-11eb-9a03-0242ac130003");
// The characteristic of the remote service we are interested in.
static BLEUUID    charUUID("beb5483e-36e1-4688-b7f5-ea07361b26a8");

static boolean doConnect = false;
static boolean connected = false;
static boolean doScan = false;
static BLERemoteCharacteristic* pRemoteCharacteristic;
static BLEAdvertisedDevice* myDevice;

static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) {

  char tempChars[16];
  strncpy(tempChars, (char*)pData, 16);
  char * strtokIndx; // this is used by strtok() as an index

  strtokIndx = strtok(tempChars, ";");     // get the first part - the string
  DataReceive1 = atoi(strtokIndx);

  strtokIndx = strtok(NULL, ";"); // this continues where the previous call left off
  DataReceive2 = atoi(strtokIndx);

  strtokIndx = strtok(NULL, ";"); // this continues where the previous call left off
  DataReceive3 = atoi(strtokIndx);
}

class MyClientCallback : public BLEClientCallbacks {
    void onConnect(BLEClient* pclient) {
    }

    void onDisconnect(BLEClient* pclient) {
      connected = false;
    }
};

bool connectToServer() {

  BLEClient*  pClient  = BLEDevice::createClient();

  pClient->setClientCallbacks(new MyClientCallback());

  // Connect to the remove BLE Server.
  pClient->connect(myDevice);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)

  // Obtain a reference to the service we are after in the remote BLE server.
  BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
  if (pRemoteService == nullptr) {
    pClient->disconnect();
    return false;
  }

  // Obtain a reference to the characteristic in the service of the remote BLE server.
  pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
  if (pRemoteCharacteristic == nullptr) {
    pClient->disconnect();
    return false;
  }

  // Read the value of the characteristic.
  if (pRemoteCharacteristic->canRead()) {
    std::string value = pRemoteCharacteristic->readValue();
  }

  if (pRemoteCharacteristic->canNotify())
    pRemoteCharacteristic->registerForNotify(notifyCallback);

  connected = true;
}
/**
   Scan for BLE servers and find the first one that advertises the service we are looking for.
*/
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
    /**
        Called for each advertising BLE server.
    */
    void onResult(BLEAdvertisedDevice advertisedDevice) {
      
      // We have found a device, let us now see if it contains the service we are looking for.
      if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(serviceUUID)) {

        BLEDevice::getScan()->stop();
        myDevice = new BLEAdvertisedDevice(advertisedDevice);
        doConnect = true;
        doScan = true;

      } // Found our server
    } // onResult
}; // MyAdvertisedDeviceCallbacks

void setup() {
  myStepper.setSpeed(10);
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
  tft.init();
  tft.setRotation(2);

  BLEDevice::init("");

  // Retrieve a Scanner and set the callback we want to use to be informed when we
  // have detected a new device.  Specify that we want active scanning and start the
  // scan to run for 5 seconds.
  BLEScan* pBLEScan = BLEDevice::getScan();
  pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
  pBLEScan->setInterval(1349);
  pBLEScan->setWindow(449);
  pBLEScan->setActiveScan(true);
  pBLEScan->start(5, false);

}

void loop() {
//  randOxy = random(85, 99);
//  randHR = random(75, 82);

  ocs3f();
  bluetooth();
  displayTFT();
  //control();
  
  delay(2000);
}

void bluetooth() {
  // If the flag "doConnect" is true then we have scanned for and found the desired
  // BLE Server with which we wish to connect.  Now we connect to it.  Once we are
  // connected we set the connected flag to be true.
  if (doConnect == true) {
    if (connectToServer()) {
    } 
    else {
    }
    doConnect = false;
  }
}

void displayTFT() {
  tft.fillScreen(TFT_BLACK);
  
//  int randOxy = random(90, 95);
//  int randHR = random(75, 82);
//  float randPress = random(1, 2);
//  float randLPM = random(1, 3);

  tft.setCursor(5, 25);
  tft.setTextColor(TFT_YELLOW);
  tft.setTextFont(4);
  tft.print("O2       : ");
  tft.setTextFont(7);
  tft.print(DataO2Fixx);
  tft.setTextFont(4);
  tft.println("%");

  tft.setCursor(5, 100);
  tft.setTextColor(TFT_GREEN);
  tft.setTextFont(4);
  tft.print("SpO2 : "); tft.print(DataReceive1); tft.println("%");
  tft.print(" HR       : "); tft.print(DataReceive2); tft.println("BPM");
  
//  tft.print("SpO2 : "); tft.print(randOxy); tft.println("%");
//  tft.print(" HR       : "); tft.print(randHR); tft.println("BPM");

  tft.setCursor(5, 180);
  tft.setTextColor(TFT_YELLOW);
  tft.setTextFont(4);
  tft.print(" Flow          : "); tft.print(DataLPMFixx,2);tft.println("LPM");

  String DataSerial = "";
  DataSerial += DataReceive1;
  DataSerial += ";";
  DataSerial += DataO2Fixx;
  DataSerial += ";";
  DataSerial += DataLPMFixx;
  DataSerial += ";";
  DataSerial += DataReceive2;
  DataSerial += ";";
  DataSerial += 1;
  DataSerial += '\0';
  Serial.println(DataSerial);
}

void ocs3f() {
  typedef unsigned char u8;
  typedef unsigned int u16;
  int inByte;
  u8 temp;
  u8 i, j, o2[12];
  u16 o2c, o2f, o2t; //Define oxygen concentration, flow rate and temperature
  // when characters arrive over the serial port...
  if (Serial2.available()) {

    // wait a bit for the entire message to arrive
    delay(200);

    // read all the available characters
    while (Serial2.available() > 0) {

      inByte = Serial2.read();
      
      //---Receiving part---
      if ((o2[0] == 0x16) && (o2[1] == 0x09) && (o2[2] == 0x01)) //Determine if the first two bytes are received correctly, I is the global variable
      {
        o2[i] = inByte;       
        i++;
      }
      else           //If one of the first three bytes received is incorrect, the first two bytes will be judged
      {
        if ((o2[0] == 0x16) && (o2[1] == 0x09))
        {
          if ( inByte == 0x01)   
          {
            o2[2] =  inByte;   
            i++;
          }
          else                                
          {
            i = 0;                
            for (j = 0; j < 12; j++)          
            {
              o2[j] = 0;
            }
          }
        }
        else     
        {
          if (o2[0] == 0x16)
          {
            if ( inByte == 0x09)
            {
              o2[1] =  inByte; 
              i++;
            }
            else        
            {
              i = 0;                
              for (j = 0; j < 12; j++)          
              {
                o2[j] = 0;
               }
            }
          }
          else     
          {
            if ( inByte == 0x16) 
            {
              o2[0] =  inByte; 
              i++;
            }
            else         
            {
              i = 0;                 
              for (j = 0; j < 12; j++)         
              {
                o2[j] = 0;
              }
            }
          }
        }
      }
     //---Receiving part---



      if (i == 12)   //Data received complete, start calibration
      {
        temp = 0;
        for (j = 0; j < 12; j++)
        {
          temp += o2[j];
        }
        if (temp == 0)     //Check passed, calculate oxygen concentration, flow, temperature value
        {
          o2c = o2[3] * 256 + o2[4];     //Oxygen concentration
          o2f = o2[5] * 256 + o2[6];     //Oxygen flow value
          o2t = o2[7] * 256 + o2[8];     //Oxygen temperature
        }

      

        i = 0;                             
        for (j = 0; j < 12; j++)           //Initialize array
        {
          o2[j] = 0;
        }
        
        DataO2x=o2c/100;
        DataO2xx=o2c/10%10;
        DataO2xxx=o2c%10;
        DataLPMx=o2f/10%10;
        DataLPMxx=o2f%10;

        String DataO2Fix = "";
        DataO2Fix += DataO2x;
        DataO2Fix += DataO2xx;
        DataO2Fix += ".";
        DataO2Fix += DataO2xxx;
      
        String DataLPMFix = "";
        DataLPMFix += DataLPMx;
        DataLPMFix += ".";
        DataLPMFix += DataLPMxx;

        DataO2Fixx=DataO2Fix.toInt();
        float x;
        x = DataLPMFix.toFloat();
        //DataLPMFixx = x;
        DataLPMFixx = (-0.0021*x*x)+(0.8605*x)+0.0111;
        
        //Serial.print(DataO2Fix+";"+DataLPMFix+";"+"1"+";"+'\n');
      }
    }
  }
}

void control(){
  if(DataReceive1 == NULL || DataReceive1 == 0){
    decreaseLPM();
  }
  
  else if(DataReceive1 > 0 && DataReceive1 < 94){
    increaseLPM();
  }

  else if(DataReceive1 >= 94 && DataLPMFixx > 3.6){
    decreaseLPM();
    waitLoop();
  }

  else if(DataReceive1 >= 94 && DataLPMFixx <= 3.6){
    waitLoop();
    if(DataReceive1 >= 94){
      decreaseLPM();
    }
    else{
      myStepper.step(650);
    }
  }
  
  else{
    waitLoop();
  }
}


void increaseLPM(){
  float DataLPM;
  DataLPM = DataLPMFixx + 2;
  while(DataLPMFixx <= DataLPM && DataLPMFixx < 3.90){
    if(DataLPMFixx >= 3.90){
      break;
    }
    myStepper.step(200);
    delay(900);
    ocs3f();
    displayTFT();
    delay(900);
    ocs3f();
    bluetooth();
    displayTFT();
  }
}

void decreaseLPM(){
  float DataLPM;
  DataLPM = DataLPMFixx - 0.5;
  while(DataLPMFixx >= DataLPM && DataLPMFixx > 0.2){
    if(DataReceive1 < 94){
      break;
    }
    myStepper.step(-100);
    delay(900);
    ocs3f();
    displayTFT();
    delay(900);
    ocs3f();
    bluetooth();
    displayTFT();
  }
}

void waitLoop(){
  for(int i=0;i<15;i++){
    if(DataReceive1 < 94){
      i=14;
    }
    delay(900);
    ocs3f();
    displayTFT();
    delay(900);
    ocs3f();
    bluetooth();
    displayTFT();
  }
}
