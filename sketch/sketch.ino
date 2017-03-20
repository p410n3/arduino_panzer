#include <SoftwareSerial.h>
#include <MotorContrl.h>



/*
    Steuerung :
 1 -> Rückwärts
 2 -> Vorwärts
 3 -> Rechts
 4 -> Links
 0 -> STOP
 8 -> Schneller
 9 -> Langsamer 
 
 wenn aus folgender Perspektive betrachtet : Von Hinten (ArduinoBoard) nach Vorne (Batt.Pack)
 MOTOR_A => Rechte Seite
 MOTOR_B => Linke Seite
 */

SoftwareSerial BT(10,11); //[BT-Adapter TX 10, RX 11] 11

int aPhase[2] = {
  2, 3};    //[MotorA]Pin APHASE des Motortreibers an Digital Pin 2,3 vom Arduino
int aEnable = 6;   //[MotorA]Pin AENABLE des Motortreibers an Digital Pin 6 vom Arduino

int bPhase[2] = {
  4, 5}; //[MotorB]Pin BPHASE des Motortreibers an Digital Pin 4,5 vom Arduino
int bEnable = 7; //[MotorB]Pin BENABLE des Motortreibers an Digital Pin 7 vom Arduino

int bt_input;

int lastPhaseA = 0;
int lastPhaseB = 0;

int lastSpeedA = 0;
int lastSpeedB = 0;

boolean steht = true;

Motor MOTOR_A(aEnable, aPhase[0], aPhase[1]);
Motor MOTOR_B(bEnable, bPhase[0], bPhase[1]);

//function
long microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  //Totally not stolen code
  return microseconds / 29 / 2;
}

//Geradeaus
int trigger1=29;
int echo1=28;
long dauer1=0;
long entfernung1=0;

//Links
int trigger2=30;
int echo2=31;
long dauer2=0;
long entfernung2=0;

//Rechts
int trigger3=8;
int echo3=9;
long dauer3=0;
long entfernung3=0;

//Motor MOTOR_A(aEnable, aPhase);
//Motor MOTOR_B(bEnable, bPhase);

void setup() 
{ 
  //Bluetooth initialisieren
  BT.begin(9600);
  BT.println("Bluetooth initialized");
  
  
  
  //Ultraschall sensor
  Serial.begin (9600);
  pinMode(trigger1, OUTPUT);
  pinMode(echo1, INPUT);

  Serial.begin (9600);
  pinMode(trigger2, OUTPUT);
  pinMode(echo2, INPUT);

  Serial.begin (9600);
  pinMode(trigger3, OUTPUT);
  pinMode(echo3, INPUT);
}

void loop() 
{
  if(BT.available())
  {
    bt_input = BT.read();
    BT.println("Bluetooth Rückmeldung : Hat folgenden Wert empfangen: ");
    BT.println(bt_input);

    //------------------------- Set Speed --------------------------
    /* Currently not working
     
     if(bt_input == '8')
     {
     MOTOR_B.Set_Speed(MOTOR_A.Give_Speed());
     if((MOTOR_A.Give_Speed()) <= 219 )
     {
     MOTOR_A.Set_Speed((MOTOR_A.Give_Speed() + 35));
     MOTOR_B.Set_Speed((MOTOR_B.Give_Speed() + 35));
     BT.print("Speed +35   ==>   ");BT.println(MOTOR_A.Give_Speed());
     lastSpeedA = MOTOR_A.Give_Speed();
     lastSpeedB = MOTOR_B.Give_Speed();
     }
     else
     {
     MOTOR_A.Set_Speed(254);
     MOTOR_B.Set_Speed(254);
     BT.print("Speed MAX   ==>   ");BT.println(MOTOR_A.Give_Speed());
     lastSpeedA = MOTOR_A.Give_Speed();
     lastSpeedB = MOTOR_B.Give_Speed();
     }
     }
     else
     {
     if(bt_input == '9')
     {
     MOTOR_B.Set_Speed(MOTOR_A.Give_Speed());
     if( (MOTOR_A.Give_Speed()) >= 55)
     {
     MOTOR_A.Set_Speed((MOTOR_A.Give_Speed() - 35));
     MOTOR_B.Set_Speed((MOTOR_B.Give_Speed() - 35));
     BT.print("Speed -35   ==>   ");BT.println(MOTOR_A.Give_Speed());
     lastSpeedA = MOTOR_A.Give_Speed();
     lastSpeedB = MOTOR_B.Give_Speed();
     }
     else
     {
     MOTOR_A.Set_Speed(25);
     MOTOR_B.Set_Speed(25);
     BT.print("Speed MIN   ==>   ");BT.println(MOTOR_A.Give_Speed());
     lastSpeedA = MOTOR_A.Give_Speed();
     lastSpeedB = MOTOR_B.Give_Speed();
     }
     }
     }
     //--------------------------------------------------------------
     */
    if(bt_input != '3' && bt_input != '4')
    {
      MOTOR_B.Set_Speed(MOTOR_A.Give_Speed());
    }

    if(bt_input == '1')
    {
      MOTOR_A.Fahren(1);
      MOTOR_B.Fahren(1);
      lastPhaseA = 1;
      lastPhaseB = 1;
      lastSpeedA = MOTOR_A.Give_Speed();
      lastSpeedB = MOTOR_B.Give_Speed();
      BT.println("Bluetooth Rückmeldung : Vorwärts");
      steht = false;
    }
    else
    {
      if(bt_input == '2')
      {
        MOTOR_A.Fahren(2);
        MOTOR_B.Fahren(2);
        lastPhaseA=2;
        lastPhaseB=2;
        lastSpeedA = MOTOR_A.Give_Speed();
        lastSpeedB = MOTOR_B.Give_Speed();
        BT.println("Bluetooth Rückmeldung : Rückwärts");
        steht = false;
      }
      else
      {
        if(bt_input == '3')
        {
          if(steht == true)
          {
            MOTOR_A.Fahren(1,127);
            MOTOR_B.Fahren(2,127);
            lastPhaseA=1;
            lastPhaseB=2;
            lastSpeedA = 127;
            lastSpeedB = 127;
            BT.println("Bluetooth Rückmeldung : Rechts");
          }
          else
          {
            MOTOR_A.Fahren(lastPhaseA, lastSpeedA/3);
            MOTOR_B.Fahren(lastPhaseB);
            BT.println("Bluetooth Rückmeldung : Rechts & vorwaerts");
          }
        }
        else
        {
          if(bt_input == '4')
          {
            if(steht==true)
            {
              MOTOR_A.Fahren(2,127);
              MOTOR_B.Fahren(1,127);
              lastPhaseA=2;
              lastPhaseB=1;
              lastSpeedA = 127;
              lastSpeedB = 127;
              BT.println("Bluetooth Rückmeldung : Links");
            }
            else
            {
              MOTOR_A.Fahren(lastPhaseA);
              MOTOR_B.Fahren(lastPhaseB, lastSpeedB/3);
              BT.println("Bluetooth Rückmeldung : Links & vorwaerts");
            }
          }
          else
          {
            if(bt_input == '0')
            {
              MOTOR_A.Fahren(0);
              MOTOR_B.Fahren(0);
              lastPhaseA = 1;
              lastPhaseB = 1;
              lastSpeedA = 0;
              lastSpeedB = 0;
              steht = true;
              BT.println("Bluetooth Rückmeldung : HALT STOP !");
            }
            else
            {
              //Ab hier ist Ultraschallsensor
              if (bt_input == '7')
              {
                do
                {             
                  //INIT US-Sensor
                  digitalWrite(trigger1, LOW);
                  //delayMicroseconds(2);
                  digitalWrite(trigger1, HIGH);
                  //delayMicroseconds(10);
                  //dauer1 = pulseIn(echo1, HIGH);
                  digitalWrite(trigger1, LOW);

                  dauer1 = pulseIn(echo1, HIGH);
                  entfernung1 = microsecondsToCentimeters(dauer1);

                  digitalWrite(trigger2, LOW);
                  //delayMicroseconds(2);
                  digitalWrite(trigger2, HIGH);
                  //delayMicroseconds(10);
                  //dauer2 = pulseIn(echo2, HIGH);
                  digitalWrite(trigger2, LOW);

                  dauer2 = pulseIn(echo2, HIGH);
                  entfernung2 = microsecondsToCentimeters(dauer2);

                  digitalWrite(trigger3, LOW);
                  //delayMicroseconds(2);
                  digitalWrite(trigger3, HIGH);
                  //delayMicroseconds(10);
                  //dauer3 = pulseIn(echo3, HIGH);
                  digitalWrite(trigger3, LOW);
                  
                  dauer3 = pulseIn(echo3, HIGH);
                  entfernung3 = microsecondsToCentimeters(dauer3);

                  //DBG
                  /*
                   BT.println("ENTFERNUNGEN: ");
                   
                   BT.println("MITTE: ");
                   //mitte
                   BT.println(entfernung1);
                   
                   BT.println("LINKS: ");
                   //links
                   BT.println(entfernung2);
                   
                   BT.println("RECHTS: ");
                   //rechts                  
                   BT.println(entfernung3);
                   */

                  //links < 10
                  if(entfernung2 < 10)
                  {
                    MOTOR_A.Fahren(2,150);
                    MOTOR_B.Fahren(1,150);
                    lastPhaseA=2;
                    lastPhaseB=1;
                    lastSpeedA = 150;
                    lastSpeedB = 150;
                  }
                  //rechts < 10
                  else if(entfernung3 < 10)
                  {
                    MOTOR_A.Fahren(1,150);
                    MOTOR_B.Fahren(2,150);
                    lastPhaseA=1;
                    lastPhaseB=2;
                    lastSpeedA = 150;
                    lastSpeedB = 150;
                  }
                  //mitte OK
                  else if (entfernung1 > 10)
                  { 
                    MOTOR_A.Fahren(2, 150);
                    MOTOR_B.Fahren(2, 150);
                    lastPhaseA=2;
                    lastPhaseB=2;
                    lastSpeedA = MOTOR_A.Give_Speed();
                    lastSpeedB = MOTOR_B.Give_Speed();
                    lastSpeedA = 150;
                    lastSpeedB = 150;
                  }
                  //rückwärts + drehen wenn mitte < 10 
                  else if(entfernung1 < 10)
                  {
                    MOTOR_A.Fahren(1, 50);
                    MOTOR_B.Fahren(1, 150);
                    lastPhaseA=1;
                    lastPhaseB=1;
                    lastSpeedA = MOTOR_A.Give_Speed();
                    lastSpeedB = MOTOR_B.Give_Speed();
                    lastSpeedA = 50;
                    lastSpeedB = 150;
                  }     
                  //Only reads bt inout when soemthing is send.
                  if(BT.available() > 0)
                  {
                    bt_input = BT.read();
                  } 
                }
                while(bt_input == '7');
              } 
            }
          }
        }
      }
    }
  }
} 
