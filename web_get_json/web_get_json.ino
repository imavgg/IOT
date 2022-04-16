/*
 *  This sketch sends data via HTTP GET requests to data.sparkfun.com service.
 *
 *  You need to get streamId and privateKey at data.sparkfun.com and paste them
 *  below. Or just customize this script to talk to other HTTP servers.
 *
 */

#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid     = "P880";
const char* password = "0917251331";

const char* url = "https://data.taipei/api/v1/dataset/bf9f74e7-22d0-4e0f-8e3d-31c04eda22a4?scope=resourceAquire";
//const char* streamId   = "....................";
//const char* privateKey = "....................";

void setup()
{
    Serial.begin(115200);
    delay(10);

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ssid : ");
    Serial.println(ssid);
    WiFi.mode(WIFI_STA);
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

int value = 0;

void loop()
{
    delay(5000);
    ++value;

    Serial.print(" Connecting to url ");
    Serial.println(url);


    // Connect to http:    
    HTTPClient http;
    http.begin(url);
    int httpCode = http.GET(); //200 normal, 404 moved away
    Serial.print("httpcode is ");
    Serial.println(httpCode);
    
    if(httpCode == HTTP_CODE_OK) {
      String data = http.getString();
      Serial.println("Connected");
      Serial.print("length of data: ");
      int len = data.length()+1;
      Serial.println(len);

      DynamicJsonDocument JObject(1024); //in case the number is too long
      deserializeJson(JObject,data);
      //
      for (int i = 0; i <= len; i++){
//        Serial.println(JObject["_id"][i]);

        Serial.println(JObject);
        }
        
    }else{
      Serial.println("Not connected");

      }

    http.end();
    Serial.println("closing connection");
}
