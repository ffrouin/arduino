#include "accm.h"
#include "basics.h"

accm::accm(void) {
	loop_index = 0;
	
	for (int i=0; i<ACCM_LINES; i++) {
		// noise[i]=DEFAULT_INIT_VALUE; // this will force user to calibrate before accounting to start
    noise[i]=0; // this will run accounting since board is powered on
		timestamp[i]=DEFAULT_INIT_VALUE;
		counter[i]=0;
		warnCounter[i]=0;
	}
}

value accm::readSample(int line_index, bool calib) {
	float timeout = 1000/line[line_index].freq; 
	int v = 0;
	value sample;
	
	if (calib)
		sample.avg_mm = false;
		
	unsigned long int start = millis();
	
	while(millis()-start<timeout) {
		v=analogRead(line[line_index].pin);
		sample+=v;
	}
	
	sample*=mV;
	/*
	if (log_level) {
		float amp =  sample.mmax-sample.mmin;
		Serial.print("[Port A" + String(line[line_index].pin));
		Serial.print(" SCT013 1V/" + String(line[line_index].sct_model));
		Serial.print(" " + String(line[line_index].freq) + "Hz:" + String(sample.count) + "pts ");
		Serial.print("("+String(sample.mmax,2)+"-"+String(sample.mmin,2)+"mV c="+String(amp/2));
		Serial.print("mV) Amp "+String(amp,2) + "mV] ");
	}
  */
	return(sample);
}

value accm::readAC(int line_index, bool calib) {
	value mA = readSample(line_index, calib);
	mA *= line[line_index].sct_model;
	/*
	if (log_level) {
		Serial.print(String(mA.eff()) + "mA");
	}
 */
	clamp[line_index]=mA;
	return(mA);
}

value accm::readPower(int line_index, bool calib) {
	value W = readAC(line_index, calib);
	W /= 1000; // mA to A
	W *= line[line_index].voltage; // P=UI W or Wh

  /*
	if (log_level) {
		Serial.print(":" + String(W.eff()) + "W");
	}
 */
	meter[line_index]=W;
	return(W);	
}

void accm::calibrate(int line_index, int iterations) {
	float n_level = 0;
	
	value n = readPower(line_index, true);
	n_level = n.eff();
	
	for (int i=0;i<iterations;i++) {
		n = readPower(line_index, true);
		n_level += n.eff();
		n_level /= 2;
	}
	
	noise[line_index]=2*n_level; // setup noise level threshold to 180%
	/*
	if (log_level) {
		Serial.println(" noise threshold " + String(noise[line_index])+"W");
	}
 */
}

void accm::loop(void) {
	if (noise[loop_index] != DEFAULT_INIT_VALUE) {
		value P = readPower(loop_index, false);
		if (P.eff() > noise[loop_index]) { 
			double kWh = P.eff();
			kWh /= 1000; // Wh to kWh
			if (timestamp[loop_index] != DEFAULT_INIT_VALUE) {
				unsigned long int now = millis();
				double count = now-timestamp[loop_index];
				count /= 1000; // millis to seconds
				count /= 3600; // seconds to hours
				count *= kWh; // total energy used
				counter[loop_index] += count; // added to counter
       /*
				if (log_level) {
					Serial.print(":" + String(count,8) + "kWh");
					Serial.println(":" + String(counter[loop_index],8) + "kWh");
				}     
			} else {        
				if (log_level)
					Serial.println();
         */
			}
		}		
		timestamp[loop_index] = millis();
	}
	
	/* else {
		if (log_level) {
			Serial.print("WARNING - please calibrate sct013 device line " + String(loop_index+1));
			Serial.println(" port A" + String(line[loop_index].pin));
		}
	}
 */
	
	if (++loop_index == ACCM_LINES)
		loop_index = 0;
}
