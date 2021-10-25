#include <ESP8266WiFi.h> 
#include <ESP8266WebServer.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <WiFiClient.h> 
 
ESP8266WebServer server(80);
TinyGPSPlus gps;  // The TinyGPS++ object
SoftwareSerial ss(4, 5); // The serial connection to the GPS devic

const int buz_pin = D4;
boolean buzzing_state = false;
float latitude , longitude;
String lat_str = "";
String lng_str = "";
String html_code;
char ssid[] = "ICHIRO UNIFI 2.4G";
char pass[] = "icH12o2021";


void handleRoot() {
  html_code = "<!DOCTYPE html><html><head><style>.button {border: none;padding: 12px 40px;text-align: center";
  html_code += "text-decoration: none;display: inline-block;font-size: 16px;margin: 4px 2px;cursor: pointer;}.button1 {background-color: black;"; 
  html_code += "color: white; border: 3px solid #1c1c1b;border-radius: 30px}body {text-align: center;}</style></head><body><h2>Smart Key With Buzzer And GPS</h2>";
  html_code += "<p>Press the Button to Turn On/Off the Buzzer</p><form action=\"/BUZ\" method=\"POST\"><button class=\"button button1\">Click Me!</button></form>";
  html_code += "<table ALIGN=CENTER style=width:50%> <tr> <th>Latitude</th><td ALIGN=CENTER >";
  html_code += lat_str;
  html_code += "</td> </tr> <tr> <th>Longitude</th> <td ALIGN=CENTER >";
  html_code += lng_str;
  html_code += "</td></tr> </table> ";

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
  Serial.print("\n\nConnecting Wifi");
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED)
  {    
    delay(500);
    Serial.print(".");
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
  if (buzzing_state == true) 
  {
    digitalWrite(buz_pin, HIGH);
    delay(400);
    yield();
    digitalWrite(buz_pin, LOW);
    delay(200);
    yield();
    delay(400);
    yield();
  }

  while (ss.available() > 0)
    Serial.println("ada serial");
    if (gps.encode(ss.read()))
    {
      Serial.println("di encode");
      if (gps.location.isValid())
      {
        latitude = gps.location.lat();
        lat_str = String(latitude , 6);
        longitude = gps.location.lng();
        lng_str = String(longitude , 6);
        Serial.println("latitude " + lat_str);
        Serial.println("longitude " + lng_str);
      }
    }

  if (gps.location.isValid())
  {
    html_code += "<p align=center><a style=""color:RED;font-size:125%;"" href=""http://maps.google.com/maps?&z=15&mrt=yp&t=k&q=";
    html_code += lat_str;
    html_code += "+";
    html_code += lng_str;
    html_code += """ target=""_top"">Click here!</a> To check the location in Google maps.</p>";
  }

  html_code += "</body> </html> \n";
  delay(100);
}
