# IOT

# Requirement:
* Hardware : 
  * MCU: esp32 wroom
  * Sensors: 
    * home: MQ135,DHT11 
    * sound: MAX98357A,INMP441, speaker
    * display: ST7735
* Software : 
  * Arduino IDE
  * packages: esp-idf,
    * home: simpledht 
    * sound: esp32_audio_I2S
    * display: ucglib
 
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
     sudo apt install mosquitto
 
     sudo apt install mosquitto-clients

     service mosquitto start
    ```
  2. Set hardware and run Arduino code
  3. Check the data transfer situation from mosquite.

## Voice and Sound project

Goal: voice control to command on application
### Set Microphone
1. set INMP441 pin VDD, GND and I2S communication pins as followings:
```
 i2s_pin_config_t i2sPins = {
    .bck_io_num = GPIO_NUM_32,(SCK)
    .ws_io_num = GPIO_NUM_25,(WS)
    .data_in_num = GPIO_NUM_33};(SD)
    
 ```
2. run memssampler.ino
3. record with I2SMEMSsamper.cpp` printf(" 0x%x,",samples[sample_index]);` and save to file(record.h)
### Set Speaker
1. Buy one , or try to find old speaker, and break down (Me, get a 8Ω 3W speaker)
2. Pin set up: 

```
i2s_pin_config_t i2sPins = {
    .bclk_io_num = GPIO_NUM_26,(CLK)
    .ws_io_num = GPIO_NUM_25,(LRC)
    .data_in_num = GPIO_NUM_22};(SD)
    
```
4. Define SetRate in SampleRate in playAACFromPROGMEM.ino to replay previous record.


## LCD Display project
Gloal: show real-time sensor ouput.
## Camera Detect project
