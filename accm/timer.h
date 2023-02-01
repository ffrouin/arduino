/*
 * by Freddy Frouin <freddy@linuxtribe.fr>
 */
 
#ifndef TIMER_h
#define TIMER_h

#include "Arduino.h"
#include "basics.h"

enum t_unit { msec, sec, min, hour, day };

class timer {
  private:
    unsigned long int start;   
    unsigned long int last;
    unsigned long int loop;

	String firstZero(float n);
	
  public:
    timer(void);

    void reset(void);
    void update(void);
    float duration(t_unit _unit=msec);
    String smartDuration(void);
};

#endif
