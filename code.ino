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

const int north = 6;  // beacon connections
const int east = 7;
const int west = 8;

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

  if (!MODE)  // task 1 mode
  {
    int sensorValue = analogRead(A0);
    Serial.println(sensorValue);
    error = sensorValue - 230;
    de = error - error_p;

    if(sensorValue < 120) // exit task 1
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
  else
  {
    irtrack();
  }

  error_p = error; 
} 



void drive(int rate) // drives the robot forward, and turns it left or right as needed
{
  if (rate > 10){
    //direction = 1; //turn left
    digitalWrite(EN1, LOW);
    digitalWrite(lm1, HIGH);
    digitalWrite(lm2, LOW);
    analogWrite(EN1, 200);
    
    digitalWrite(EN2, LOW);
    digitalWrite(rm1, LOW);
    digitalWrite(rm2, HIGH);
    analogWrite(EN2, 200);
  } 
  
  else if (rate < -10){
    //direction = -1; //turn right
    digitalWrite(EN1, LOW);
    digitalWrite(lm1, LOW);
    digitalWrite(lm2, HIGH);
    analogWrite(EN1, 200);

    digitalWrite(EN2, LOW);
    digitalWrite(rm1, HIGH);
    digitalWrite(rm2, LOW);
    analogWrite(EN2, 200);
  } 
  
  else {
    //direction = 0; // go forward
    analogWrite(EN1, 150);
    digitalWrite(lm1, HIGH); 
    digitalWrite(lm2, LOW);

    analogWrite(EN2, 150);
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

  if (sensorValueR > 400)
  {
    drive(15);
    delay(750);
    drive(0);
    delay(1000);

  }
  else if (sensorValueC > 400)
  {
    drive(15);
    delay(750);
    drive(0);
    delay(1000);
  }
  else if (sensorValueL > 400)
  {
    drive(-15);
    delay(750);
    drive(0);
    delay(1000);
  }
  else {
    //follow beacon
    if (digitalRead(east) == LOW)
    { 
      if(digitalRead(north) != LOW)
      {
      drive(-15);
      delay(500);
      }
    }

    else if (digitalRead(west) == LOW)
    {
      if(digitalRead(north) != LOW)
      {
      drive(15);
      delay(500);
      }
    }

    else
    {
      drive(0);
    }
  }
}