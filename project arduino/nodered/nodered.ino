L#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

//const char* ssid = "ICHIRO UNIFI 2.4G";
//const char* password = "icH12o2021";
const char* ssid = "Lantai2 lorong 1";
const char* password = "56789012";
const char* mqtt_server = "103.56.148.12";


TinyGPSPlus gps;  // The TinyGPS++ object
SoftwareSerial ss(4, 5); // The serial connection to the GPS device
float latitude , longitude;
String lat_str , lng_str;

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;

StaticJsonDocument<256> doc;
char out[128];

void setup() {
  pinMode(D3, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  ss.begin(9600);
  doc["latitude"] = -1.0;
  doc["longitude"] = -1.0;
}

void setup_wifi() {

  delay(10);
  // connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if ((char)payload[0] == '0') {
     Serial.println("LOW");
    digitalWrite(D3, LOW);   // Turn the LED on 
  } 

 if ((char)payload[0] == '1') {
    Serial.println("HIGH");
    digitalWrite(D3, HIGH);  // Turn the LED off 
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      client.subscribe("event");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if (ss.available() > 0)
  {
    if (gps.encode(ss.read()))
    {
      if (gps.location.isValid())
      {
        latitude = gps.location.lat();
        lat_str = String(latitude , 6);
//        Serial.println(lat_str);
        doc["latitude"] = latitude;
        
        longitude = gps.location.lng();
        lng_str = String(longitude , 6);
//        Serial.println(lng_str);
        doc["longitude"] = longitude;
      }
    }
  }
  serializeJson(doc, out);
  

  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    client.publish("event1", out);
  }
}
