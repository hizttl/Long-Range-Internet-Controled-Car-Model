#include <AFMotor.h>
#include <SoftwareSerial.h>

AF_DCMotor motor3(3);
AF_DCMotor motor4(4);

SoftwareSerial s(10,9);

int message;

void setup() {
  // put your setup code here, to run once:
  motor3.setSpeed(200);
  motor4.setSpeed(200);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
  Serial.begin(9600);
  s.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("acc");
  if (s.available()) {
    while(s.available()){
      message = s.read();
    }
    Serial.println(message);
    
    delay(100);
    if (message == 0) {
      motor3.run(RELEASE);
      motor4.run(RELEASE);
    } else if (message == 1) {
      motor3.run(FORWARD);
      motor4.run(FORWARD);
      motor3.setSpeed(200);
      motor4.setSpeed(200);
      delay(1000);
      motor3.run(RELEASE);
      motor4.run(RELEASE);
    }
  }
}
