#include<ESP8266WiFi.h>

void setup() {
  Serial.begin(9600);
}

void loop() {
  if(Serial.available()>0){
    char uniqueOrderNum = Serial.read();
    Serial.print(uniqueOrderNum);
  }
 
}