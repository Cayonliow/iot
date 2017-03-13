
//IN1,2 control left motor
const int motorIn1 = 31;
const int motorIn11 = 33;

//IN3,4 control right motor
const int motorIn2 = 30;
const int motorIn22 = 32;

const int trigF = 13;
const int echoF = 3;

const int trigL1 = 6;
const int echoL1 = 7;

const int trigR1 = 4;
const int echoR1 = 5;

const int trigL2 = 11;
const int echoL2 = 12;

const int trigR2 = 8;
const int echoR2 = 9;

char last='S';

int time = 0;const int inter_time = 100;
//make the motor stop

//turn right
void turn_right(int m,int d)
{
   analogWrite(motorIn1, 0);
   analogWrite(motorIn11, d);
   analogWrite(motorIn2, 0);
   analogWrite(motorIn22, d);
   delay(m);
}

//turn left
void turn_left(int m, int d)
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
  analogWrite(motorIn22, x);
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

  if(distanceF>=3000)
  {
    distanceF = 0;
  }
  
  return distanceF;
}

//calculate the distance from the LEFT
float distancel()
{
  float durationL1, distanceL1;
  digitalWrite(trigL1, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigL1, LOW);
  durationL1 = pulseIn (echoL1, HIGH);
  distanceL1 = (durationL1 / 2) / 29;
  
  if(distanceL1>=3000)
  {
    distanceL1 = 0;
  }
    
  return distanceL1;
}

//calculate the distance from the RIGHT
float distancer()
{
  float durationR1, distanceR1;
  digitalWrite(trigR1, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigR1, LOW);
  durationR1 = pulseIn (echoR1, HIGH);
  distanceR1 = (durationR1 / 2) / 29;

  if(distanceR1>=3000)
  {
    distanceR1 = 0;
  }

  return distanceR1;
}

//calculate the distance from the LEFT
float distancell()
{
  float durationL2, distanceL2;
  digitalWrite(trigL2, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigL2, LOW);
  durationL2 = pulseIn (echoL2, HIGH);
  distanceL2 = (durationL2 / 2) / 29;

  if(distanceL2>=3000)
  {
    distanceL2 = 0;
  }
  
  return distanceL2;
}

//calculate the distance from the RIGHT
float distancerr()
{
  float durationR2, distanceR2;
  digitalWrite(trigR2, HIGH);
  delayMicroseconds(5);
  digitalWrite(trigR2, LOW);
  durationR2 = pulseIn (echoR2, HIGH);
  distanceR2 = (durationR2 / 2) / 29;

  if(distanceR2>=3000)
  {
    distanceR2 = 0;
  }
  
  return distanceR2;
}

//左轉
void left90(){
    turn_left(350,180);
    stop();
    delay(1000);
}

//右轉
void right90(){
    turn_right(350,180);
    stop();
    delay(1000);
}

//左微調
void left30(){
    turn_left(80,250);
    stop();
    delay(100); 
}

//右微調
void right30(){
    turn_right(80,250);
    stop();
    delay(100); 
}

//直走
void straight(){
    move_forward(700,200);
    stop();
    delay(1000); 
}

//倒退
void back(){
    move_backward(700,200);
    stop();
    delay(1000); 
}

void setup() {
  // put your setup code here, to run once:

  Serial.begin(9600);
  pinMode (trigF, OUTPUT);
  pinMode (echoF, INPUT);
  pinMode (trigL1, OUTPUT);
  pinMode (echoL1, INPUT);
  pinMode (trigR1, OUTPUT);
  pinMode (echoR1, INPUT);
  pinMode (trigL2, OUTPUT);
  pinMode (echoL2, INPUT);
  pinMode (trigR2, OUTPUT);
  pinMode (echoR2, INPUT);

  ////////////////////////
  pinMode(motorIn1, OUTPUT);  
  pinMode(motorIn11, OUTPUT);
  pinMode(motorIn2, OUTPUT);
  pinMode(motorIn22, OUTPUT);

}
void loop() 
{   

//monitor the data printing out
  Serial.print("Data:");
  Serial.print (time / 1000);
  Serial.print(", d = ");
  //Serial.print(distancef());
  Serial.print("               ");
  Serial.print(distancel());
  Serial.print("               ");
  Serial.print(distancer());
  Serial.print("               ");
  Serial.print(distancell());
  Serial.print("               ");
  Serial.print(distancerr());
  Serial.println(" cm");
  time = time + inter_time;

  if(distancell()<=4.5||distancerr()<=4.5)
  {
    if(distancell()<=4.5)
      right30();

    if(distancerr()<=4.5)
      left30();
      
    while(distancell()<=3.5||distancerr()<=3.5)
    {
      if(distancell()<=3.5)
       right30();

      if(distancerr()<=3.5)
        left30();
    }

  }

  //只有左邊有路
  else if(distancel()>=20&&distancer()<=20&&distancef()<=20)
  {
    left90();
    straight();
  }
    

  //只有右邊有路
  else if(distancel()<=20&&distancer()>=20&&distancef()<=20)
  {
    right90();
    straight();
  }
    

  //左右都有路
  else if(distancel()>=20&&distancer()>=20&&distancef()<=20)
  {
    if(distancel()<distancer())
    {
      left90();
      straight();
      last='L';
    }
      
    else
    {
      right90();
      straight();
      last='R';
    }
  }

  //左前有路
  else if(distancel()>=20&&distancer()<=20&&distancef()>=20)
  {
    if(distancel()<distancef())
    {
      left90();
      straight();
      last='Q';
    }

    else
    {
      straight();
      last='P';
    }
  }

  //右前有路
  else if(distancel()<=20&&distancer()>=20&&distancef()>=20)
  {
    if(distancer()<distancef())
    {
      right90();
      straight();
      last='W';
    }

    else
    {
      straight();
      last='O';
    }
  }

  //都沒有路
  else if(distancel()<=20&&distancer()<=20&&distancef()<=20)
  {
    move_backward(200,150);
    if(last=='L')
    {
      if(distancel()>distancer())
      {
        left90();left90();
      }
        
      else
      {
        right90();right90();
      }
        
      straight();
      straight();
    }

    else if(last=='R')
    {
      if(distancel()>distancer())
      {
        left90();left90();
      }
        
      else
      {
        right90();right90();
      }
        
      straight();
      straight();
    }

    else if(last=='Q')
    {
      if(distancel()>distancer())
      {
        left90();left90();
      }
        
      else
      {
        right90();right90();
      }
        
      straight();
      left90();
      straight();
    }

     else if(last=='P')
    {
      if(distancel()>distancer())
      {
        left90();left90();
      }
        
      else
      {
        right90();right90();
      }
        
      straight();
      right90();
      straight();
    }

     else if(last=='W')
    {
      if(distancel()>distancer())
      {
        left90();left90();
      }
        
      else
      {
        right90();right90();
      }
        
      straight();
      right90();
      straight();
    }

     else if(last=='O')
    {
      if(distancel()>distancer())
      {
        left90();left90();
      }
        
      else
      {
        right90();right90();
      }
        
      straight();
      left90();
      straight();
    }

    last='S';
  }

  else
  {
    straight();
  }
}
