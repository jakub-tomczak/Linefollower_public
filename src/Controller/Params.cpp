#include "Params.h"

#if SET_PARAMS

extern float KP;
extern float KD;
extern float KI;
void insertParams() {
	Serial.println("Give me KP");
	while (!Serial.available()) {}
	KP = Serial.parseFloat();
	Serial.println("Give me KD");
	while (!Serial.available()) {}
	KD = Serial.parseFloat();
	Serial.println("Give me KI");
	while (!Serial.available()) {}
	KI = Serial.parseFloat();
}

void checkParams() {
	Serial.print("Hello, given nums are \n KP: ");
	Serial.print(KP);
	Serial.print("\n");
	Serial.print("KD: ");
	Serial.print(KD);
	Serial.print("\n");
	Serial.print("KI: ");
	Serial.print(KI);
	Serial.print("\n");
}
#endif