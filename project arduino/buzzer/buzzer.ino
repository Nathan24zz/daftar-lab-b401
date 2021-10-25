#include <ESP8266WiFi.h> 
#include <WiFiClient.h> 
#include <ESP8266WebServer.h> 
ESP8266WebServer server(80);   
const int buz_pin = D2;
const char* ssid = "Lantai2 lorong 1";
const char* pass = "56789012";
String html_code = "<!DOCTYPE html><html><head><style>.button {border: none;padding: 12px 40px;text-align: center;text-decoration: none;display: inline-block;font-size: 16px;margin: 4px 2px;cursor: pointer;}.button1 {background-color: black; color: white; border: 3px solid #1c1c1b;border-radius: 30px}body {text-align: center;}</style></head><body><h2>IoT Based Keychain</h2><p>Press the Button to Turn On/Off the Buzzer</p><form action=\"/BUZ\" method=\"POST\"><button class=\"button button1\">Click Me!</button></form>";
boolean buzzing_state = false;
void handleRoot() { 
server.send(200, "text/html", html_code + "Current state: <b>" + buzzing_state);
}
void handleBUZ() { 
buzzing_state = !buzzing_state; 
server.sendHeader("Location","/"); 
server.send(303); 
}
void handleNotFound(){
server.send(404, "text/plain", "404: Not found"); 
}
void setup(void){ 
  Serial.begin(115200);        
  delay(10);
  pinMode(buz_pin, OUTPUT); 
  Serial.print("\n\nConnecting Wifi... ");
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {    
    delay(500);
  }
  Serial.println("OK!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());           
  Serial.println();
  server.on("/", HTTP_GET, handleRoot);     
  server.on("/BUZ", HTTP_POST, handleBUZ);  
  server.onNotFound(handleNotFound);        
  server.begin();                           
  Serial.println("HTTP server started\n");
}
void loop(void){ 
  server.handleClient();                    
  if (buzzing_state == true) {
    digitalWrite(buz_pin, HIGH);
    delay(400);
    yield();
    digitalWrite(buz_pin, LOW);
    delay(200);
    yield();
    delay(400);
    yield();
  }
}
