#include "timer.h"
#include "basics.h"

timer::timer(void) {
  reset();
}

void timer::reset(void) {
	start = millis();
    loop = 0;
    last = 0;
}

void timer::update(void) {
	unsigned long int now = millis();
	
	if (now < last) {
		loop++;
	}
	last = now;
}

float timer::duration(t_unit _unit) {	
  unsigned long int now = millis();
  
  float len = ULLONG_MAX;
  len *= loop;
  len -= start;
  len += now;

  switch(_unit) {
      case msec:
	    return(len);
  	    break;
      case sec:
	    return(len/1000);
        break;
      case min:
	    return(len/60000);
        break;
      case hour:
	    return(len/3600000);
        break;
      case day:
        return(len/86400000);
        break;
      default:
	      return(0);
  	    break;
   }
   return(0);
}

String timer::smartDuration(void) {	
	if (duration(msec) < 1000) {
		return("0d 00:00:00."+String(duration(msec),0));
	} else if (duration(sec) < 60) {
		return("0d 00:00:" + String(duration(sec),3));
	} else if (duration(min) < 60) {
		float rs = duration(min)-floor(duration(min));
		rs *= 60;
		return("0d 00:" + String(duration(min),0) + ":" + String(rs,3));
	} else if (duration(hour) < 24) {
		float rm = duration(hour)-floor(duration(hour));
		rm *= 60;
		float rs = rm-floor(rm);
		rs *= 60;
		return("0d " + String(duration(hour),0)+":"+String(rm,0)+":"+String(rs,3));
	} else {
		float d = duration(day);
		float rh = d-floor(d);
		rh *= 24;
		float rm = rh-floor(rh);
		rm *= 60;
		float rs = rm-floor(rm);
		rs *= 60;
		return(String(floor(d),0)+"d "+ String(rh,0)+":"+String(rm,0)+":"+String(rs,3));
	}
}
