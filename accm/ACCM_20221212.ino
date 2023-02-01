#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <MemoryFree.h>
#include "appButton.h"
#include "accm.h"
#include "basics.h"
#include "timer.h"

#define POWER_LED 12
#define COUNTER_LED 11

#define APP_BTN_PIN 2

const String APP="ACCM";
const String VERSION="v0.13";
const String DATE="20221216";
const String CONTACT="freddy@linuxtribe.fr";

timer uptime; // provides uptime loop and display for end-user.

appButton menuButton(APP_BTN_PIN, INPUT_PULLUP); // button app declaration

// declare display
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Declare the accm application and accm line number
accm EnergyApp;

void setup() {
  // setup ACCM Lines (for SCT013 probes)
  EnergyApp.line[0].pin = 3;
  EnergyApp.line[0].sct_model = 10;
  EnergyApp.line[0].freq = 50;
  EnergyApp.line[0].voltage = 220;
  EnergyApp.line[0].warn = 0.001;
  
  EnergyApp.line[1].pin = 2;
  EnergyApp.line[1].sct_model = 10;
  EnergyApp.line[1].freq = 50;
  EnergyApp.line[1].voltage = 220;
  EnergyApp.line[1].warn = 0.001;
  
  EnergyApp.line[2].pin = 1;
  EnergyApp.line[2].sct_model = 30;
  EnergyApp.line[2].freq = 50;
  EnergyApp.line[2].voltage = 220;
  EnergyApp.line[2].warn = 0.001;
  
  EnergyApp.line[3].pin = 0;
  EnergyApp.line[3].sct_model = 30;
  EnergyApp.line[3].freq = 50;
  EnergyApp.line[3].voltage = 220;
  EnergyApp.line[3].warn = 0.001;
  
/*
  Serial.begin(9600);
  Serial.println();
  Serial.println(APP + " " + VERSION + " " + DATE);
  Serial.println(String(freeMemory()) + " bytes RAM free");
*/
  
//   initialize serial port to access reports
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,1);
  display.println(APP + " " + VERSION + " " + DATE);
  display.println(CONTACT);
  display.println(String(freeMemory()) + " bytes RAM free");
  digitalWrite(POWER_LED, HIGH);
  digitalWrite(COUNTER_LED, HIGH);
  display.display();
  delay(500);
}

enum viewMode { group, single, settings, credentials, off };
enum menu { current, power, counter, budget, noise };
menu accmMenu = counter;
viewMode accmViewMode = credentials;
int accmCurrentLine = 0;

void nextViewMode(void) {
  switch (accmViewMode) {
    case group:
      accmDisplayMenu("VIEW MODE", "SINGLE");
      accmViewMode = single;
      break;
    case single:
      accmDisplayMenu("FIRMWARE", "SETTINGS");
      accmViewMode = settings;
      break;
    case settings:      
      accmDisplayMenu("SCREEN OFF", "SLEEPING");
      accmViewMode = off;
      break;
    case off:
      accmDisplayMenu("ABOUT", "CREDENTIALS");
      accmViewMode = credentials;
      break;
    case credentials:
      accmDisplayMenu("VIEW MODE", "GROUP");
      accmViewMode = group;
      break;
    default:
      break; 
  }
}

void nextAppMenu(void) {
  switch (accmMenu) {
    case current:
       accmDisplayMenu("MENU", "POWER CLAMP");
       accmMenu = power;
       break;
    case power:
      accmDisplayMenu("MENU", "ENERGY COUNTER");
      accmMenu = counter;
      break;
    case counter:
      accmDisplayMenu("MENU", "BUDGET CLAMP");
      accmMenu = budget;
      break;
    case budget:
      accmDisplayMenu("MENU", "NOISE THRESHOLDS");
      accmMenu = noise;
      break;
    case noise:
      accmDisplayMenu("MENU", "CURRENT CLAMP");
      accmMenu = current;
      break;
    default:
      break;
  }
}

void accmDisplayMenu(String m, String sm) {
  display.clearDisplay();
  int x = 5*floor((25-m.length())/2);
  display.setCursor(x,6);
  display.println(m);
  x = 5*floor((25-sm.length())/2);
  display.setCursor(x,18);
  display.println(sm);
  display.display();
  delay(800);
}

void accmAllLedsWorkarround(void) {
  int isOn = 0;
  for (int i=0;i<ACCM_LINES;i++) {
    if (EnergyApp.meter[i].eff() > EnergyApp.noise[i])
      isOn++;
  }
  if (isOn) {
    digitalWrite(POWER_LED, HIGH);
  } else {
    digitalWrite(POWER_LED, LOW);
  }
}

void accmSingleCounterLeds(void) {
  if (floor(EnergyApp.counter[accmCurrentLine]/EnergyApp.line[accmCurrentLine].warn) > EnergyApp.warnCounter[accmCurrentLine]) {
    EnergyApp.warnCounter[accmCurrentLine]++;
    digitalWrite(COUNTER_LED, HIGH);
    delay(80);
    digitalWrite(COUNTER_LED, LOW);
  }
}

void accmReport(void) {
    display.clearDisplay();
    display.setCursor(0,0);
    switch (accmViewMode) {
      case group:
        switch (accmMenu) {
          case current:
            accmAllLedsWorkarround();
            for (int i=0;i<ACCM_LINES; i++)
              if (EnergyApp.clamp[i].eff() > 1000)
                display.println("L" + String(i+1) + " " + String(EnergyApp.clamp[i].eff()/1000)+" A");
              else
                display.println("L" + String(i+1) + " " + String(EnergyApp.clamp[i].eff())+" mA");
            break;
          case power:
            accmAllLedsWorkarround();
            for (int i=0;i<ACCM_LINES; i++)
              if (EnergyApp.meter[i].eff() > 1000)
               display.println("L" + String(i+1) + " " + String(EnergyApp.meter[i].eff()/1000)+" kWatt");
             else
               display.println("L" + String(i+1) + " " + String(EnergyApp.meter[i].eff())+" Watt");
            break;
          case counter:
            accmAllLedsWorkarround();
            for (int i=0;i<ACCM_LINES; i++) {
               accmCurrentLine = i;
               accmSingleCounterLeds();
               display.println("L" + String(i+1) + " " + String(EnergyApp.counter[i],10)+" kWh");
            }  
            break;
          case budget:
            accmAllLedsWorkarround();
            for (int i=0;i<ACCM_LINES; i++)
              display.println("L" + String(i+1) + " " + String(EnergyApp.counter[i]*0.1740,10)+" euros");
            break;
          case noise:
            accmAllLedsWorkarround();
            for (int i=0;i<ACCM_LINES; i++)
              display.println("N" + String(i+1) + " " + String(EnergyApp.noise[i],2)+" Watt");
            break;
          default:
            break;
        };
        break;
      case single:
        if (EnergyApp.meter[accmCurrentLine].eff() > EnergyApp.noise[accmCurrentLine]) {
          digitalWrite(POWER_LED, HIGH);
        } else {
          digitalWrite(POWER_LED, LOW);
        }
        accmSingleCounterLeds();
        display.print("L" + String(accmCurrentLine+1) + " A" + String(EnergyApp.line[accmCurrentLine].pin));
        if (EnergyApp.noise[accmCurrentLine] > 1000)
          display.print(" N" + String(EnergyApp.noise[accmCurrentLine]/1000,2)+"kW ");
        else
          display.print(" N" + String(EnergyApp.noise[accmCurrentLine],2)+"W ");
        display.println("1V/" + String(EnergyApp.line[accmCurrentLine].sct_model) + "A" );
        if (EnergyApp.clamp[accmCurrentLine].eff() > 1000)
          display.print(String(EnergyApp.clamp[accmCurrentLine].eff()/1000,3)+"A ");
        else
          display.print(String(EnergyApp.clamp[accmCurrentLine].eff(),2)+"mA ");
        if (EnergyApp.meter[accmCurrentLine].eff() > 1000)
          display.println(String(EnergyApp.meter[accmCurrentLine].eff()/1000,3)+"kW");
        else
          display.println(String(EnergyApp.meter[accmCurrentLine].eff(),2)+"W");
        display.println(String(EnergyApp.counter[accmCurrentLine],10)+" kWh");
        display.println(String(EnergyApp.counter[accmCurrentLine]*0.1740,10)+" euros");      
        break;
      case settings:
        for (int i=0;i<ACCM_LINES; i++)
         display.println("L" + String(i+1) + " A" + String(EnergyApp.line[i].pin) + " " + String(EnergyApp.line[i].voltage) + "V" + String(EnergyApp.line[i].freq) + "Hz 1V/" + String(EnergyApp.line[i].sct_model) + "A");
        break;
      case credentials:
        digitalWrite(POWER_LED, LOW);
        digitalWrite(COUNTER_LED, LOW);
        display.println(APP + " " + VERSION + " " + DATE);
        display.println(CONTACT);
        display.println(String(freeMemory()) + " bytes RAM free");
        display.println("up " + uptime.smartDuration()); 
        break;
      default:
        break;
    };       
    display.display();    
}

void loop() {
  // put your main code here, to run repeatedly:
  menuButton.loop();
  EnergyApp.loop();
   
  if (menuButton.shortPress() || menuButton.longPress()) {
    switch (accmViewMode) {
      case group:
        nextAppMenu();
        break;
      case single:
        if (++accmCurrentLine == ACCM_LINES)
          accmCurrentLine = 0;
        break;
      default:
        break;
    }
  }
  if (menuButton.veryLongPress()) {
    display.clearDisplay();
    display.setCursor(0,0);
    switch (accmViewMode) {
      case group:
        accmDisplayMenu("FAST CALIBRATION", "all lines off");
        display.setCursor(40,9);
        display.print("__");
        display.display();       
        for (int i=0;i<ACCM_LINES; i++) {
          EnergyApp.calibrate(i, 100);
          display.print("__");
          display.display();
        }
        break;
      case single:
        accmDisplayMenu("FINE CALIBRATION", "L" + String(accmCurrentLine +1 ) + " A" + String(EnergyApp.line[accmCurrentLine].pin) + " off");
        EnergyApp.calibrate(accmCurrentLine, 1000);
        break;
      default:
        break;    
    }
  }
  if (menuButton.doublePress()) {
    nextViewMode();
  }
  accmReport();
  uptime.update();
}
