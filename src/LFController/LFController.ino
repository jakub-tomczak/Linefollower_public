#include <PID_v1.h>
//#include <NewPing.h>
//#include <PID/PID_v1.h>
void initializePins();
void initializeSpeed();
void setEnginesSpeeds();
void readFromSensors();
int getLeftError();
int getRightError();
void debugPrint();
void setError();
void setSpeeds();
void setLeftEngine();
void setRightEngine();
void initializePid();

#define CENTER 3
#define NUMBER_OF_SENSORS 7

#define MOTOR_LEFT_FORWARD 2
#define MOTOR_LEFT_SPEED 3
#define MOTOR_LEFT_BACKWARD 7
#define MOTOR_RIGHT_FORWARD 4
#define MOTOR_RIGHT_SPEED 6
#define MOTOR_RIGHT_BACKWARD 5

#define MOTORS_CONSTANTS 6
#define MAX_ERROR 6
#define MAX_SPEED 255
#define BASE_SPEED 100
#define SPEED_DIFFERENCE 50
#define MULTIPLIER 1.5
#define DEBUG true
#define SERIAL_DEBUG true
#define DELAY_BETWEEN_READINGS 30

float KP = 0.0;
float KD = 0.0;
float KI = 0.0;

int weight[] = { -3, -2, -1, 0, 1, 2, 3 };
int sensors[] = { A0, A1, A2, A3, A5, A4, 13 };
int motors[] = {
  MOTOR_LEFT_FORWARD, MOTOR_LEFT_BACKWARD, MOTOR_LEFT_SPEED,
  MOTOR_RIGHT_FORWARD, MOTOR_RIGHT_BACKWARD, MOTOR_RIGHT_SPEED
};

#define TRIG_PIN 8
#define ECHO_PIN 9
#define MAX_DISTANCE 30
//NewPing sonar(TRIG_PIN, ECHO_PIN, MAX_DISTANCE);

int sensorsResults[NUMBER_OF_SENSORS];
double leftSpeed, rightSpeed;
int distanceFromObstacle;
double error = 0;
double difference = 0;
double setPoint = 0;
PID pid(&error, &difference, &setPoint, KP, KI, KD, DIRECT);
;/** INITIALIZATION */
void setup() {
  if (SERIAL_DEBUG)
  {
    Serial.begin(9600); //bluetooth accepts only 19200
    Serial.write("START!\n");
  } 
  insertParams();
  checkParams();
  initializePins();
  initializePid();
  initializeSpeed();
}

void insertParams(){
  Serial.println("Give me KP");
  while(!Serial.available()){}
  KP = Serial.parseFloat(); 
  Serial.println("Give me KD");
  while(!Serial.available()){}
  KD = Serial.parseFloat(); 
  Serial.println("Give me KI");
  while(!Serial.available()){}
  KI = Serial.parseFloat();
}

void checkParams(){
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

void initializePins() {
  for (uint8_t id = 0; id < MOTORS_CONSTANTS; id++) {
    pinMode(motors[id], OUTPUT);
  }
  for (uint8_t id = 0; id < NUMBER_OF_SENSORS; id++) {
    pinMode(sensors[id], INPUT);
  }
}

void initializeSpeed() {
  digitalWrite(MOTOR_RIGHT_FORWARD, HIGH);
  digitalWrite(MOTOR_RIGHT_BACKWARD, LOW);
  digitalWrite(MOTOR_LEFT_FORWARD, HIGH);
  digitalWrite(MOTOR_LEFT_BACKWARD, LOW);

  for (uint8_t speed = 0; speed <= BASE_SPEED * 1.25; speed++) {
    leftSpeed = speed;
    rightSpeed = speed;
    setEnginesSpeeds();
    delay(5);
  }
}

void initializePid()
{
  pid.SetMode(AUTOMATIC);
  pid.SetSampleTime(10);
  pid.SetOutputLimits(-MAX_ERROR, MAX_ERROR);
  pid.Compute();
}


/** MAIN PROGRAM */
void loop() {
  readFromSensors();
  setError();
  pid.Compute();
  setSpeeds();
  setEnginesSpeeds();
  if (SERIAL_DEBUG) {
    debugPrint();
  }
}

void readFromSensors() {
  for (uint8_t id = 0; id < NUMBER_OF_SENSORS; id++) {
    sensorsResults[id] = digitalRead(sensors[id]);
  }
}
float numberOfDetectedSensors;
void setError() {
  numberOfDetectedSensors = 0;                        // ilo�� czujnik�w z wykryt� lini� 

  for (int i = 0; i < NUMBER_OF_SENSORS; i++)
  {
    error += sensorsResults[i] * weight[i];        // wagi kolejnych czujnik�w (dla i z zakresu [0;7]): 
    numberOfDetectedSensors += sensorsResults[i];            // czujniki[i] ma warto�� 1/0 
  }
  if (numberOfDetectedSensors == 0 || numberOfDetectedSensors == NUMBER_OF_SENSORS) {
    error = 0;
  }
  else {
    ;//error /= numberOfDetectedSensors;
  }
}

void setSpeeds() {
  int difference = map(error, -MAX_ERROR, MAX_ERROR, -SPEED_DIFFERENCE, SPEED_DIFFERENCE);
  leftSpeed = BASE_SPEED - difference;
  rightSpeed = BASE_SPEED + difference;
}

void setEnginesSpeeds() {
  setLeftEngine();
  setRightEngine();
}

void setLeftEngine() {
  analogWrite(MOTOR_LEFT_SPEED, leftSpeed);
}

void setRightEngine() {
  analogWrite(MOTOR_RIGHT_SPEED, rightSpeed);
}

int getLeftError() {
  int sumOfWeights = 0;
  for (uint8_t id = 0; id < CENTER; id++) {
      sumOfWeights+= sensorsResults[id]*weight[id];// -(sensorsResults[id + 1] * weight[id + 1]) - 1;
  }
  return sumOfWeights;
}

int getRightError() {
  int sumOfWeights = 0;
  for (uint8_t id = NUMBER_OF_SENSORS - 1; id < CENTER; id--) {
    sumOfWeights += sensorsResults[id] * weight[id];// -(sensorsResults[id + 1] * weight[id + 1]) - 1;
  }
  return sumOfWeights;
}

long lastTime = 0;
void debugPrint() {
  static uint16_t delayer1 = 0;
  static uint16_t delayer2 = 0;
  if (delayer1 != 0xFF){   
    ++delayer1;
    return;
  }
  else if (delayer2 != 0xFF){
    ++delayer2;
    return;
  }
  
  char buff[50];
  for (uint8_t i = 0; i < NUMBER_OF_SENSORS; i++) {
    //Serial.print(i);
    //Serial.write(":");
    //Serial.print(sensorsResults[i]);
    //Serial.write("\t");
    sprintf(buff, "sensor %d : %d\n", i, sensorsResults[i]);
    Serial.write(buff);
  }
  //Serial.write("\n");
  //Serial.write("leftSpeed ");
  //Serial.print(int(leftSpeed));
  //Serial.write(" ; rightSpeed ");
  //Serial.print(int(rightSpeed));
  //Serial.write(" ; error ");
  //Serial.print(int(error));

  sprintf(buff, "LeftSpeed : %d ; RightSpeed : %d ; error : %d \n", int(leftSpeed), int(rightSpeed), int(error));
  Serial.write(buff);

  delayer1 = 0;
  delayer2 = 0;
  //  sonar.timer_ms(50, checkDistance);
  //  sonar.timer_stop();
  //  sonar.ping_timer(checkDistance);
  //  distanceFromObstacle = sonar.ping_cm();
  //  Serial.write(" sonar : ");
  //  Serial.write(distanceFromObstacle);
  //
  //  Serial.write(" read time : ");
  //  long currentTime = millis();
  //  Serial.write(currentTime - lastTime);
  //  Serial.writeln("");
  //  lastTime = currentTime;

  //  delay(DELAY_BETWEEN_READINGS);
}

//void checkDistance()
//{
//  if (sonar.check_timer())
//  {
//    distanceFromObstacle = sonar.ping_cm();
//  }
//}
