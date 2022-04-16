# IOT

# Requirement:
* Hardware : 
  * MCU: esp32 wroom
  * Sensors: 
    * home: MQ135,DHT11 
    * sound: MAX98357A,INMP441, speaker
* Software : 
  * Arduino IDE
  * packages: esp-idf
 
## IFTTT project
Goal: record sensor data on google sheet.
1. Login IFTTT and start a Applet
2. Choose: If this (Webhook :receive from web request URL) then that(google sheet)
3. Change Arduino code URL to the Webhook URl done in step2  

## MQTT project
 Goal: use low-bandwidth MQTT protocal to transform data from MCU to server.
 
 1. Run a docker to have a mqtt service on linux:
     Run ubuntu Docker with port to connect network 
    ```
     docker run -it -p 1883:1883 -p 9001:9001 ubuntu /bin/bash
    ```
    Download mosquitto and run 
    ```
     service mosquitto start
    ```
  2. Set hardware and run Arduino code
  3. Check the data transfer situation from mosquite.

# Voice and Sound project

Goal: voice control to command on application

Still working on it ....
