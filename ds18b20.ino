
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <OneWire.h>
#include <DallasTemperature.h>

OneWire oneWire(D4);
DallasTemperature sensors(&oneWire);

const char* host = "api.thingspeak.com";
String url = "/update?api_key=WD112D9UCJ4ZWDPR";   // Your Own Key here
const int httpPort = 80;
int interval = 60000;

const char* ssid = "ABTC";   // Your own ssid here
const char* password = "dnflwlq0";  // Your own password here

String working(double deg) { 
  
  return(String("field1=")+String(deg));
}

void delivering(String payload) { 
  WiFiClient client;
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, httpPort)) {
    Serial.print("connection failed: ");
    Serial.println(payload);
    return;
  }
  String getheader = "GET "+ String(url) +"&"+ String(payload) +" HTTP/1.1";
  client.println(getheader);
  client.println("User-Agent: ESP8266 Kyuho Kim");  
  client.println("Host: " + String(host));  
  client.println("Connection: close");  
  client.println();

  Serial.println(getheader);
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    Serial.println(line);
  }
  Serial.println("Done cycle.");
}

void connect_ap() {
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\n Got WiFi, IP address: ");
  Serial.println(WiFi.localIP());  
}

void setup() {
  Serial.begin(115200);
  connect_ap();
  sensors.begin();
}

unsigned long mark = 0;
void loop() {
  if (millis() > mark ) {
     mark = millis() + interval;
     sensors.requestTemperatures();
     double deg = sensors.getTempCByIndex(0);
     Serial.println(deg);
     String payload = working(deg);
     delivering(payload);
  }
}
