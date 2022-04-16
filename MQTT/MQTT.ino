#include <PubSubClient.h>
#include <WiFi.h>
#include <SimpleDHT.h>
#include <myclass.h>

const char* ssid     = "yours network";
const char* password = "yours network";
const char* server = "yours server";
const char* server_id = "yours server";
const char* server_pasw ="yours  server";

//const char* server = "mqtt.eclipseprojects.io";
//const char* server = "broker.emqx.io";

int pinDHT = 19; 
int pinMQ = 32;
SimpleDHT11 dht11(pinDHT);

WiFiClient ESPClient; 
PubSubClient MQTTclient(ESPClient);

unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup()
{
 
    pinMode(2, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
    Serial.begin(115200);

    //Setup WIFI
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
         
    delay(5000);

    //setup MQTT

    MQTTclient.setServer(server, 1883);
    MQTTclient.setCallback(callback);

}


void loop()
{
  // check MQTT connect?
  if (!MQTTclient.connected()) {
    reconnect();
  }
  // update MQTT Publish,subscribe
  MQTTclient.loop();

  // If the transfer is out to date, then retransfer. 
  unsigned long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    

      byte temperature = 0 ;
      byte humidity = 0 ;
      double air_qua = 0;
      get_sensor_data(&temperature,&humidity,&air_qua);
      
      // Once connected, publish an announcement...
      MQTTclient.publish("sensorHUMID",String(humidity).c_str());
      MQTTclient.publish("sensorTEMP",String(temperature).c_str());
      MQTTclient.publish("sensorAIR",String(air_qua).c_str());
 
      // ... and resubscribe
      MQTTclient.subscribe("mqtt_topic");

    snprintf (msg, MSG_BUFFER_SIZE, "humidity ",String(humidity).c_str());
    
  }
}


// if there is new topic update, then update.
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!MQTTclient.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    // Create a random client ID
    String clientId = "ESP32Client-eqkk36";
//    clientId += String(random(0xffff), HEX);
    Serial.println("clientId="+clientId);
    
    // Attempt to connect
    if (MQTTclient.connect(clientId.c_str(),server_id,server_pasw)) {
      Serial.println("MQTT connected");

    } else {
      Serial.print("failed, rc=");
      Serial.print(MQTTclient.state());
      Serial.println(" try again in 2 seconds");
      delay(2000);
    }
  }
}


void get_sensor_data(byte *temperature, byte *humidity, double *air_qua)
{    
    *air_qua = analogRead(pinMQ);
    
    int err = SimpleDHTErrSuccess;   
    if ((err = dht11.read(temperature,humidity, NULL) )!= SimpleDHTErrSuccess)
    {
      Serial.println("Read DHT failed. err = ");
      Serial.println(err);
      delay(100);
      return;
    }else
    Serial.println("Read DHT success.");
}
    
