// Error.h

#ifndef _CORRECTOR_h
#define _CORRECTOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "Sensor.h"
#include "Error.h"
#include "Params.h"

class Corrector {
	Error error;
	Sensor * sensors;
	int numberOfSensors;
	float KP;
	float KD;
	float KI;
public:
	Corrector(Sensor * sensors, const int & numberOfSensors,  float & KP, float & KD, float & KI);
	int getSpeedDifference();
	float getCurrentError() const;
	void resetError();
	#if SERIAL_DEBUG
	void serialDebug();
	#endif
private:
	void readFromSensors() const;
	void countError();
	int computeSpeedDifference();
};

#endif

