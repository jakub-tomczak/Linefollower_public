#include "Sensor.h"


Sensor::Sensor(int pin, float weight):
	pin(pin), result(0), weight(weight) {
	pinMode(pin, INPUT);
}

void Sensor::read() {
	result = digitalRead(pin);
}

float Sensor::getValue() const {
	return result * weight;
}

int Sensor::getResult() const {
	return result;
}
