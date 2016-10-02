#  Garden Angel for Arduino Uno release 0.1

The garden angel is an electronic system that monitor air humidity,
temperature and light. Data is acquired every second. Due to the low
level of available memory on board, system is actually able to store
36 hours of data with a point every 28 minutes. Data stored is an average
value of the collected data over the storage period.

You can adujst the history length and the archive size of your board as
more memory available, you may get best data resolution in the data
history. Do not forget to recompile with your local time in order the
board to report correct timestamp. All this has to be configured in the
history.h file :

	time_t LOCAL_TIME=1475394875;
	
	const int HIST_CAPACITY=76;
	float HIST_LENGTH=86400*1.5;
	int HIST_PERIOD=int (HIST_LENGTH/HIST_CAPACITY);

The board will report to you on the serial console the number of day available
for data storage and the history resolution in minutes.

To display the history, just push the button on board until next data
acquisition and you'll get a CSV output of collected data on the serial
console of the board.

## Required hardware

  Arduino Uno board

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

## Sources used to build this project :

    https://nakkaya.com/2009/10/29/connecting-a-photoresistor-to-an-arduino/

    https://www.carnetdumaker.net/articles/utiliser-un-capteur-de-temperature-et-dhumidite-dht11-dht22-avec-une-carte-arduino-genuino/#le-capteur-dht22-et-son-petit-frere-le-dht11

    http://circuitdigest.com/microcontroller-projects/7-segment-display-interfacing-with-arduino

    https://www.arduino.cc/en/tutorial/pushbutton

## Messages produced on the serial console 

Garden Angel v0.1 - Freddy Frouin <freddy@linuxtribe.fr>

  Data History length : 1.50 day

  Data History resolution : 28 minutes

  Data acquired every second : average values are pushed to history

push the button to display the history or to show last values collected on the 7 segments digit

