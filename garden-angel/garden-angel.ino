/*
  Garden Angel for Arduino Uno release 0.1

  DHT11 : humidity and temperature sensor
    + 100nF capacitor
    + 10KOhms resistor
    + digital pin read capacity
    
  photoresistor : light sensor
    + 10KOhms resistor
    + analog pin read capacity

  7 segments digit (. to be considered as h segment) :
    + 220Ohms resistor
    + digital pin write capacity

  1 push button :
    + 10KOhms resistor
    + digital pin read capacity

  author : "Freddy Foruin" <freddy.frouin@linuxtribe.fr>

  sources used :
    https://nakkaya.com/2009/10/29/connecting-a-photoresistor-to-an-arduino/
    https://www.carnetdumaker.net/articles/utiliser-un-capteur-de-temperature-et-dhumidite-dht11-dht22-avec-une-carte-arduino-genuino/#le-capteur-dht22-et-son-petit-frere-le-dht11
    http://circuitdigest.com/microcontroller-projects/7-segment-display-interfacing-with-arduino
    https://www.arduino.cc/en/tutorial/pushbutton
*/

#include <Time.h>

#include "history.h"
#include "digits.h"
#include "sensors.h"

const int  buttonPin = 4;

int firstRun = 0;

void setup() {
  Serial.begin(115200);
  Serial.println(F("Garden Angel v0.1 - Freddy Frouin <freddy@linuxtribe.fr>"));
  Serial.println();
  Serial.print(F("  Data History length : "));
  Serial.print(HIST_LENGTH/86400);
  Serial.println(F(" day"));
  Serial.print(F("  Data History resolution : "));
  Serial.print(HIST_PERIOD/60);
  Serial.println(F(" minutes"));
  Serial.println(F("  Data acquired every second : average values are pushed to history"));
  Serial.println();
  Serial.println(F("push the button to display the history or to show last values collected on the 7 segments digit"));

  if (firstRun == 0) {
    firstRun = 1;
    /* DHT11 pin setup */
    pinMode(BROCHE_DHT11, INPUT_PULLUP);
    /* 7 segment led pin setup */
    for (int i=13;i>5;i--) { pinMode(i, OUTPUT); }
    /* button pin setup */
    pinMode(buttonPin, INPUT);
    display_message("hello - garden angel 0.1 - freddy.frouin at linuxtribe.fr ", 100);
    /* init local clock */
    setTime(LOCAL_TIME+7200);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  float tempAVG = -1, humAVG = -1, lightAVG = -1; 
  float temperature, humidity, light;

  float buttonState = digitalRead(buttonPin);

  /* data acquisition cycle reported to end-user ù*/
  display_char('.',50);

  
  light = analogRead(BROCHE_LIGHT)/10;

  switch (readDHT11(BROCHE_DHT11, &temperature, &humidity)) {
  case DHT_SUCCESS: 
     
     /* Display results on 7 segments display
    timestamp();  
    Serial.print(F(" Humidite (%): "));
    Serial.print(humidity, 2);   
    Serial.print(F(" Temperature (°C): "));
    Serial.print(temperature, 2);
    Serial.print(F(" Lumière (%) : "));
    Serial.println(light);   
    */
    
    break;
 
  case DHT_TIMEOUT_ERROR:
    timestamp(); 
    Serial.println(F(" DHT11 sensor : communication timeout !")); 
    break;
 
  case DHT_CHECKSUM_ERROR: 
    timestamp();
    Serial.println(F(" DHT11 sensor : communication checksum error !")); 
    break;
  }

  if (humAVG == -1) {
    humAVG = humidity;
    tempAVG = temperature;
    lightAVG = light;
  } else {
    humAVG = (humAVG+humidity)/2;
    tempAVG = (tempAVG+temperature)/2;
    lightAVG = (lightAVG+light)/2;
  }

  /* if data stored to history DB then we reset average counters */
  if (storeData(humAVG, tempAVG, lightAVG)) {
    humAVG = -1;
    tempAVG = -1;
    lightAVG = -1;
  }

  /* button pushed by end users */
  if (buttonState == LOW) {
    flushHistoryRequest();

    display_number(humidity);
    display_char('h'); 
    display_number(temperature);
    display_char('c');
    display_number(light);
    display_char('l');
  }
  
  delay(1000);
}

