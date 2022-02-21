float frac = 1.0; //left_speed = frac* right_speed





#include <Adafruit_MotorShield.h>
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); // Create the motor shield object

// Set port M1 as the left motor and port M2 as the right motor
Adafruit_DCMotor *motorLeft = AFMS.getMotor(1);
Adafruit_DCMotor *motorRight = AFMS.getMotor(2);

boolean testOver = false; // Set a boolean that indicates the test is complete


void setup() {
  Serial.begin(9600);         
}

void loop() {
  if (testOver == false) {
    //Check if my bot's left motor is motorLeft
    motorLeft->run(FORWARD); motorLeft->setSpeed(100);   delay(2000); brake(); delay(3000);
    
    
    //Adjust two wheel's speed relative to each other in same voltage
    drive_forward(100);
    delay(5000);
    brake();
    
//     drive_backward(100);
//     delay(5000);
//     brake();


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
