//--> library yang dibutuhkan
#include <DHT.h>
#include <ESP8266WiFi.h>
#include "ThingSpeak.h"

//--> diganti sesuai kebutuhan pin sensor
#define DHTPIN D7
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

//--> wifi client digunakan untuk connect ke server dan port
WiFiClient client;

//--> environment API_KEY third party Thingspeak
String apiKey = "AXCMPMAVM5MZ20MR"; //--> perlu diganti
const char* server = "api.thingspeak.com";

//--> ganti ssid dan password sesuai yang anda gunakan
const char* ssid = "anjani";
const char* pass = "gendhisanjani";

void setup(){
  Serial.begin(115200);
  delay(10);
  dht.begin();
  ThingSpeak.begin(client);
  Serial.println("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void loop()
{ 
  float h = dht.readHumidity();
  float t = dht.readTemperature(false);
  
  if (isnan(h)||isnan(t)){
      Serial.println("Failed to read from DHT sensor!");
      return;
      delay(1000);
    }
    if(client.connect(server, 80)){
      String postStr= apiKey;
      postStr += "&field1=";
      postStr += String(t);
      postStr += "&field2=";
      postStr += String(h);
      postStr += "\r\n\r\n";
      
      client.print("POST /update HTTP/1.1\n");
      client.print("Host: api.thingspeak.com\n");
      client.print("Connection: close\n");
      client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
      client.print("Content-Type: application/x-www-form-urlencoded\n");
      client.print("Content-Length: ");
      client.print(postStr.length());
      client.print("\n\n");
      client.print(postStr);
      
      Serial.print("Temperature: ");
      Serial.print(t);
      Serial.print(" degrees Celcius, Humidity: ");
      Serial.print(h);
      Serial.print("%. Send to Thingspeak.");
    }
  
  client.stop();
  Serial.println(" Waiting...");
  delay(1000);
}
