#include <Wire.h>
#include "MAX30102_lib_intg.h"
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>


BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t value = 0;

int spo2;  //indicator to show if the SPO2 calculation is valid
float hr; //heart rate value
String dat_spo2;


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



void setup() {
  Serial.begin(115200);
  Wire.begin();


  // Create the BLE Device
  BLEDevice::init("PoxyThes");

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
  Serial.println("Waiting a client connection to notify...");
}

void loop() {
  //  particleSensor.heartrateAndOxygenSaturation(&spo2, &spo2_valid, &heart_rate, &hr_valid, &ratio, &correl);
  //  int spo2fix=round(spo2)+2;
  //  Serial.print("SpO2 = ");
  //  Serial.print(spo2fix, DEC);
  //  Serial.print(" // ");
  //  Serial.print("SpO2 valid = ");
  //  Serial.print(spo2_valid);
  //  Serial.print(" // ");
  //  Serial.print("HR = ");
  //  Serial.print(heart_rate, DEC);
  //  Serial.print(" // ");
  //  Serial.print("HR valid= ");
  //  Serial.println(hr_valid);
  //
  //  Serial.print("HR autocorrelation ratio = ");
  //  Serial.print(ratio);
  //  Serial.print(" // ");
  //  Serial.print("Pearson correlation = ");
  //  Serial.println(correl);
  String sendToESP;
  while (Serial.available()) {
    delay(100);
    sendToESP = Serial.readStringUntil('\n');
  }

  Serial.print("Data: ");
  Serial.println(sendToESP);
  //delay(10000);
  // notify changed value

  if (deviceConnected) {
    if (sendToESP != NULL) {
      char txString[16];

      sendToESP.toCharArray(txString, 16);

      pCharacteristic->setValue(txString);
      pCharacteristic->notify();

      Serial.print("Sent Data: ");
      Serial.println(txString);
      Serial.println("");
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
  delay(1000);
}
