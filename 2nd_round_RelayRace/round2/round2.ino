#include<BRCClient.h>

/* Just receive the message from others.
 * If it received the MSG_ROUND_END, quit the server.
 */

// #define UNO

#ifdef UNO
 #define UART_RX 3
 #define UART_TX 2
#else
 #define UART_RX 10
 #define UART_TX 2
#endif

// You have to modify the corresponding parameter
//#define AP_SSID    "Z5P"
#define AP_SSID    "programtheworld"
//#define AP_PASSWD  "0123456789"
#define AP_PASSWD  "screamlab"
//#define TCP_IP     "192.168.43.1"
#define TCP_IP     "192.168.150.11"
#define TCP_PORT   5000
#define MY_COMM_ID (char)0xDD


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
  return distanceR2;
}

BRCClient brcClient(UART_RX, UART_TX);

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

    
  pinMode(motorIn11, OUTPUT);
  pinMode(motorIn2, OUTPUT);
  pinMode(motorIn22, OUTPUT);

  brcClient.begin(9600);
  brcClient.beginBRCClient(AP_SSID, AP_PASSWD, TCP_IP, TCP_PORT);

  delay(2000);
  if (brcClient.registerID(MY_COMM_ID))
    Serial.println("ID register OK");
  else {
    Serial.println("ID register FAIL");
    brcClient.endBRCClient();
  }
/*    while (1)
      ;
  }

  */
  CommMsg msg;
  char buf[40];

  while(0){
    if(brcClient.receiveMessage(&msg)){
      sprintf(buf,"%s",msg.buffer);
      if(msg.ID==(char)0xCC){
        if(buf[0]=='D' && buf[1]=='D' && buf[2]=='_' && buf[3]=='G' && buf[4]=='O'){
          break;
        }
      }
    }
  }
}

void loop() 
{   
  //turn_right(50,150);
  //turn_left(50,150);
  //turn_left(120,180);
  //turn_right(120,180);
  //move_forward(1000,200);
  //stop();
  //delay(200);
  
  //if(1/*死路*/){
  //  CommMsg msg;
  //  msg.type = MSG_ROUND_COMPLETE;
  //  brcClient.sendMessage(&msg);
  //}
  
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
  
  if(distancer()<=5)
     {
    move_backward(50,150);
    turn_left(180,150);
    stop();
    delay(50); 
  }
  
   if(distancel()<=5)
  {
    move_backward(50,150);
    turn_right(180,150);
    stop();
    delay(50);
  }
  
  if(distancef()<=12)// need to turnright or left
  {
    if(distancel() >= 20)
    {
      turn_left(350,180);
      stop();
      delay(750);
    }
    else if(distancer() >= 20)
    {
      turn_right(350,180);
      stop();
      delay(750);
    }
    else if(distancef()<=6)
    {
      CommMsg msg;
      msg.type = MSG_ROUND_COMPLETE;
      brcClient.sendMessage(&msg);
      delay(120);
    }
     
  }
    
    else
    {
        move_forward(500,200);
    }

}
