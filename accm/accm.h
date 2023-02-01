/*
 * by Freddy Frouin <freddy@linuxtribe.fr>
 */
 
#ifndef ACCM_h
#define ACCM_h

#include "Arduino.h"
#include "sct_line.h"
#include "value.h"

#define ACCM_LINES 4

class accm {
  private:
	int loop_index;
	unsigned long int timestamp[ACCM_LINES];
			
    value readSample(int line_index, bool calib=false);
    
    value readAC(int line_index, bool calib=false);
    value readPower(int line_index, bool calib=false);
    
  public:
	  float noise[ACCM_LINES];
  	value clamp[ACCM_LINES];
	  value meter[ACCM_LINES];
	  double counter[ACCM_LINES];
	  unsigned long int warnCounter[ACCM_LINES];	

    sct_line line[ACCM_LINES];
    
    accm(void);	
    void calibrate(int line_index, int iterations);
    void loop(void);
};

#endif
