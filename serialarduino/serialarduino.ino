#define RXD2 16
#define TXD2 17

void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);
}

void loop() {
  while(Serial2.available()){
    String data=Serial2.readStringUntil('\n');
    Serial.println(data);
  }
  delay(200);
}
