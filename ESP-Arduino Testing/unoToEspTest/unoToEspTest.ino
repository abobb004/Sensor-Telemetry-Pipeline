#include <SoftwareSerial.h>
SoftwareSerial espSerial(10, 11); // RX, TX

void setup() {
  Serial.begin(9600);       // USB debug
  espSerial.begin(9600);    // link to ESP32
}

void loop() {
  espSerial.write(0xAA); // This writes to the ESP
  Serial.println("Sent: AA"); // This indicates that the ESP was written to.
  delay(1000);
}
