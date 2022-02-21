//LINE FOLLWING SENSOR VALUES TESTING:
int pin_arr[5] = {A8,A9,A10,A11,A12}; //Analog pins assignments for each sensor cell
float y_arr[5]; //array for outputs from each line following cell

const float minim = 150; //todo
const float maxim = 990;


void setup() {
  Serial.begin(9600);
  //Line following sensor:
  for (int i = 0; i<5; i++) {
    pinMode(pin_arr[i],INPUT);
  }
}

void loop() {
  for (int i = 0; i<5; i++) {  
    float y_raw = (float) analogRead(pin_arr[i]);
    Serial.println(y_raw);    
    float zi = (y_raw - minim)/(maxim-minim);
    
  }
}
