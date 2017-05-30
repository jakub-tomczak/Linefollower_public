// Params.h
#ifndef _PARAMS_h
#define _PARAMS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

/// ********* CONTROLS ***************
#define SERIAL_DEBUG false
#define SET_PARAMS false
#define SONAR_ENABLED true
/// **********************************

/// ********** PINS ******************
// ########## MOTORS #################
#define MOTOR_LEFT_FORWARD 2
#define MOTOR_LEFT_SPEED 3
#define MOTOR_LEFT_BACKWARD 7
#define MOTOR_RIGHT_FORWARD 4
#define MOTOR_RIGHT_SPEED 6
#define MOTOR_RIGHT_BACKWARD 5

// ########## SENSORS ################
#define NUMBER_OF_SENSORS 8

#define SENSOR_1_PIN A0
#define SENSOR_2_PIN A1
#define SENSOR_3_PIN A2
#define SENSOR_4_PIN A3
#define SENSOR_5_PIN A4
#define SENSOR_6_PIN A5
#define SENSOR_7_PIN 13
#define SENSOR_8_PIN 12

// ########### SONAR #################
#define SONAR_TRIG_PIN 8
#define SONAR_ECHO_PIN 9
/// **********************************

/// ********** PARAMS ****************
// ########### SPEEDS ################
#define MAX_SPEED 255
#define OPPOSED_ENGINE_HARD_BREAK_LIMIT 220
#define HARD_BREAK_SPEED 50
#define TOTAL_BREAK_SPEED 220
#define OPPOSED_ENGINE_BREAK_LIMIT 190
#define SPEED_LIMIT 255
#define BASE_SPEED 130

// ########### ERRORS ################
#define ERROR_OVERSHOOT_LIMIT 2
#define ERROR_STRAIGHT_LIMIT 8
#define ERROR_AFTER_OVERSHOOT 6
#define ERROR_STRAIGHT_ANGLE 10

// ########## WEIGHTS ################
#define SENSOR_WEIGHT_4 4.25
#define SENSOR_WEIGHT_3 2.25
#define SENSOR_WEIGHT_2 1
#define SENSOR_WEIGHT_1 0.25

#define NORMAL_WEIGHT 2
#define OVERSHOOT_WEIGHT 1

#define NORMAL_WEIGHT_MULTIPLIER 2
#define OVERSHOOT_WEIGHT_MULTIPLIER 1
/// **********************************

/// ********** OTHERS ****************
#define MAX_DISTANCE 20
#define AVOIDANCE_TURN_TIME 135
/// **********************************

#if SET_PARAMS
void insertParams();
void checkParams();
#endif
#endif

