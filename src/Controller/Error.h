// Error.h

#ifndef _ERROR_h
#define _ERROR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif
enum OverShoot { NONE, LEFT, RIGHT };
struct Error {
	OverShoot overshoot;
	float current;
	float previous;
	float total;
	Error();
	void setOverShoot(OverShoot overShoot);
	void setCurrentError(float& errorSum, int & activeSensorsNumber);
	int getErrorMultiplier() const;

private:
	bool isOverShooted() const;
	void setOverShootError();
	void setNormalError(float & errorSum, int & activeSensorsNumber);
	void resetOverShootIfOnLine();
};

#endif

