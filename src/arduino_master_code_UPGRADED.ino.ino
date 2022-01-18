//Arduino Master Code
#include <DHT.h>
#include <SPI.h> //Call SPI library so you can communicate with the nRF24L01+
#include <RF24.h> //nRF2401 libarary
#include <LiquidCrystal.h> //LCD Library
#define dht_apin A1
#define wateringTime 10000 //20 seconds
#define RELAY_ON LOW
#define RELAY_OFF HIGH
#define afterWateringWait 5000;
#define offDelayTime 2000;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int pinCE = 7; //This pin is used to set the nRF24 to standby (0) or active mode (1)
const int pinCSN = 8; //This pin is used to tell the nRF24 whether the SPI communication is a //command or message to send out
RF24 radio(pinCE, pinCSN); // Declare object from nRF24 library (Create your wireless SPI)
//Create up to 3 pipe addresses P0 - P5;  the "LL" is for LongLong type
const uint64_t rAddress[3] = {0x7878787878LL, 0xB3B4B5B6F1LL, 0xB3B4B5B6CDLL}; 
int check; //identity
int receiveDaverage;  //variable to hold payload from transmitter
void CornTest();
void TomatoTest();
void CowpeaTest();
dht DHT;
void setup() 
{
  //  put your setup code here, to run once:
Serial.begin(9600); // Initialize serial monitor
lcd.begin (16, 2);   //set the LCD's number of columns and rows
    pinMode (28,OUTPUT);//set pin 28 as output
    pinMode (30,OUTPUT);
    pinMode (32,OUTPUT);
      lcd.setCursor (0,0);
    lcd.print("MECHATRONICS IN"); //print on LCD display
    lcd.setCursor(0,1);
    lcd.print("AGRICULTURE");
    delay(3000);
    lcd.clear(); 
    lcd.setCursor(0,0);
    lcd.print("A FOCUS ON");
    lcd.setCursor(0,1);
    lcd.print("IRRIGATION");
    delay(offDelayTime);
    lcd.clear(); 
    lcd.print("Project by:");
    lcd.setCursor(0,1);
    lcd.print("Presh,Daran,Fame");
    delay(3000);
    lcd.clear(); 
    lcd.print("Supervisor");
    lcd.setCursor(0,1);
    lcd.print("Prof. Bolu");
    delay(3000);
    lcd.clear();
radio.begin();  //Start the nRF24 module
digitalWrite(28, RELAY_OFF); // Set pin 28 as high initially
digitalWrite(30, RELAY_OFF);
digitalWrite(32, RELAY_OFF);
// Open up to three pipes for PRX to receive data
  radio.openReadingPipe(0,rAddress[0]);
  radio.openReadingPipe(1,rAddress[1]);
  radio.openReadingPipe(2,rAddress[2]);
  
  radio.startListening();  // Start listening for messages
}
void loop()
{ 
   // put your main code here, to run repeatedly:
  lcd.setCursor(0,0);
  lcd.print("receiving");
  lcd.setCursor(0,1);
  lcd.print("data...");
  delay(offDelayTime);
 

  DHT.read11(dht_apin);
  lcd.setCursor(0,0);
  lcd.print("H = ");
  lcd.print(DHT.humidity);
  lcd.print("% ");
  lcd.print("T = ");
  lcd.print(DHT.temperature); 
  lcd.print("C ");
  delay(offDelayTime);
  lcd.clear();
while (radio.available()) //Check if received data
   {
    radio.read(&receiveDaverage, 1);//reads the received average from the transmitting micro controller into the 'receiveDaverage' variable.
       radio.read(&check, 1);//reads the received identity value from the transmitting micro controller into the 'check' variable.
  if (check == 2)//arduino Identity conditional check
    {
 TomatoTest();
    }    

  if (check == 1)
   { 
    CornTest();
   }
  if (check ==3)
   { 
      CowpeaTest()
   } 
}

//Function definitions
void tomatoTest()
{ 

  { 
    Serial.print("Dry soil, Valve 2: Open");
        lcd.setCursor(0,0);//sets cursor position
        lcd.print("DRY SOIL: ");//prints
        lcd.print(receiveDaverage);
         lcd.print("%");
             lcd.setCursor(0,1);
           lcd.print("VALVE 2: OPEN");
       digitalWrite(28, RELAY_ON);//sets pin at 28 high to open valve
        delay (wateringTime);//waters for 'wateringTime value'
           digitalWrite(28, RELAY_OFF);
           delay(afterWateringWait);
            lcd.clear();
  }
     else  if (receiveDaverage>= 20 && receiveDaverage<=50) //for Moist Soil
      {
        Serial.print("Moist soil, Valve 2: Closed");
     lcd.setCursor(0,0);//sets cursor position
     lcd.print("MOIST SOIL: ");//prints
      lcd.print(receiveDaverage);
       lcd.print("%");
     lcd.setCursor(0,1);
     digitalWrite(28,RELAY_OFF);//sets pin at 28 to low, keeping valve closed
     delay(offDelayTime);//delay for 2 seconds
     lcd.clear();
     }
else if (receiveDaverage > 50)  // For Soggy soil
  { 
    Serial.print("soggy soil, Valve 2: Closed");
  }
    lcd.setCursor(0,0);
     lcd.print("SOGGY SOIL: ");
      lcd.print(receiveDaverage);
       lcd.print("%");
     lcd.setCursor(0,1);
       lcd.print("VALVE 2: CLOSED");
     digitalWrite(28,RELAY_OFF);
     delay(offDelayTime);
     lcd.clear(); 
  } 
  }
  
  void CornTest()
  {
    if (receiveDaverage<40)   // for dry soil
  { 
    Serial.print("DRY Soil Valve 1 Open");
        lcd.setCursor(0,0);
        lcd.print("DRY SOIL: ");
         lcd.print(receiveDaverage);
          lcd.print("%");
         lcd.setCursor(0,1);
            lcd.print("VALVE 1: OPEN");
       digitalWrite(30, RELAY_ON);
       delay (wateringTime);
           digitalWrite(30, RELAY_OFF);
         delay(afterWateringWait);
  }
else if (receiveDaverage>= 40 && receiveDaverage<=70) //for Moist Soil
    { 
      Serial.print("Moist soil, Valve 1: Closed");
      lcd.setCursor(0,0);
     lcd.print("MOIST SOIL: ");
      lcd.print(receiveDaverage);
       lcd.print("%");
     lcd.setCursor(0,1);
    lcd.print("VALVE 1: CLOSED");
        digitalWrite(30, RELAY_OFF);
        delay(offDelayTime);
     lcd.clear();
    }
 else if (receiveDaverage > 70)  // For Soggy soil
  { Serial.print("soggy soil, Valve 1: Closed");
    lcd.setCursor(0,0);
     lcd.print("SOGGY SOIL: ");
      lcd.print(receiveDaverage);
       lcd.print("%");
     lcd.setCursor(0,1);
     digitalWrite(30,RELAY_OFF);
     lcd.print("VALVE 1: CLOSED");
     lcd.setCursor(0,1);
      delay(offDelayTime);
     lcd.clear();
  }
  }

 void cowpeaTest()
  {
    if (receiveDaverage< 60)
  { 
    Serial.print("DRY Soil Valve 3: Open");
        lcd.setCursor(0,0);
        lcd.print("DRY SOIL: ");
         lcd.print(receiveDaverage);
          lcd.print("%");
        lcd.setCursor(0,1);
         lcd.print("VALVE 3: OPEN");
        digitalWrite(32, RELAY_ON);
         delay (wateringTime);
           digitalWrite(32, RELAY_OFF);
         delay(afterWateringWait);
  }
else if (receiveDaverage>= 60 && receiveDaverage<=80) //for Moist Soil
    { Serial.print("Moist soil, Valve 3: Closed");
      lcd.setCursor(0,0);
     lcd.print("MOIST SOIL: ");
      lcd.print(receiveDaverage);
       lcd.print("%");
     lcd.setCursor(0,1);
     lcd.print("VALVE 3: CLOSED");
     digitalWrite(32,RELAY_OFF);
     lcd.setCursor(0,1);
     delay(offDelayTime);
     lcd.clear();
    }
else if (receiveDaverage > 80)  // For Soggy soil
  { 
    Serial.print("soggy soil, Valve 3: Closed");
    lcd.setCursor(0,0);
     lcd.print("SOGGY SOIL: ");
      lcd.print(receiveDaverage);
       lcd.print("%8");
     lcd.setCursor(0,1);
         lcd.print("VALVE 3: CLOSED");
     digitalWrite(32, RELAY_OFF);
         lcd.setCursor(0,1);
         delay(offDelayTime);
     lcd.clear();
  } 
   }
