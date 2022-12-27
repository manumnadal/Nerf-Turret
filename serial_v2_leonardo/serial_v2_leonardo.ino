///////*** LIBRERIAS ***\\\\\\\

#include <Servo.h>
#include <string.h>
#include <stdlib.h>

///////*** SERVOS ***\\\\\\\

Servo myservoX;
Servo myservoY;
Servo myservoShot;

const int servoXPin = 3;
const int servoYPin = 5;
const int servoShotPin = 6;

int servo_valueX = 90;
int servo_valueY = 90;
int servo_valueShot = 100;

int balas = 5;

#define SHOT 45
#define RELOAD 100

///////*** COMUNICACION SERIE ***\\\\\\\

const byte numChars = 32;
char receivedCharsX[numChars];   // an array to store the received data
char receivedCharsY[numChars];   // an array to store the received data
boolean newData = false;


void setup()  
{ 
  myservoX.attach(servoXPin);
  myservoY.attach(servoYPin);
  myservoShot.attach(servoShotPin);
  
  myservoX.write(servo_valueX);
  myservoY.write(servo_valueY);
  myservoShot.write(servo_valueShot);
  
  Serial.begin(115200); 
}

void loop()  
{ 
  recvWithEndMarker();
}

///////*** FUNCIONES AUXILIARES ***\\\\\\\

void recvWithEndMarker() //CONTROL DE LECTURA
{
    static byte ndx = 0;
    char endMarker = '*';
    char code;
    char rc;
    
    if(Serial.available() > 0 && newData == false) 
    {
        rc = Serial.read();
        if (rc != endMarker) 
        {
          switch(rc)
          {
            case 'x':
              read_serial_X();
            break;
            case 'y':
              read_serial_Y();
            break;
            case 'd':
              Fire();
            break;
            case 'a':
              Fire_All();
            break;
            case 'r':
              Reload();
            break;
          }       
        }
        else 
        {
            ndx = 0;
            newData = true;
        }
    }
    rc = 'a';
    Serial.flush();
}

void read_serial_X() //LECTURA EJE X
{
    static byte i = 0;
    char endMarker = '*';
    char rc;
    while (Serial.available() > 0 && newData == false) 
    {
        rc = Serial.read();
        if (rc != endMarker) 
        {

              receivedCharsX[i] = rc;
              i++;
              if (i >= numChars) //Por si me paso leyendo, sobreescribo
              {
                  i = numChars - 1;
              }
            
        }
        else 
        {
            i = 0;
            newData = true;
        }
    }
    MoveServoX(); // muevo el servo del eje X
}

void read_serial_Y() //LECTURA EJE Y
{
    static byte i = 0;
    char endMarker = '*';
    char rc;
    while (Serial.available() > 0 && newData == false) 
    {
        rc = Serial.read();
        if (rc != endMarker) 
        {
            receivedCharsY[i] = rc;
            i++;
            if (i >= numChars) //Por si me paso leyendo, sobreescribo
            {
                i = numChars - 1;
            }
        }
        else 
        {
            i = 0;
            newData = true;
        }
    }
    MoveServoY(); // muevo el servo del eje Y
}
void MoveServoX() //MOVER EJE X
{
    if (newData == true) 
    {
        servo_valueX = atoi(receivedCharsX); // paso a entero
        servo_valueX = map(servo_valueX, 300, 1535, 0, 180);
        myservoX.write(servo_valueX); //escribo valor en el servo
        for(int i= 0; i < sizeof(receivedCharsX); i++) //limpio la cadena para eliminar acumulo basura
        {
          receivedCharsX[i] = "";
        }
        newData = false;
    }
}
void MoveServoY() //MOVER EJE Y
{
    if (newData == true) 
    {
        servo_valueY = atoi(receivedCharsY); // paso a entero
        servo_valueY = map(servo_valueY, 0, 863, 180, 0);
        myservoY.write(servo_valueY); //escribo valor en el servo
        for(int i= 0; i < sizeof(receivedCharsY); i++) //limpio la cadena para eliminar acumulo basura
        {
          receivedCharsY[i] = "";
        }
        newData = false;
    }
}
void Fire()
{
    static byte i = 0;
    char endMarker = '*';
    char rc;
    rc = Serial.read();
    if (rc == endMarker) 
    {
        newData = false;
        //ejecuto el disparo
        if(balas > 0)
        {
            myservoShot.write(SHOT);
            delay(100);
            myservoShot.write(RELOAD);
            balas--;
        }       
    }
}
void Fire_All()
{
    static byte i = 0;
    char endMarker = '*';
    char rc;
    rc = Serial.read();
    if (rc == endMarker) 
    {
        newData = false;
        //ejecuto el disparo
        while(balas > 0)
        {        
            myservoShot.write(SHOT);
            delay(300);
            myservoShot.write(RELOAD);
            delay(300);
            
            balas--;
        }
    }
}
void Reload()
{
    static byte i = 0;
    char endMarker = '*';
    char rc;
    rc = Serial.read();
    if (rc == endMarker) 
    {
        newData = false;
        balas = 5;
    }
}
