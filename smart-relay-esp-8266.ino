#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "fauxmoESP.h"

#define RELAY_PIN 0
#define WIFI_SSID ""
#define WIFI_PASS ""
#define RELAY "DEVICE NAME"

#define SERIAL_BAUDRATE 115200

fauxmoESP fauxmo;

void wifiSetup() {

  // Define as STA
  WiFi.mode(WIFI_STA);

  // Connect
  Serial.printf("[WIFI] SSID %s ", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  // Wait
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();

  // Connected
  Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
}

void amazonEchoSetup() {
  fauxmo.createServer(true);
  fauxmo.setPort(80);
  fauxmo.enable(true);

  fauxmo.addDevice(RELAY);

  fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
    if ( (strcmp(device_name, RELAY) == 0) ) {
      if (state) {
        digitalWrite(RELAY_PIN, HIGH);
      } else {
        digitalWrite(RELAY_PIN, LOW);
      }
    }    
  });  
}

void setup() {
  Serial.begin(SERIAL_BAUDRATE);

  wifiSetup();
  amazonEchoSetup();

  pinMode(RELAY_PIN,OUTPUT);
  digitalWrite(RELAY_PIN,LOW);

  
}

void loop() {
  fauxmo.handle();
}
