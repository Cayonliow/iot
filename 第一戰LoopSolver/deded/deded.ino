










//IN1,2 control left motor
const int motorIn1 = 2;
const int motorIn11 = 3;

//IN3,4 control right motor
const int motorIn2 = 5;
const int motorIn22 = 4;

//front sensor
const int trigF = 6;
const int echoF = 7;

//left sensor
const int trigR = 8;
const int echoR = 9;

//right sensor
const int trigL = 10;
const int echoL = 11;

const int find=A7;

const int inter_time = 100;
  int time = 0;

int goodD=8;

int R;

//make the motor stop

//turn right
void turn_left(int m,int d)
{
   analogWrite(motorIn1, 0);
   analogWrite(motorIn11, d);
   analogWrite(motorIn2, 0);
   analogWrite(motorIn22, d);
   delay(m);
}

//turn left
void turn_right(int m, int d)
{
   analogWrite(motorIn1, d);
   analogWrite(motorIn11, 0);
   analogWrite(motorIn2, d);
   analogWrite(motorIn22, 0);
   delay(m);
}


void stop()
{
   analogWrite(motorIn1, 0);
   analogWrite(motorIn11, 0);
   analogWrite(motorIn2, 0);
   analogWrite(motorIn22, 0);
}
void move_left(int LEFT)
{
   while(distancef()-LEFT>=3||distancef()-LEFT<=-3)
   {
      turn_left(0,150);
   }
   move_forward(800,200);
}

//move forward
void move_forward(int m,int x)
{
  analogWrite(motorIn1, x);
  analogWrite(motorIn11, 0);
  analogWrite(motorIn2, 0);
  analogWrite(motorIn22, x+10);
  delay(m);
}

//reverse
void move_backward(int m, int x)
{
  analogWrite(motorIn1, 0);
  analogWrite(motorIn11, x);
  analogWrite(motorIn2, x);
  analogWrite(motorIn22, 0);
  delay(m);
}

//calculate the distance from the FRONT
float distancef()
{
  float durationF, distanceF;
  digitalWrite(trigF, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigF, LOW);
  durationF = pulseIn (echoF, HIGH);
  distanceF = (durationF / 2) / 29;
  return distanceF;
}

//calculate the distance from the LEFT
float distancel()
{
  float durationL, distanceL;
  digitalWrite(trigL, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigL, LOW);
  durationL = pulseIn (echoL, HIGH);
  distanceL = (durationL / 2) / 29;
  return distanceL;
}

//calculate the distance from the RIGHT
float distancer()
{
  float durationR, distanceR;
  digitalWrite(trigR, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigR, LOW);
  durationR = pulseIn (echoR, HIGH);
  distanceR = (durationR / 2) / 29;
  return distanceR;
}

void setup() {
  // put your setup code here, to run once:

  R=-1;
  pinMode(motorIn1, OUTPUT);
  pinMode(motorIn11, OUTPUT);
  pinMode(motorIn2, OUTPUT);
  pinMode(motorIn22, OUTPUT);
  
  /*frequency*/Serial.begin(9600);
  pinMode (trigF, OUTPUT);
  pinMode (echoF, INPUT);
  pinMode (trigL, OUTPUT);
  pinMode (echoL, INPUT);
  pinMode (trigR, OUTPUT);
  pinMode (echoR, INPUT);

  pinMode(53,OUTPUT);
  //digitalWrite(53,HIGH);
  /*pinMode(52,OUTPUT);
  digitalWrite(52,HIGH);
  pinMode(51,OUTPUT);
  digitalWrite(51,HIGH);
  pinMode(50,OUTPUT);
  digitalWrite(50,HIGH);
  */
  pinMode(49,OUTPUT);
  digitalWrite(49,LOW);
  //左轉
  pinMode(48,OUTPUT);
  digitalWrite(48,LOW);
  //右轉
  pinMode(47,OUTPUT);
  digitalWrite(47,LOW);
  //微調
  pinMode(46,OUTPUT);
  digitalWrite(46,LOW);
  //回頭
  pinMode(45,OUTPUT);
  digitalWrite(45,LOW);
  
  pinMode(A7,INPUT);




}

void loop() 
{   
    
//monitor the data printing out
  Serial.print("Data:");
  Serial.print (time / 1000);
  Serial.print(", d = ");
  Serial.print(distancef());
  Serial.print("               ");
  Serial.print(distancel());
  Serial.print("               ");
  Serial.print(distancer());
  Serial.println(" cm");
  time = time + inter_time;
//  delay(inter_time);

  if(distancel()<6 || distancer()>13)
  {
    move_backward(150,120);
    stop();
    turn_right(80,120);
    stop();
    move_forward(500,150);
    stop();
  }


  else if(distancer()<6||(distancel()>12&&distancel()<15))
  {
    move_backward(150,120);
    stop();
    turn_left(100,120);
    stop();
    move_forward(400,150);
    stop();
  }
  
  /*else if(distancef()<4.5)
  {
    move_backward(300,150);
    stop();
  }*/
  
  else if(distancef()>7 && distancef()<1000)
  {
    move_forward(100,120);
    stop();
  }  
  
  else if(distancel()>40 && (distancef()<7 || distancef()<7))
  {
    move_backward(200,150);
    turn_left(180,120);
    stop();
    turn_left(180,120);
    stop();
  move_forward(1000,100);
    R=0;
    stop(); 
}


  else
  {
    move_forward(100,120);
    stop();
  }
  

    



    
  
  
  
  
}
