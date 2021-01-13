# Realtime RC522 RFID Reader with Node.js and ESP8266

| Signal    | Pin |
|:---------:|:---:|
| RST/Reset | D3  |
| SPI SS    | D8  |
| SPI MOSI  | D7  |
| SPI MISO  | D6  |
| SPI SCK   | D5  |
<br>

![ESP8266 and RC522 pinout](https://miro.medium.com/max/780/0*rxWtG-74EjYyWFup.png)
<br>

# How To Run
1. Install and run the server first
```
npm install
node server.js
```
2. Enter your SSID, password and IP adress of your server inside `rfid.ino`
2. Build .ino file using PlatformIO Build
3. Upload the compiled file to ESP8266

**PS** : This project structure based on PlatformIO IDE for VS Code but you can also edit the .ino file using Arduino IDE and run node.js server separately