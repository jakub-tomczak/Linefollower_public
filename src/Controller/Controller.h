// LFControllerOwnPid.h

#ifndef _DEFINITIONS_h
#define _DEFINITIONS_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "Params.h"
enum DrivingMode { STRAIGHT, MOVE_LEFT, MOVE_RIGHT };
void driveOnLine();
void initializeSpeed();
void correctPosition(int & speedDifference);
void moveEngines(int & leftSpeed, int & rightSpeed);
void increaseTheSteeringAngle(Engine & inner, Engine & outer, int & outerSpeed);
#if SONAR_ENABLED
	void avoidObstacle();
	bool isObstacleDetected();
	DrivingMode getAvoidanceDirection(float &error);
	DrivingMode reverseDirection(DrivingMode currentMode);
	void driveOnBlind(DrivingMode mode);
#endif

#endif