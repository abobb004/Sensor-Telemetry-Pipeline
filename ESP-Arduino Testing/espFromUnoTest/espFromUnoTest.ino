HardwareSerial unoSerial(2); // UART2

void setup() {
  Serial.begin(115200);                       // USB debug
  unoSerial.begin(9600, SERIAL_8N1, 16, 17);  // RX=16, TX=17
  Serial.println("ESP32 ready, listening...");
}

void loop() {
  if (unoSerial.available()) {
    uint8_t b = unoSerial.read();
    Serial.print("Received: 0x"); // As this is used with another testing file, it should print 0xAA
    if (b < 0x10) Serial.print("0");  // pad single digit hex
    Serial.println(b, HEX);
  }
}