HardwareSerial unoSerial(2); // UART2
int count = 0;
float temp = 0.0;
float humidity = 0.0;
uint8_t length;
uint8_t givenChecksum;
uint8_t checksum;
uint8_t packets[11];

void setup() {
  Serial.begin(115200);                       // USB debug
  unoSerial.begin(9600, SERIAL_8N1, 16, 17);  // RX=16, TX=17
  Serial.println("ESP32 ready, listening...");
}

void loop() {
  if(count == 11) { // If count is 11, that means the completed packet is sent through
    if(packets[0] == 0xAA) { // This is the start byte. If this isn't "OxAA," then the packet is wrong.
      // Setting variables with their value from the packet.
      length = packets[1];
      memcpy(&temp, &packets[2], 4);
      memcpy(&humidity, &packets[2+(length/2)], 4);
      givenChecksum = packets[10]; // This is the checksum from the packet. It will be used for a comparison.
      
      // Taking the data we got from the packet, a new checksum will be created. If data was not corrupted in
      // the travel from Arduino to ESP32, the checksums will match.
      checksum ^= packets[0];
      checksum ^= packets[1];
      for (int i = 2; i < 10; i++) {
        checksum ^= packets[i];
      }

      if(checksum == givenChecksum) { // Printing the data to from arduino. This allows easy comparison.
        Serial.print("Temperature = ");
        Serial.println(temp);
        Serial.print("Humidity = ");
        Serial.println(humidity);
        Serial.println();
      }
    }

    count = 0; // Resetting for new packet
    checksum = 0;
  }

  if(unoSerial.available()) {
    uint8_t b = unoSerial.read(); // Byte Data from arduino will be taken and put into a packet array.

    packets[count] = b;
    count++;
  }
}
