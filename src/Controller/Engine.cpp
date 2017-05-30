#include "Engine.h"

Engine::Engine(int pinForward, int pinBackward, int pinSpeed):
	pinForward(pinForward), pinBackward(pinBackward), pinSpeed(pinSpeed), speed(0) {
	pinMode(pinForward, OUTPUT);
	pinMode(pinBackward, OUTPUT);
	pinMode(pinSpeed, OUTPUT);
}

void Engine::moveForward(int speed) const {
	digitalWrite(pinForward, HIGH);
	digitalWrite(pinBackward, LOW);
	analogWrite(pinSpeed, speed);
}

void Engine::moveBackward(int speed) const {
	digitalWrite(pinForward, LOW);
	digitalWrite(pinBackward, HIGH);
	analogWrite(pinSpeed, speed);
}

void Engine::stop() const {
	digitalWrite(pinForward, LOW);
	digitalWrite(pinBackward, LOW);
}
