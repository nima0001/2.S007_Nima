//LINE FOLLWING SENSOR VALUES TESTING:
int pin_arr[5] = {A8,A9,A10,A11,A12}; //Analog pins assignments for each sensor cell
float y_prime; //current y_val ranging from 0 to 5 (target y_val = 3)
const float minim = 50; //min raw output of sensors
const float maxim = 1000; //max raw output of sensors
const float non_black_threshold = 0.85; //y value after which bot is confident that given cell is above non-black surface 
const float black_threshold = 0.65; //confidently black below this value
int n; //number of sensors in black line
  
int black_cross_num; //number of times bot has crossed horizontal black lines since the start of the program


//PID Variables:
float y = 3.0; //target value
const float delta_t = 50.0;//Time between subsequent execution of controller
float integral; //integral(y) value
float previousError; //for derivative calculation de/dt = (e1 - e0)/dt

const float Kp = 140.0; //Proportional parameter 
const float Ki = 0.0;     //Integral Parameter
const float Kd = 40.0;  //Derivative parameter 



//LED:
#define LED 42


//MOTION CONTROL:
#include <Adafruit_MotorShield.h>
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *motorLeft = AFMS.getMotor(1);
Adafruit_DCMotor *motorRight = AFMS.getMotor(2);

const int Speed = 100; 
float frac = 0.95; //left_speed = frac* right_speed





void setup() {
  Serial.begin(9600);
  
  previousError = 0;
  integral = 0;
  black_cross_num = 0;

  //Line following sensor:
  for (int i = 0; i<5; i++) {
    pinMode(pin_arr[i],INPUT);
  }
  //LED:
  pinMode(LED , OUTPUT);

  //Motors:
  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
    // if (!AFMS.begin(1000)) {  // OR with a different frequency, say 1KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found."); 

  delay(3000);
}





void loop() {
  float StartTime = micros();
   
  y_prime = get_yprime(); //get current y-value //takes about 30ms 
  Serial.println(y_prime); 
   
  if (n == 5) { //if all sensors detect black surface
    communicate_status(1); //blink LED as bot crosses horizontal lines and stop bot's motion when it reaches destination
  }
  
  else{ 
    PID(y_prime); 
    
    //NO CODES DOWN HERE: making sure PID execution Time period/frequency is constant
    float CurrentTime = micros();
    float elapsedTime = ((CurrentTime - StartTime))/1000.0;
    delay(delta_t - elapsedTime);    
  }
  
}



/*
************************************************************************************************************************************************
*/

float get_yprime() {
  /*
   * returns number between 0.0 to 5.0 
   * 0.0 means bot is NOT in black line
   * 1.0 to 5.0 indicates position of line following sensor in black line
   */
  float y_prime = 0.0; //output from sensor (to be updated)
  float sum = 0.0;
  float y_raw;  //raw analog output values from sensors
  n = 0;
  for (int i = 0; i<5; i++) { 
    if (i == 0) {y_raw = (float) analogRead(pin_arr[i]) +300; }  //adjust offset on sensor 1
    else{y_raw = (float) analogRead(pin_arr[i]);}      
    float yi = (y_raw - minim)/(maxim-minim); 
    
    if (yi >= non_black_threshold) {yi = 1;} 
    else if (yi <= black_threshold) { yi = 0; n++;} 
    
    sum += yi;  
    y_prime += (1- yi)*i;      
  } 
  
  y_prime = y_prime/(5 - sum) + 1; 

  if (isnan(y_prime)) { 
    Serial.println("Bot in non-black region!"); 
    y_prime = 0.0;  
  }
  return y_prime;
}



void PID(float yp) {
  /*
   * Implement PID controller given current y_value
   */
  float error = y - yp; 

  integral += error * delta_t; //right reimann sum or right rectangle piece-wise approx.
  float derivative = (error - previousError)/delta_t; //forward difference or forward Euler

  float del = Kp*error + Ki*integral + Kd*derivative;
  previousError = error;
  drive(Speed - del, Speed + del); //move wheel to adjust y value  
}



void communicate_status(int mode) {
    brake();
    black_cross_num++; //increase counter for number of black lines crossed
    blink_led(black_cross_num);   

    if (mode == 1) { //a1 to a2
      if (black_cross_num < 7) {
        drive_forward(Speed);
      }
      else{
        brake(); 
        delay(5000);
      }
    }

    else if (mode == 2) { //a3 to a4
      if (black_cross_num < 16) {
        drive_forward(Speed);
      }
      else {
        brake(); 
        delay(5000);
      } 
    }
      
    delay(200); //  //to cross that line without using PID
}



/*
************************************************************************************************************************************************
*/


//MOTION CONTROL FUNCTIONS:
void drive(int left_speed, int right_speed) {
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
  for (int i = 0; i<n; i++) {
    digitalWrite(LED , HIGH);//turn the LED On by making the voltage HIGH
    delay(300);                       // wait half a second
    digitalWrite(LED , LOW);// turn the LED Off by making the voltage LOW
    delay(300);    
  }
}



