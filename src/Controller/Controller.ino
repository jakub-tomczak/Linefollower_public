#include "Error.h"
#include "Corrector.h"
#include "Sensor.h"
#include "Engine.h"
#include "Controller.h"
#include "Params.h"

#if SONAR_ENABLED
#include <NewPing.h>
#endif
Engine leftEngine(MOTOR_LEFT_FORWARD, MOTOR_LEFT_BACKWARD, MOTOR_LEFT_SPEED);
Engine rightEngine(MOTOR_RIGHT_FORWARD, MOTOR_RIGHT_BACKWARD, MOTOR_RIGHT_SPEED);
Sensor sensors[] = {
	Sensor(SENSOR_1_PIN, -SENSOR_WEIGHT_4),
	Sensor(SENSOR_2_PIN, -SENSOR_WEIGHT_3),
	Sensor(SENSOR_3_PIN, -SENSOR_WEIGHT_2),
	Sensor(SENSOR_4_PIN, -SENSOR_WEIGHT_1),
	Sensor(SENSOR_5_PIN, SENSOR_WEIGHT_1),
	Sensor(SENSOR_6_PIN, SENSOR_WEIGHT_2),
	Sensor(SENSOR_7_PIN, SENSOR_WEIGHT_3),
	Sensor(SENSOR_8_PIN, SENSOR_WEIGHT_4)
};

float KP = 17; // 41
float KD = 95;  // 61
float KI = 0.000015;
Corrector corrector(sensors, NUMBER_OF_SENSORS, KP, KD, KI);

#if SONAR_ENABLED
bool obstacleDetected = false;
NewPing sonar(SONAR_TRIG_PIN, SONAR_ECHO_PIN, MAX_DISTANCE);
#endif
/** INITIALIZATION SECTION */

void setup() {
	#if SERIAL_DEBUG
	Serial.begin(9600);
	Serial.print("START!");
		#if SET_PARAMS
			insertParams();
			checkParams();
		#endif
	#endif
	delay(2000);
	initializeSpeed();
}

void initializeSpeed() {
	float limit = MAX_SPEED;
	for (int speed = 0; speed <= limit; speed++) {
		leftEngine.moveForward(speed);
		leftEngine.moveForward(speed);
		delay(2);
	}
}

/** MAIN PROGRAM */
void loop() {
	#if SONAR_ENABLED
		if (isObstacleDetected()) {
			avoidObstacle();
		} else {
			driveOnLine();
		}
	#else
		driveOnLine();
	#endif
}

#if SONAR_ENABLED
bool checkIfInDistance(unsigned long distance);
void echoCheck() {
	if (sonar.check_timer()) {
		obstacleDetected = checkIfInDistance(sonar.ping_result / US_ROUNDTRIP_CM);
	}
}

bool checkIfInDistance(unsigned long distance) {
	return distance > 0 && distance < MAX_DISTANCE;
}

bool isObstacleDetected() {
	unsigned long ping_cm = sonar.ping_cm();
	return ping_cm > 0 && ping_cm <= MAX_DISTANCE;
}


void avoidObstacle() {
	float currentError = corrector.getCurrentError();
//	DrivingMode selectedDirection = getAvoidanceDirection(currentError);
	DrivingMode selectedDirection = MOVE_RIGHT;
//	do {
//		driveOnBlind(selectedDirection);
//		delay(60);
//	} while (isObstacleDetected());
	driveOnBlind(selectedDirection);
	delay(185);
	driveOnBlind(STRAIGHT);
	delay(150);
	selectedDirection = reverseDirection(selectedDirection);
	driveOnBlind(selectedDirection);
	delay(130);
	driveOnBlind(STRAIGHT);
	delay(100);
	driveOnBlind(selectedDirection);
	delay(120); // 100
	driveOnBlind(STRAIGHT);
	delay(60);//ok
	
//	selectedDirection = reverseDirection(selectedDirection); //TODO do testu czy zostawi� mu opcje samo ustawienia czy ustawia�.
//	driveOnBlind(selectedDirection);
//	delay(150);
	corrector.resetError();
}

DrivingMode getAvoidanceDirection(float & currentError) {
	return currentError < 0 ? MOVE_RIGHT : MOVE_LEFT;
}


DrivingMode reverseDirection(DrivingMode currentMode) {
	return currentMode == MOVE_LEFT ? MOVE_RIGHT : MOVE_LEFT;
}

void driveOnBlind(DrivingMode mode) {
	switch (mode) {
		case MOVE_LEFT: 
			rightEngine.moveForward(MAX_SPEED);
			leftEngine.moveBackward(MAX_SPEED);
		break;
		case MOVE_RIGHT:
			rightEngine.moveBackward(MAX_SPEED);
			leftEngine.moveForward(MAX_SPEED);
		break;
		case STRAIGHT:
			leftEngine.moveForward(MAX_SPEED);
			rightEngine.moveForward(MAX_SPEED);
			break;
		default: 
			break;
	}
}
#endif

void driveOnLine() {
	int speedDifference = corrector.getSpeedDifference();
	#if SERIAL_DEBUG
		corrector.serialDebug();
	#endif
	correctPosition(speedDifference);
}

void correctPosition(int & speedDifference) {
	int leftSpeed = constrain(BASE_SPEED + speedDifference, 0, MAX_SPEED);
	int rightSpeed = constrain(BASE_SPEED - speedDifference, 0, MAX_SPEED);
	moveEngines(leftSpeed, rightSpeed);
}

void moveEngines(int & leftSpeed, int & rightSpeed) {
	#if SERIAL_DEBUG
		Serial.print(" leftSp ");
		Serial.print(leftSpeed);
		Serial.print(" rightSp ");
		Serial.print(rightSpeed);
		Serial.println();
	#endif

	if (rightSpeed > OPPOSED_ENGINE_BREAK_LIMIT) {
		increaseTheSteeringAngle(leftEngine, rightEngine, rightSpeed);
	} else if (leftSpeed > OPPOSED_ENGINE_BREAK_LIMIT) {
		increaseTheSteeringAngle(rightEngine, leftEngine, leftSpeed);
	} else {
		leftEngine.moveForward(leftSpeed);
		rightEngine.moveForward(rightSpeed);
	}
}

void increaseTheSteeringAngle(Engine & inner, Engine & outer, int & outerSpeed) {
	if (outerSpeed >= MAX_SPEED) {
		inner.moveBackward(TOTAL_BREAK_SPEED);
	} else if (outerSpeed >= OPPOSED_ENGINE_HARD_BREAK_LIMIT) {
		inner.moveBackward(HARD_BREAK_SPEED); //hard stop
	} else {
		inner.stop();
	}
	outer.moveForward(outerSpeed);
}
