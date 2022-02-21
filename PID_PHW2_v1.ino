//LINE FOLLWING SENSOR:
int pin_arr[5] = {A8,A9,A10,A11,A12}; //Analog pins assignments for each sensor cell
float y_arr[5]; //array for outputs from each line following cell

//assuming binary inout (other colors can be thresholded)
const float maxim = 990.0; //Max value of sensor //todo
const float minim = 150.0; //Min value of sensor
const float non_black_threshold = 0.70; //update y value after which bot is extremely confident that cell is not black todo
const float black_threshold = 0.20; //confidently non-black



//MOTION CONTROL:
const int Speed = 100;
#include <Adafruit_MotorShield.h>
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); // Create the motor shield object

// Set port M1 as the left motor and port M2 as the right motor
Adafruit_DCMotor *motorLeft = AFMS.getMotor(1);
Adafruit_DCMotor *motorRight = AFMS.getMotor(2);
float frac = 1; //left_speed = frac* right_speed


//PID Variables:
int y = 3; //target value 
float integral;  // error 
float previousError;


float Kp = 140.0; //Proportional parameter 
float Ki = 0.0;     //Integral Parameter
float Kd = 40.0;  //Derivative parameter 
float delta_t = 1.5;//Time between subsequent change


float t_start;
float t_end;








int mode = 1; //for PHW 2 
bool over = false;

 int no_black_cross;

void setup() {
  previousError = 0;
  integral = 0;
  no_black_cross = 0;
  
  Serial.begin(9600);
  
  //Line following sensor:
  for (int i = 0; i<5; i++) {
    pinMode(pin_arr[i],INPUT);
  }
}





void loop() {
   if (over == false) {

    float StartTime = micros();

    int n = 0; //how many cells in black line
    float y_prime = 0.0; //output from sensor (to be updated)
    float sum = 0.0;

    for (int i = 0; i<5; i++) {  
      float yi = ((float)analogRead(pin_arr[i]) - minim)/(maxim-minim); //scaling between 0 and 1
      if (yi >= non_black_threshold) {yi = 1;} //todo
      else if (yi <= black_threshold) {n +=1;} //todo
      y_arr[i] = yi;
      sum += yi;  
      y_prime += (1- yi)*i;  
    }
    


    if (n == 5) { //black line crossed
      float t = millis();
      brake();
      no_black_cross+=1;
      blink_led(no_black_cross);


      if (mode == 1) { //a1 to a2
        if (no_black_cross < 7) {
          drive_forward(Speed);
        }
        else{over = true;}
      }

      else if (mode == 2) { //a3 to a4
        if (no_black_cross < 16) {
          drive_forward(Speed);
        }
        else {over = true;} 
      }
          
      delay(200 - t); //todo  //to cross that line without using PID
    }


    else {
      y_prime = y_prime/(5 - sum) + 1.0;
      float error = y - y_prime;  
      
      //PID:
      integral += error * delta_t; //right reimann sum or right rectangle piece-wise approx.
      float derivative = (error - previousError)/delta_t; //forward difference or forward Euler
    
      float del = Kp*error + Ki*integral + Kd*derivative;
      previousError = error;

      drive(Speed + del, Speed - del); //TODO

      //NO CODES DOWN HERE:
      //MAKING SURE time difference is exactly delta_t
      float CurrentTime = micros();
      float elapsedTime = ((CurrentTime - StartTime))/1000.0;
      delay(delta_t - elapsedTime); //For the difference in times between two subsequent output to be 'exactly' 1000 milliseconds 
    }
  } 
}





//MOTION CONTROL FUNCTIONS:
void drive(int left_speed, int right_speed) {
  /*
   * Runs forward with given motorspeed until stopped
   */
  motorLeft->run(FORWARD); motorRight->run(FORWARD);
  
  motorLeft->setSpeed(left_speed*frac);  
  motorRight->setSpeed(right_speed); 
}

void brake() {
  motorLeft->run(RELEASE); 
  motorRight->run(RELEASE); 
}

void drive_forward(int motorSpeed) {
  /*
   * Runs forward with given motorspeed until stopped
   */
  motorLeft->run(FORWARD); motorRight->run(FORWARD);
  
  motorLeft->setSpeed(motorSpeed*frac);  
  motorRight->setSpeed(motorSpeed); 
}

void drive_backward(int motorSpeed) {
  /*
   * Runs backward with given motorspeed until stopped
   */
  motorLeft->run(BACKWARD); motorRight->run(BACKWARD);

  motorLeft->setSpeed(motorSpeed*frac);  
  motorRight->setSpeed(motorSpeed); 
}


//LED blink count black crossings:
void blink_led(int n){
  //TODO
}
    
