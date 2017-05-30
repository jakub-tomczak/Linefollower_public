#define ITERATIONS 100

int sensorPin1 = A0;
int sensorPin2 = A1;

int value = 0;
int value2 = 0;
    unsigned int startTime = 0;
    unsigned int stopTime = 0; 
    
void setup() {
  Serial.begin(9600);
  pinMode(sensorPin1, INPUT);
  pinMode(sensorPin2, INPUT);
}
unsigned long sixTimes = 0;
unsigned long sixTimesOneAnalog = 0;
float avg = 0;

void loop() {
    sixTimes = 0;
    sixTimesOneAnalog = 0;
  
  for(int i=0;i<ITERATIONS;i++)
  {
        startTime = micros();
        readSixTimes();
        stopTime = micros();
        sixTimesOneAnalog += stopTime-startTime;

        startTime = micros();
        readSix();
        stopTime = micros();
        sixTimes += stopTime-startTime;
  }

  Serial.println(sixTimesOneAnalog);
  Serial.println(sixTimes);
  

  displayResult("Reading from one pin",(double)sixTimesOneAnalog/ITERATIONS);

  displayResult("Reading 6 times from diffrent pins",(double)sixTimes/ITERATIONS);

  Serial.println("---------------------------------");
  delay(2000);
}

void readSixTimes()
{
  for(int i=0;i<6;i++)
  {
    digitalRead(1);
  }
}

void readSix()
{
  analogRead(A0);
  analogRead(A1);
  analogRead(A2);
  analogRead(A3);
  analogRead(A4);
  analogRead(A5);
}

void displayResult(char * title, double avg)
{
  Serial.println("**********");
  
  Serial.println(title);
  Serial.print("Avg 6. ");
  Serial.println(avg);
  Serial.print("Avg 1. ");
  Serial.println(avg/6);

  Serial.println("**********");

}

