//LINE FOLLWING SENSOR VALUES TESTING:
int pin_arr[5] = {A8,A9,A10,A11,A12}; //Analog pins assignments for each sensor cell
float y_arr[5]; //array for outputs from each line following cell

const float minim = 50; //todo
const float maxim = 1000;
float y_raw; float zi;

const float non_black_threshold = 0.85; //update y value after which bot is extremely confident that cell is not black todo
const float black_threshold = 0.65; //confidently non-black

//PID Variables:
float integral;  // error 
float previousError;


float Kp = 140.0; //Proportional parameter 
float Ki = 0.0;     //Integral Parameter
float Kd = 40.0;  //Derivative parameter 
float delta_t = 3.0;//Time between subsequent change





  
void setup() {
  Serial.begin(9600);

  previousError = 0;
  integral = 0;

  
  //Line following sensor:
  for (int i = 0; i<5; i++) {
    pinMode(pin_arr[i],INPUT);
  }
}

void loop() {
  float y_prime = 0.0; //output from sensor (to be updated)
  float sum = 0.0;
    
  for (int i = 0; i<5; i++) { 
    if (i == 0) {y_raw = (float) analogRead(pin_arr[i]) +300; }  //adjust offset on sensor 1
    else{y_raw = (float) analogRead(pin_arr[i]);}         
    float yi = (y_raw - minim)/(maxim-minim); 
    
    if (yi >= non_black_threshold) {yi = 1;} 
    else if (yi <= black_threshold) { yi = 0;} 
    y_arr[i] = yi;
    sum += yi;  
    y_prime += (1- yi)*i;      
  }
  y_prime = y_prime/(5 - sum) + 1;
  if (isnan(y_prime)) { Serial.println("Not in black line");}
  else {Serial.println(y_prime); }
  
  
  
  Serial.println(); 
  delay(2000);
  
}

/*
 * non-black over 0.90
 * black below 0.65
 */
