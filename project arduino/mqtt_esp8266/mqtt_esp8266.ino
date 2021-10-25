#include "EspMQTTClient.h"

EspMQTTClient client(
  "ICHIRO UNIFI 2.4G",
  "icH12o2021",
  "103.56.148.12",  // MQTT Broker server ip
  "nathan",   // Can be omitted if not needed
  "icH12o",   // Can be omitted if not needed
  "TestClient",      // Client name that uniquely identify your device
  1883
);

void setup() {}

void onConnectionEstablished() {

  client.subscribe("mytopic/test", [] (const String &payload)  {
    Serial.println(payload);
  });

  client.publish("mytopic/test", "This is a message");
}

void loop() {
  client.loop();
}
