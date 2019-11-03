#include "DHTesp.h"
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

int UVsensorIn = A0;
int DHTPin = D0;
DHTesp dht;
const char fingerprint[] = "65 1B A9 FB 88 94 E8 3E 11 56 4E A3 26 2D 09 AF 75 5C 7C 61";
String data = "";
void setup()
{
  pinMode(UVsensorIn, INPUT);
  dht.setup(DHTPin, DHTesp::DHT11);
  Serial.begin(9600);

  WiFi.begin("SSID", "PWD");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Waiting for connection");
  }
  Serial.println("Connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
 
void loop()
{
  if(getData()) {
    sendDataToServer();
    delay(1000*60*10);
  }

}
 
// takes an average of readings on a given pin
// returns the average
int averageAnalogRead(int pinToRead)
{
  byte numberOfReadings = 8;
  unsigned int runningValue = 0; 
 
  for(int x = 0 ; x < numberOfReadings ; x++)
    runningValue += analogRead(pinToRead);
  runningValue /= numberOfReadings;
 
  return(runningValue);  
 
}
 
 
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
bool getData() {
  int uvLevel = averageAnalogRead(UVsensorIn);
  float outputVoltage = 3.3 * uvLevel/1024;
  float uvIntensity = mapfloat(outputVoltage, 0.99, 2.9, 0.0, 15.0);
  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();
  
  if(!isnan(humidity) && !isnan(temperature)) {
    Serial.print("UV Intensity: ");
    Serial.print(uvIntensity);
    Serial.println(" mW/cm^2"); 
    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print(" %\nTemperature: ");
    Serial.print(temperature);
    Serial.println(" deg C\n");
    data = "humid=" + String(humidity) + "&temp=" + String(temperature) + "&uv=" + String(uvIntensity);
    return true;
  }
  delay(1000);
  return false;
}
void sendDataToServer() {
  if(!data)
    return;
  const char host[] = "stoneapp.tech";
  String link = "/iot_test/send.php";
  WiFiClientSecure httpsClient;
  
  
  if(WiFi.status() == WL_CONNECTED) {
    int r=0; // retry counter
    while((!httpsClient.connect(host, 443)) && (r < 30)){
        delay(100);
        r++;
    }
    if(r >= 30) {
      Serial.println("Connection failed");
    }
    if (httpsClient.verify(fingerprint, host)) {
      Serial.println("certificate matches");
    } else {
      Serial.println("certificate doesn't match");
    }
    httpsClient.print(String("POST ") + link + " HTTP/1.1\r\n" +
           "Host: " + host + "\r\n" +
           "Content-Length: " + String(data.length()) + "\r\n" +
           "Content-Type: application/x-www-form-urlencoded" + "\r\n\r\n" +
           data + "\r\n" +
           "Connection: close");
    
    while (httpsClient.connected()) {
      String line = httpsClient.readStringUntil('\n');
      if (line == "\r") {
        Serial.println("headers received");
        break;
      }
    }
    String line = httpsClient.readStringUntil('\n');
    
    Serial.println("reply was:");
    Serial.println("==========");
    Serial.println(line);
    while(httpsClient.available()){        
      line = httpsClient.readStringUntil('\n');
      Serial.println(line); // print response
    }
    Serial.println("==========");
    Serial.println("closing connection");
     
  } else {
      Serial.println("Error in WiFi connection");
   }
}
