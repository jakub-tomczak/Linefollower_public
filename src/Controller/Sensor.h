// Sensor.h

#ifndef _SENSOR_h
#define _SENSOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class Sensor {
	int pin;
	int result;
	float weight;

public:
	Sensor(int pin, float weight);
	void read();
	float getValue() const;
	int getResult() const;
};

#endif

