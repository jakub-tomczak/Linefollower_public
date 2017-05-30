#include "Error.h"
#include "Params.h"
Error::Error() :
	overshoot(NONE), current(0), previous(0), total(0) {}

bool Error::isOverShooted() const {
	return overshoot != NONE;
}

void Error::setOverShoot(OverShoot overShoot) {
	this->overshoot = overshoot;
}

void Error::setCurrentError(float & errorSum, int & activeSensorsNumber) {
	if (activeSensorsNumber != 0) {
		setNormalError(errorSum, activeSensorsNumber);
	} else {
		setOverShootError();
	}
	resetOverShootIfOnLine();
}

int Error::getErrorMultiplier() const {
	return isOverShooted() ? OVERSHOOT_WEIGHT_MULTIPLIER : NORMAL_WEIGHT_MULTIPLIER;
}

void Error::setOverShootError() {
	if (previous < -ERROR_OVERSHOOT_LIMIT) {
		current = previous < -ERROR_STRAIGHT_LIMIT ? -ERROR_STRAIGHT_ANGLE : -ERROR_AFTER_OVERSHOOT;
		setOverShoot(LEFT);
	} else if (previous > ERROR_OVERSHOOT_LIMIT) {
		current = previous >  ERROR_STRAIGHT_LIMIT ? ERROR_STRAIGHT_ANGLE : ERROR_AFTER_OVERSHOOT;
		setOverShoot(RIGHT);
	} else {
		current = 0;
	}
}

bool isBetween(int & value, int lower, int upper) {
	return value >= lower && value <= upper;
}

void Error::setNormalError(float& errorSum, int& activeSensorsNumber) {
	if (isBetween(activeSensorsNumber, 3, 5) && errorSum != 0) {
		current = errorSum < 0 ? -ERROR_STRAIGHT_ANGLE : ERROR_STRAIGHT_ANGLE;
	} else {
		current = errorSum / float(activeSensorsNumber);
	}
}

void Error::resetOverShootIfOnLine() {
	if ((overshoot == LEFT && current >= 0) || // when there is no longer overshoted - lf is back on line
		(overshoot == RIGHT && current <= 0)) {
		setOverShoot(NONE);
	}
}
