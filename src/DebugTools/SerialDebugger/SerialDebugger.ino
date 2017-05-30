#include <SomeSerial.h>
#include <SoftwareSerial.h>


// specify txPin && rxPin as in your connection. 
SoftwareSerial mySerial(10,11); // RX, TX

const int baud = 19200;

void setup(){
    Serial.begin(baud);
    while(!Serial){}
    Serial.println("Goooooooooooooooooooooood morning Vietnam! Let's debug!\n\n\n");
    mySerial.begin(baud);
}

void loop(){
    if(mySerial.available()){
        Serial.write(mySerial.read());
    }
    if(Serial.available()){
        mySerial.write(Serial.read());
    }


}
