const int EN1 = 11;   // H bridge connections
const int EN2 = 10;
const int lm1 = 3;
const int lm2 = 2;
const int rm1 = 5;
const int rm2 = 4;

int velocity = 0;

float ir1 = A0; // right side ir sensor 
float ir2 = A1; // front right ir sensor
float ir3 = A2; // front left ir sensor
float ir4 = A4; // front center ir sensor

int val = 0;
bool on = false;
int error = 0; // the current error (i.e. how far away from optimal the robot is), gets moved to 'error_p' (previous error) at the end of each cycle.
int error_p = 0; // the previous error
int de = 0; // an approximation of the rate of change of error, equal to error - error_p

const int pb = 9;   // push button initialization

int direction = 0; //direction, should only be 0, +1 (left) or -1 (right)

bool MODE = false; //whether in wall-following or ir-following modes. 'false' is wall-following, 'true' is ir-following

void setup() 
{ 
  pinMode(EN1, OUTPUT);   // enable pin setup
  pinMode(EN2, OUTPUT);

  pinMode(lm1, OUTPUT);   // left motor setup
  pinMode(lm2, OUTPUT); 
  
  pinMode(rm1, OUTPUT);   // right motor setup
  pinMode(rm2, OUTPUT); 
  
  pinMode(pb, INPUT);    // button setup
  Serial.begin(9600);
} 



void loop()   // use button to start program
{
  while(!on)
  {
    val = digitalRead(pb);
    if(val == 1)
    {
      on = true;
    }
  }

  if (!MODE)
  {
    int sensorValue = analogRead(A0);
    Serial.println(sensorValue);
    error = sensorValue - 230;
    de = error - error_p;

    if(sensorValue < 120)
    {
      MODE = true;
    }

    else
    {
    // forward();
      drive(1*error+1*de); 
    }

    // delay(100);
  }
  else{
    irtrack()
  }
} 



void drive(int rate) // drives the robot forward, and turns it left or right as needed
{
  if (rate > 10){
    direction = 1; //turn left
    if (!MODE) 
    {
      digitalWrite(EN1, LOW);
      digitalWrite(lm1, HIGH);
      digitalWrite(lm2, LOW);
      analogWrite(EN1, rate);
    }

    digitalWrite(EN2, LOW);
    digitalWrite(rm1, LOW);
    digitalWrite(rm2, HIGH);
    analogWrite(EN2, rate);
  } 
  
  else if (rate < -10){
    direction = -1; //turn right
    digitalWrite(EN1, LOW);
    digitalWrite(lm1, LOW);
    digitalWrite(lm2, HIGH);
    analogWrite(EN1, rate);

    if (!MODE) 
    {
      digitalWrite(EN2, LOW);
      digitalWrite(rm1, HIGH);
      digitalWrite(rm2, LOW);
      analogWrite(EN2, rate);
    }
  } 
  
  else {
    direction = 0; // go forward
    analogWrite(EN1, 200);
    digitalWrite(lm1, HIGH); 
    digitalWrite(lm2, LOW);

    analogWrite(EN2, 200);
    digitalWrite(rm1, HIGH);
    digitalWrite(rm2, LOW);
  }
}

void stop() // braking function
{
  digitalWrite(EN1, LOW);
  digitalWrite(lm1, LOW);
  digitalWrite(lm2, LOW);
  digitalWrite(rm1, LOW);
  digitalWrite(rm2, LOW);
  digitalWrite(EN2, LOW);
}

void irtrack() //ir tracking function
{
  int sensorValueR = analogRead(A1);
  int sensorValueL = analogRead(A2);
  int sensorValueC = analogRead(A3);

  if (sensorValueR > 200 || sensorValueC > 200)
  {
    drive(15);
  }
  else if (sensorValueL > 200)
  {
    drive(-15);
  }
  else {
    //follow beacon
  }
}

