#include <SoftwareSerial.h>
#include <Servo.h>

SoftwareSerial hovercraft(9, 10);

// Define Motors
Servo hover_motor;
Servo motion_motor;
Servo control_motor;

// Constants
int MOTION_ACC = 10;
int RUDDER_ACC = 5;
int MOTION_MAX_SPEED = 180;
int MAX_HOVER = 170; // MAX = 180
int TURBO_ACC = 2 * MOTION_ACC;
int MAX_RUDDER_ANGLE = 135;
int MIN_RUDDER_ANGLE = 45;
int DEFAULT_RUDDER_ANGLE = 90;

// Variables
int rudder_angle = 90;
int velocity = 0;
int acceleration  = 0;
bool isTurbo = false;

void setup() {
  Serial.begin(9600);
  hovercraft.begin(9600);
  hover_motor.attach(11);
  motion_motor.attach(12);
  control_motor.attach(13);
}

void loop() {
  if (hovercraft.available()){
    velocity += min(MOTION_MAX_SPEED, acceleration + velocity);
    motion_motor.write(velocity);
    delay(100);
    int num = hovercraft.read();
    switch(num){
        break;
      case 117:
        // Serial.println("UP");
        if(velocity == 0){
          hover_motor.write(MAX_HOVER);
        }
        if(isTurbo){
          acceleration = TURBO_ACC;
        } else {
          acceleration = MOTION_ACC;
        }
        break;
      case 114:
        // Serial.println("RIGHT");
        for(; rudder_angle <= 135; rudder_angle += RUDDER_ACC){
          control_motor.write(min(rudder_angle, MAX_RUDDER_ANGLE));
          delay(5);
        }
        break;
      case 100:
        // Serial.println("DOWN");
        acceleration = -2 * MOTION_ACC;
        break;
      case 108:
        // Serial.println("LEFT");
        for(; rudder_angle >= 45; rudder_angle -= RUDDER_ACC){
          control_motor.write(max(rudder_angle, MIN_RUDDER_ANGLE));
          delay(5);
        }
        break;
      case 116:
        // Serial.println("Triangle");
        isTurbo = !isTurbo;
        break;
      case 120:
        // Serial.println("X");
        if(velocity != 0){
          motion_motor.write((int) velocity/2);
          control_motor.write(90);
          delay(50);
          motion_motor.write(0);
        } else {
          hover_motor.write(0);
        }
        break;
      case 48:
        // Serial.println("0");
        if(rudder_angle != DEFAULT_RUDDER_ANGLE){
          control_motor.write(rudder_angle);
          rudder_angle = DEFAULT_RUDDER_ANGLE;
        } else {
          acceleration = 0;
        }
        break;
      default:
        Serial.println(num);
    }
  }
}
