//IN1,2 control left motor
const int motorIn1 = 2;
const int motorIn11 = 3;

//IN3,4 control right motor
const int motorIn2 = 4;
const int motorIn22 = 5;

//front sensor
const int trigF = 6;
const int echoF = 7;

//left sensor
const int trigR = 8;
const int echoR = 9;

//right sensor
const int trigL = 10;
const int echoL = 11;

//variable used to print the data from sensor
//const int inter_time = 100;
//int time = 0;

//the smallest distance accepted between the car and the wall 
int adjD = 4;
//the normal distance between the cae and the wall
int avD = 12;

int ninety = 150;
int straight = 150;

//array used to record the previous distance detected from the 3 direction
float rf[100], rl[100], rr[100];
static int x = 1,y = 1,z = 1;

//array used to record the path of the car move
int path[100];
static int r = 1;


//make the motor stop
void stop()
{
  analogWrite(motorIn1, 0);
  analogWrite(motorIn11, 0);
  analogWrite(motorIn2, 0);
  analogWrite(motorIn22, 0);
  delay(100);
}

//turn right
void turn_right(int m, int n, int d)
{
  for (int i = 0; i < n; ++i)
  {
    analogWrite(motorIn1, 0);
    analogWrite(motorIn11, d);
    analogWrite(motorIn2, 0);
    analogWrite(motorIn22, d);
    delay(m);
  }
}

//turn left
void turn_left(int m, int n, int d)
{
  for (int i = 0; i < n; ++i)
  {
    analogWrite(motorIn1, d);
    analogWrite(motorIn11, 0);
    analogWrite(motorIn2, d);
    analogWrite(motorIn22, 0);
    delay(m);
  }
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

//intialization
void setup()
{
  pinMode(53,OUTPUT);
  digitalWrite(53,LOW);
  //setup the pins of the motors
  pinMode(motorIn1, OUTPUT);
  pinMode(motorIn11, OUTPUT);
  pinMode(motorIn2, OUTPUT);
  pinMode(motorIn22, OUTPUT);

  //setup the pins of the sensor
  /*frequency*/Serial.begin(9600);
  pinMode (trigF, OUTPUT);
  pinMode (echoF, INPUT);
  pinMode (trigL, OUTPUT);
  pinMode (echoL, INPUT);
  pinMode (trigR, OUTPUT);
  pinMode (echoR, INPUT);

  //record the distance from 3 direction
  rf[0] = 0.0;
  rl[0] = 0.0;
  rr[0] = 0.0;

  //initialize the array of recording the moves of the car
  for (int i = 0; i < 100; ++i)
  {
    path[i] = -1;
  }
}

void loop()
{
  //initial the motor to be at rest
  stop();

  int adjD = 5;
  int avD = 12;
  int ninety = 150;
  int straight = 150;
  
  //monitor the data printing out
  /*Serial.print("Data:");
  Serial.print (time / 1000);
  Serial.print(", d = ");
  Serial.print(distancef());
  Serial.print("               ");
  Serial.print(distancel());
  Serial.print("               ");
  Serial.print(distancer());
  Serial.println(" cm");
  time = time + inter_time;
  delay(inter_time);*/

  if ((int)rf[x] == (int)rf[x - 1] && (int)rl[y] == (int)rl[y - 1] && (int)rr[z] == (int)rr[z - 1])
  {
    move_backward(200, 150);

    if (path[r - 1] == 6)
    {
      turn_left(100, 1, 100);
      stop();
    }

    if (path[r - 1] == 4)
    {
      turn_right(100, 1, 100);
      stop();
    }
  }

  if (distancef() > 8)
  {
    if (distancef() > 3000)
    {
      move_backward(250, 150);
      stop();
      if (path[r - 1] == 6)
      {
        turn_right(100, 1, 80);
        stop();
      }

      else if (path[r - 1] == 4)
      {
        turn_left(100, 1, 80);
        stop();
      }
    }

    if (distancel() < adjD)
    {
      move_backward(200, 150);
      stop();
      turn_right(200, 1, 100);
      path[r++] = 6;
    }

    if (distancer() < adjD)
    {
      move_backward(200, 150);
      stop();
      turn_left(200, 1, 100);
      path[r++] = 4;
    }
    
    move_forward(1000,straight);

    rf[++x] = distancef();
    rl[++y] = distancel();
    rr[++z] = distancer();
    stop();
  }

  else
  {
    if (distancel() < avD+3 && distancer() < avD+3)
    {
      move_backward(200,150);
      turn_right(300, 1, 100);
      move_forward(200,150);
      stop();
      turn_left(500, 4, 130);
      digitalWrite(53,HIGH);
      while(1>0)
      {
         move_forward(500,130);
         //stop();
      ///////
        if(distancel()>25||distancer()>25)
        {
          digitalWrite(53,LOW);
          if(distancel()>distancer())
          {
            stop();
            turn_left(500, 1, 130);
            break;
          }
          
          if(distancer()>distancel())
          {
            stop();
            turn_right(500, 1, 130);
            break;
          }
          digitalWrite(53,HIGH);
        }

        if (distancel() < adjD)
        {
            //digitalWrite(53,LOW);
            move_backward(200, 150);
            stop();
            turn_right(200, 1, 100);
           // digitalWrite(53,HIGH);
        }
        else if (distancer() < adjD)
        {
           // digitalWrite(53,LOW);
            move_backward(200, 150);
            stop();
            turn_left(200, 1, 100);
           // digitalWrite(53,HIGH);
        }

        
      }
      digitalWrite(53,LOW);
      stop();
      
    }

    else if (distancel() > avD && distancer() < avD)
    {
      if (distancef() <= 3)
      {
        move_backward(100, 120);
        stop();
      }

      turn_left(500, 1, 150);
      path[r++] = 4;
    }

    else if (distancel() < avD && distancer() > avD)
    {
      if (distancef() <= 3)
      {
        move_backward(100, 120);
        stop();
      }
      turn_right(500, 1, 150);
      path[r++] = 6;
    }

    else if (distancel() > avD && distancer() > avD)
    {
      move_backward(250, 120);
      stop();

      if (distancel() >= distancer())
      {
        ////
        turn_left(500, 1, 180);
        path[r++] = 6;
      }

      if (distancel() < distancer())
      {
        turn_right(500, 1, 130);
        path[r++] = 4;
      }
    }

    stop();
  }
}

