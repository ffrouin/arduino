/*
 * by Freddy Frouin <freddy@linuxtribe.fr>
 */
 
#ifndef SCT_LINE_h
#define SCT_LINE_h

class sct_line {
	public:
		int pin;
		int sct_model;
		int voltage;
		int freq;
		float warn; // kWh
		
		sct_line(void);
};

#endif
