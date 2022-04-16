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
#include <WiFiClientSecure.h>
#include <SimpleDHT.h>

const char* ssid     = "your network";
const char* password = "your network";

String url = "your hook?value1="; //get
int pinDHT = 19; 
SimpleDHT11 dht11(pinDHT);


void setup()
{
    Serial.begin(115200);
    
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
}


void loop()
{
    double air_qua = analogRead(32);

    int err = SimpleDHTErrSuccess;
    byte temperature = 0 ;
    byte humidity = 0 ;

    if ((err = dht11.read(&temperature,&humidity, NULL) )!= SimpleDHTErrSuccess)
    {
      Serial.println("Read DHT failed. err = ");
      Serial.println(err);
      delay(100);
      return;
    }else
    Serial.println("Read DHT success.");
    
    

      int message1 =  (int)temperature;
      int message2 =  (int)humidity;
      int message3 =  (int)air_qua;

      
    // Connect to http:    
    HTTPClient http;
    //thinkspeak
//    http.begin( url+message1+"&field2="+message2+"&field3="+message3 );
    //google sheet
    http.begin( url+message1+"&value2="+message2+"&value3="+message3 ); 

    int httpCode = http.GET(); //200 normal, 404 moved away
        
    if(httpCode == HTTP_CODE_OK) {
      String data = http.getString();
      Serial.println("Connected");
      Serial.println(data);
    }
    else{
      Serial.println("Not Connected");
      }

    
    http.end();
    delay(2000); //google sheet

    delay(15000); //thinkspeak
    Serial.println("closing connection");
}
