#include <dht.h>
#include <SoftwareSerial.h>

dht DHT;
SoftwareSerial espSerial(10, 11); // RX, TX

#define DHT11_PIN 7

void setup(){
  Serial.begin(9600);
  espSerial.begin(9600);
}

void loop(){
  int chk = DHT.read11(DHT11_PIN);
  uint8_t packets[11]; // Packet size is 11

  if(chk == 0) {
    // Packet Creation
    packets[0] = 0xAA; // Start byte 1
    packets[1] = 8; // Length of payload is 8
    memcpy(&packets[2], &DHT.temperature, 4); // Temp is 4 bytes
    memcpy(&packets[6], &DHT.humidity, 4); // Humidity is 4 bytes

    // Creation of checksum which will be compared in the espSensorTelemetry
    uint8_t checksum = 0;
    checksum ^= packets[0];
    checksum ^= packets[1];
    for (int i = 2; i < 10; i++) {
      checksum ^= packets[i];
    }
    packets[10] = checksum; // The last byte of packet is the checksum

    // For the user: data regarding the temperature and humidity. 
    // This will be used to compare the data sent from arduino to esp. (The temperature there and here should match)
    Serial.print("Temperature = ");
    Serial.println((DHT.temperature)); // This is in celsius
    Serial.print("Humidity = ");
    Serial.println(DHT.humidity);
    Serial.println();

    for(int i = 0; i < 11; i++) { // Sending the data from arduino to esp
      espSerial.write(packets[i]);
    }

    // Debug Info
    // Serial.println("Bytes");
    // for(int i = 0; i < 11; i++) {
      // Serial.println(packets[i]);
    // }
    // Serial.println("done");
  } else {
    packets[0] = 0x00; 
  }

  delay(1000);
}
