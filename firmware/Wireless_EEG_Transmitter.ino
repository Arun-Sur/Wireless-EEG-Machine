#include <WiFi.h>
#include <WebSocketsServer.h>

const char* ssid = "asdf";
const char* password = "oooo9999";

WebSocketsServer webSocket = WebSocketsServer(81);
const int eegPin = 34;  // Your EEG analog input
unsigned long lastSample = 0;
const int sampleInterval = 4;  // 4ms = 250Hz

void setup() {
  Serial.begin(115200);
  analogReadResolution(12);  // 0–4095
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(WiFi.localIP());

  webSocket.begin();
  webSocket.onEvent([](uint8_t n, WStype_t t, uint8_t * p, size_t l) {});
}

void loop() {
  webSocket.loop();

  unsigned long now = micros();
  if (now - lastSample >= 4000) {  // Every 4 ms
    lastSample = now;
    int val = analogRead(eegPin);
    String payload = String(val);
    webSocket.broadcastTXT(payload);
  }
}
