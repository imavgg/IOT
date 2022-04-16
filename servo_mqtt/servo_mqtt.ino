/*
reference:https://randomnerdtutorials.com/esp32-servo-motor-web-server-arduino-ide/?fbclid=IwAR31DAyFajk_WoBKjEYnNvTTkcSbon5AlxNUa1EjUQdHssgvKAvCY1DeKoA
*/

#include <ESP32Servo.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <SimpleDHT.h>



const char* server = "192.168.1.104"; //MQTT server run in Docker
WiFiServer WEBserver(80);
WiFiClient wificlient; 
PubSubClient client(wificlient); //mqtt server
Servo myServo;

int pinServo = 13;
int pinDHT = 19; 
int pinMQ = 32;

const char* ssid     = "P880";
const char* password = "0917251331";
char* sensorHUMID = "sensor/humidity";
char* sensorTEMP = "sensor/temperature";
char* mqtt_topic = "command/degree";

byte temperature = 0 ;
byte humidity = 0 ;
double air_qua = 0;

// Variable to store the HTTP request
String header;
int deg = 0;

// Decode HTTP GET value
String valueString = String(5);
int pos1 = 0;
int pos2 = 0;
// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0; 
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;
    
SimpleDHT11 dht11(pinDHT);


void setup() {

    Serial.begin(115200);
    
    WifiConnect();
    Serial.println("WiFi connected");

    // WEB
    WEBserver.begin();
    Serial.print("Web= ");
    Serial.println(WiFi.localIP());
    Serial.println("web served");

    myServo.attach(pinServo);

    // MQTT
    client.setServer(server, 1883);
    client.setCallback(callback);    
    Serial.println("MQTT served");

}



void loop() {
  

        // check MQTT connect?
        if (!client.connected()) {
          reconnect();
        }
        WiFiClient wificlient = WEBserver.available();
        if (wificlient){
          Web();//show on web
        }else
        { analogWrite(14,0); //blue
        }    
        client.loop();             
        analogWrite(27,0);      
        myServo.write(180); //degree
        delay(200);     
       
 
}



void Web(){
  

        currentTime = millis();
        previousTime = currentTime;
        String currentLine = "";                // make a String to hold incoming data from the client
        
        while (currentTime - previousTime <= timeoutTime) { 
          
              currentTime = millis();
//        
//            
//            if (currentLine.length() == 0) {         
//            
              // Create HTML Page:              
              wificlient.println("HTTP/1.1 200 OK");
              wificlient.println("Content-Type: text/html");
              wificlient.println("");
              wificlient.println("<!DOCTYPE HTML>");
              wificlient.println("<html><head><meta charset='utf-8'></head>");
              wificlient.println("<br>");
              wificlient.println("<h1> ESP32 SERVER </h1>");
              // Display the HTML web page 
              wificlient.println("<!DOCTYPE html><html>");
              wificlient.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
              wificlient.println("<link rel=\"icon\" href=\"data:,\">");
              
              wificlient.println("<style>body { text-align: center; font-family: \"Trebuchet MS\", Arial; margin-left:auto; margin-right:auto;}");
              wificlient.println(".slider { width: 300px; }</style>");
              wificlient.println("<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js\"></script>");
      
         
              // Web Page bar
              client.println("<p>Position: <span id=\"servoPos\"></span></p>"); 
              client.println("<input type=\"range\" min=\"0\" max=\"180\" class=\"slider\" id=\"servoSlider\" onchange=\"servo(this.value)\"      value=\"" + valueString+"\"/>");
              
      
              // Javascript
              client.println("<script>var slider = document.getElementById(\"servoSlider\");");
              client.println("var servoP = document.getElementById(\"servoPos\");          servoP.innerHTML = slider.value;");
              client.println("slider.oninput = function() { slider.value = this.value;       servoP.innerHTML = this.value; }");
              client.println("$.ajaxSetup({timeout:1000});   function servo(pos) { ");
                    
              // /?value=\180&\
              
              client.println("$.get(\"/?value=\" + pos + \"&\"); {Connection: close};}</script>");
              client.println("</body></html>");     

              //End of HTML
              
              // try to change to MQTT subscribe
              
              // update MQTT Publish,subscribe
              get_sensor_data(&temperature,&humidity,&air_qua);        

              client.publish("sensorHUMID",String(humidity).c_str());
              client.publish("sensorTEMP",String(temperature).c_str());
              client.publish("sensorAIR",String(air_qua).c_str());
              client.publish("mqtt_topic", "TEST MSG");

              client.subscribe("mqtt_topic");
              client.println();
              analogWrite(14,0); //blue
//
//              
//            } 
      } 
             
      
}

void WifiConnect(){
  
      WiFi.mode(WIFI_STA);
      WiFi.begin(ssid, password);
  
      while (WiFi.status() != WL_CONNECTED) {
          delay(500);
          Serial.print(".");
      }
}
   
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  String payloadString;
  for (int i=0;i<length;i++) {
    payloadString = payloadString +(char)payload[i];
  }
  Serial.println();

  if(strcmp(topic,mqtt_topic)==0){
    Serial.println("GET MQTT TOPIC= "+payloadString);
    int deg = payloadString.toInt();
    
    }
}
 

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    
    // Create a random client ID
    String clientId = "ESP32Client-eqkk36";
    
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("MQTT connected");

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
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
    }
    delay(2000);

}
    
