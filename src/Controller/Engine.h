// Engine.h

#ifndef _ENGINE_h
#define _ENGINE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

class Engine {
	int pinForward;
	int pinBackward;
	int pinSpeed;
	int speed;
	//TODO direction?

public:
	Engine(int pinForward, int pinBackward, int pinSpeed);
	void moveForward(int speed) const;
	void moveBackward(int speed) const;
	void stop() const;
};

#endif

