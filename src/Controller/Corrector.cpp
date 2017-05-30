#include "Corrector.h"
#include "Params.h"
Corrector::Corrector(Sensor * sensors, const int & numberOfSensors, float & KP, float& KD, float& KI) :
	sensors(sensors), numberOfSensors(numberOfSensors),
	KP(KP), KD(KD), KI(KI) {}


int Corrector::getSpeedDifference() {
	readFromSensors();
	countError();
	return computeSpeedDifference();
}

float Corrector::getCurrentError() const {
	return error.current;
}

void Corrector::resetError() {
	error = Error();
}
#if SERIAL_DEBUG
void Corrector::serialDebug() {
	for (int i = 0; i < numberOfSensors; i++) {
		Serial.print(sensors[i].getResult());
		Serial.print("\t");
	}
	Serial.print("\n error ");
	Serial.print(error.current);
	Serial.print(" prEr ");
	Serial.print(error.previous);
	Serial.print(" totalEr ");
	Serial.print(error.total);
}
#endif

void Corrector::readFromSensors() const {
	for (uint8_t id = 0; id < numberOfSensors; id++) {
		sensors[id].read();
	}
}

void Corrector::countError() {
	float errorSum = 0;
	int weightMultiplier = error.getErrorMultiplier();
	int activeSensorsNumber = 0;
	for (int id = 0; id < NUMBER_OF_SENSORS; id++) {
		errorSum += sensors[id].getValue() * weightMultiplier;
		activeSensorsNumber += sensors[id].getResult();
	}
	error.setCurrentError(errorSum, activeSensorsNumber);
}

int Corrector::computeSpeedDifference() {
	int derivative = error.current - error.previous;
	error.previous = error.current;
	error.total += error.current;
	return KP * error.current + KD * derivative + KI * error.total;
}