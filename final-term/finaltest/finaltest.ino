 #include <SPI.h>
#include <RFID.h>
#include<BRCClient.h>

/* The pin mapping of SPI */
#ifdef UNO
 #define SPI_MOSI 11
 #define SPI_MISO 12
 #define SPI_SCLK 13 
 
 #define UART_RX 3
 #define UART_TX 2
 
#else
 #define SPI_MOSI 51
 #define SPI_MISO 50
 #define SPI_SCLK 52

 #define UART_RX 10
 #define UART_TX 2
#endif

// You have to modify the corresponding parameter
//#define AP_SSID    "AndroidAPF487"
//#define AP_PASSWD  "dwlw2631"
//#define TCP_IP     "192.168.43.1"

#define AP_SSID    "Z5P"
#define AP_PASSWD  "0123456789"
#define TCP_IP     "192.168.43.1"

//#define AP_SSID    "programtheworld"
//#define AP_PASSWD  "screamlab"
//#define TCP_IP     "192.168.150.11"
#define TCP_PORT   5000
#define MY_COMM_ID (char)0xDD

// SPI_SS pin can be chosen by yourself
// becasue we use SPI in master mode.
#define SPI_SS   53
#define MFRC522_RSTPD 9

RFID rfid(SPI_SS, MFRC522_RSTPD);

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
const int echoR2 = 48;

char last='S';

boolean y=true;

static uint8_t status;
static uint16_t card_type;
static uint8_t sn[MAXRLEN], snBytes;


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

//左轉
void left90(){
    turn_left(325,180);
    stop();
    delay(1000);
    
}

//右轉
void right90(){
    turn_right(335,180);
    stop();
    delay(1000);
}

//左微調
void left30(){
    turn_left(120,150);
    stop();
    delay(1000); 
}

//右微調
void right30(){
    turn_right(120,150);
    stop();
    delay(1000); 
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

boolean check(float m,float n)
{
  if(abs(m-n)<0.8)
    return true;

  return false;
}

void adj()
{
  while((check(distancell(),distancel())==false||check(distancerr(),distancer())==false)&&(distancell()<=6||distancerr()<=6)&&y==true)
  {
    if(distancell()>distancerr())
      left30();

    else
      right30();

    y=false;
  }
  stop();
}

void adjB()
{
  while((distancel()<=6&&distancell()>=20)||(distancer()<=6&&distancerr()>=20)||(distancell()<=6&&distancel()>=20)||(distancerr()<=6&&distancer()>=20))
  //while((distancel()<=6||distancer()<=6)&&(distancell()>=20||distancerr()>=20))
  {
    if(distancel()>distancer())
    {
      move_backward(150,200);
      left30();
    }
      

    else
    {
      move_backward(150,200);
      right30();

    }
  }
  stop();
}

void Wtoturn()
{
  if(distancel()>distancer())
      {
        left90();
        left90();
      }
        
      else
      {
        right90();
        right90();
      }
      
  straight();
}


BRCClient brcClient(UART_RX, UART_TX);

void sense()
{
  if ((status = rfid.findTag(&card_type)) == STATUS_OK &&
      card_type == 1024) {
    Serial.print("Tag SN: ");
    if ((status = rfid.readTagSN(sn, &snBytes)) == STATUS_OK) {
      CommMsg msg;
      msg.type = 0x10;
      for (int i = 0; i < snBytes; ++i){
        msg.buffer[i]=sn[i];
        Serial.print(sn[i], HEX);
      }
      brcClient.sendMessage(&msg);
      Serial.println();
      rfid.piccHalt();
      brcClient.receiveMessage(&msg);
      int x=msg.buffer[4];
      int y=msg.buffer[5];
      int DD = msg.buffer[6];
      if( DD == 36 ){
        brcClient.broadcast("DD_PARK");
        CommMsg msg;
        msg.type = MSG_ROUND_COMPLETE;
        brcClient.sendMessage(&msg);
        
        stop();
        while(1);
      }
      Serial.println("Recv:");
      Serial.print(x);
      Serial.print(",");
      Serial.print(y);
      Serial.println("Recv_END");
    }
  }
  else
    Serial.println("No tag.");
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

  brcClient.begin(9600);
  brcClient.beginBRCClient(AP_SSID, AP_PASSWD, TCP_IP, TCP_PORT);

  delay(2000);
  if (brcClient.registerID(MY_COMM_ID))
    Serial.println("ID register OK");
  else {
    Serial.println("ID register FAIL");
    brcClient.endBRCClient();
  }

  SPI.begin();
  SPI.beginTransaction(SPISettings(10000000L, MSBFIRST, SPI_MODE3));
  rfid.begin();

  Serial.begin(9600);
  while (!Serial)
    ;

  CommMsg msg;
  char buf[40];

  //
//  while(!brcClient.receiveMessage(&msg) || msg.type != (char)0x20);
  while(1){
    if(brcClient.receiveMessage(&msg)){
      sprintf(buf,"%s",msg.buffer);
      if( (buf[0]=='D' && buf[1]=='D' && buf[2]=='_' && buf[3]=='G' && buf[4]=='O') || (buf[0]=='A' && buf[1]=='L' && buf[2]=='L' && buf[3]=='_' && buf[4]=='G' && buf[5]=='O') )
        break;
    }
  }
/*  while(1){
    if(brcClient.receiveMessage(&msg)){
      sprintf(buf,"%s",msg.buffer);
      if(msg.ID==(char)0xCC || msg.ID==(char)0xFF){
        if(buf[0]=='D' && buf[1]=='D' && buf[2]=='_' && buf[3]=='G' && buf[4]=='O'){
          break;
        }
      }
    }
  }*/

  

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
  stop();

  //sense(); 
  adj();
  adjB();

  //只有左邊有路
  if(distancel()>=20&&distancer()<=20&&distancef()<=20)
  {
    left90();
    straight();
    y=true;
  }
    

  //只有右邊有路
  else if(distancel()<=20&&distancer()>=20&&distancef()<=20)
  {
    right90();
    straight();
    y=true;
  }
    

  //左右都有路
  else if(distancel()>=20&&distancer()>=20&&distancef()<=20)
  {
    if(distancel()<distancer())
    {
      left90();
      straight();
      last='L';
      y=true;
    }
      
    else
    {
      right90();
      straight();
      last='R';
      y=true;
    }
  }

  //左前有路
  else if(distancel()>=20&&distancer()<=20&&distancef()>=20)
  {
    /*if(distancel()<distancef())
    {
      left90();
      straight();
      last='Q';
      y=true;
    }

    else
    {
      straight();
      last='P';
      y=true;
    }*/

      left90();
      straight();
      last='Q';
      y=true;
  }

  //右前有路
  else if(distancel()<=20&&distancer()>=20&&distancef()>=20)
  {
    if(distancer()<distancef())
    {
      right90();
      straight();
      last='W';
      y=true;
    }

    else
    {
      straight();
      last='O';
      y=true;
    }
  }

  //都沒有路
  else if(distancel()<=20&&distancer()<=20&&distancef()<=20)
  {
    stop();
    delay(100);
        char buf[20];
        CommMsg msg;
        if(brcClient.receiveMessage(&msg)){
          sprintf(buf,"%s",msg.buffer);
          Serial.println(buf);
          delay(100);
          if(buf[0]=='D' && buf[1]=='D' && buf[2]=='_' && buf[3]=='D' && buf[4]=='I' && buf[5]=='E'){
            brcClient.broadcast("DD_IS_HERE");
            CommMsg msg;
            msg.type = MSG_ROUND_COMPLETE;
            brcClient.sendMessage(&msg);
            stop();
            while(1);
          }
        }
        
    sense();
    move_backward(200,150);
    sense();
    if(last=='L')
    {
      sense();
      Wtoturn();
      sense();
        
      while(distancer()<20)
      {
        adj();
        sense();
        straight();
      }
       
      straight();
      y=true;
    }

    else if(last=='R')
    {
      sense();
      Wtoturn();
      sense();
        
      while(distancel()<20)
      {
        adj();
        sense();
        straight();
      }
       
      straight();
      y=true;
    }

    else if(last=='Q')
    {
      sense();
      Wtoturn();
      sense();
        
      while(distancel()<20)
      {
        adj();
        sense();
        straight();
      }
        
      left90();
      straight();
      y=true;
    }

     else if(last=='P')
    {
      sense();
      Wtoturn();
      sense();
        
      while(distancer()<20)
      {
        adj();
        sense();
        straight();
      }
        
      right90();
      straight();
      y=true;
    }

     else if(last=='W')
    {
      sense();
      Wtoturn();
      sense();
      
      while(distancer()<20)
      {
        adj();
        sense();
        straight();
      }
      
      right90();
      straight();
      y=true;
    }

     else if(last=='O')
    {
      sense();
      Wtoturn();
      sense();
      
      while(distancel()<20)
      {
        adj();
        sense();
        straight();
      }
       
      left90();
      straight();
      y=true;
    }

    last='S';
  }

  else
  {
    straight();
    y=true;
  }
    
    
}
