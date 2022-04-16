#include <BluetoothSerial.h>
#include<stdio.h>
#include <stdlib.h>
#include <analogWrite.h>

#define PIN_RED    15
#define PIN_GREEN  21
#define PIN_BLUE   5

BluetoothSerial BT;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  // bluetooth name
  BT.begin("TEST");
  
  sigmaDeltaSetup(0, 115200);
  sigmaDeltaAttachPin(PIN_RED, 1);
  sigmaDeltaAttachPin(PIN_BLUE, 2);
  sigmaDeltaAttachPin(PIN_GREEN, 3);

  sigmaDeltaWrite(1, 0);
  sigmaDeltaWrite(2, 0);
  sigmaDeltaWrite(3, 0);
}

void loop() {
      // put your main code here, to run repeatedly:
      BT.println("Hello");
      
      // check data from bt.
      while(BT.available()){
        
        String BTstring =BT.readString();        
        char arr[12];
        BTstring.toCharArray(arr, sizeof(arr));
        // change char to number.
        long BTdata = atol(arr);        
        // print to serial window.
        Serial.println(BTdata);
        
        // led change
        if (BTdata ==1){
          for (int i = 0; i++; i<255){
          sigmaDeltaWrite(1, i); //red
          sigmaDeltaWrite(2, 0);
          sigmaDeltaWrite(3, 0); 
          Serial.println("red");
  
          delay(10);
          }
        }
        else if (BTdata == 2){
           for (int i = 0; i++; i<255){
              sigmaDeltaWrite(1, 0); 
              sigmaDeltaWrite(2, 0);
              sigmaDeltaWrite(3, i); //green
              Serial.println("green");
              delay(10);
          }
        }
        else
        {
            sigmaDeltaWrite(1, 0); 
            sigmaDeltaWrite(2, 255);//blue
            sigmaDeltaWrite(3, 0); 
            Serial.println("else");
            delay(10);
        }
      
      }

      delay(1000);
}
