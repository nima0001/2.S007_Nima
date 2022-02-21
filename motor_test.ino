#include <Adafruit_MotorShield.h>
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); // Create the motor shield object

// Set port M1 as the left motor and port M2 as the right motor
Adafruit_DCMotor *motorLeft = AFMS.getMotor(1);
Adafruit_DCMotor *motorRight = AFMS.getMotor(2);
float frac = 1; //left_speed = frac* right_speed


boolean testOver = false; // Set a boolean that indicates the test is complete


void setup() {
  Serial.begin(9600);         
}

void loop() {
  if (testOver == false) {
    drive_forward(100);
    delay(5000);
    brake();
    
    drive_backward(100);
    delay(5000);
    brake();


    //No code below here
    testOver = true;
  }
}


void brake() {
  motorLeft->run(RELEASE); 
  motorRight->run(RELEASE); 
}

void drive_forward (int motorSpeed) {
  /*
   * Runs forward with given motorspeed until stopped
   */
  motorLeft->run(FORWARD); motorRight->run(FORWARD);
  
  motorLeft->setSpeed(motorSpeed*frac);  
  motorRight->setSpeed(motorSpeed); 
}



void drive_backward (int motorSpeed) {
  /*
   * Runs backward with given motorspeed until stopped
   */
  motorLeft->run(BACKWARD); motorRight->run(BACKWARD);

  motorLeft->setSpeed(motorSpeed*frac);  
  motorRight->setSpeed(motorSpeed); 
}
