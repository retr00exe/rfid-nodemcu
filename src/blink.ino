/* Node.js Server for RC522 RFID Reader using NodeMCU ESP8266 and Arduino Uno R3
 *
 * Created by Mekel Ilyasa
 * Source available at https://github.com/retr00exe/nodejs-rfid-reader
 *
 */

#include <ESP8266WiFi.h>
#include <MFRC522.h>
#include <SPI.h>
#include <WebSocketClient.h>

#define RST_PIN D3
#define SDA_PIN D4

boolean handshakeFailed = 0;
String data = "";
char PATH[] = "/";
const char* SSID = "n0psledbyte";
const char* PASSWORD = "password12345";
char* HOST = "192.168.100.3";
const int PORT = 4000;

WebSocketClient webSocketClient;
WiFiClient client;
MFRC522 mfrc522(SDA_PIN, RST_PIN);

unsigned long previousMillis = 0;
unsigned long currentMillis;
unsigned long interval = 300;

void setup() {
  Serial.begin(115200);
  SPI.begin();
  Serial.println();
  Serial.printf("Connecting to %s", SSID);

  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.print("IP address : ");
  Serial.println(WiFi.localIP());
  delay(1000);
  wsconnect();

  mfrc522.PCD_Init();
  Serial.println("Put your card to the reader...");
}

void loop() {
  if (client.connected()) {
    if (!mfrc522.PICC_IsNewCardPresent()) {
      return;
    }
    if (!mfrc522.PICC_ReadCardSerial()) {
      return;
    }

    String rfid = "";
    byte letter;
    for (byte i = 0; i < mfrc522.uid.size; i++) {
      rfid.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? "0" : ""));
      rfid.concat(String(mfrc522.uid.uidByte[i], HEX));
    }
    Serial.println(rfid);
    currentMillis = millis();
    webSocketClient.getData(data);

    if (data.length() > 0) {
      Serial.println(data);
      if (abs(currentMillis - previousMillis) >= interval) {
        previousMillis = currentMillis;
        webSocketClient.sendData(rfid);
      }
    }
  }
}

void wsconnect() {
  if (client.connect(HOST, PORT)) {
    Serial.println("Connected");
  } else {
    Serial.println("Connection failed");
    delay(2000);
    if (handshakeFailed) {
      handshakeFailed = 0;
      ESP.restart();
    }
    handshakeFailed = 1;
  }

  webSocketClient.path = PATH;
  webSocketClient.host = HOST;
  if (webSocketClient.handshake(client)) {
    Serial.println("Handshake successful");
  } else {
    Serial.println("Handshake failed");
    delay(2000);
    if (handshakeFailed) {
      handshakeFailed = 0;
      ESP.restart();
    }
    handshakeFailed = 1;
  }
}