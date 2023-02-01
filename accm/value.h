/*
 * by Freddy Frouin <freddy@linuxtribe.fr>
 */

#ifndef VALUE_h
#define VALUE_h

#include "Arduino.h"

class value {
	public:
		unsigned long int count;
		float mmin;
		float mmax;
		float last;		
		float sum;
		bool avg_mm = true;
	
		value(void);
		value &operator+=(float _v);
		value &operator*=(float _v);
		value &operator/=(float _v);
		float avg(void) { return(sum/count); }
		float eff(void) { return((mmax-mmin)/sqrt(2)); } // amp x2 as we removed vcc divider circuits, half of the period is zero.
		void reset(void);
};

#endif
