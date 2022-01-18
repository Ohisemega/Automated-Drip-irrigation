//Arduino Slave Code (Tomatoe Sample)
#include <SPI.h> //Call SPI library so you can communicate with the nRF24L01+
#include <nRF24L01.h> //nRF2401 libarary found at https://github.com/tmrh20/RF24/
#include <RF24.h> //nRF2401 libarary found at https://github.com/tmrh20/RF24/
#define potPin  A0
#define potPin1  A1
#define potPin2  A2
#define potPin3  A3
#define potPin4  A4
const int numReadings = 5;   // how many readings to average
const int check = 2;// use this for transmitting a special Identification character
int sensorValue = 0;// value read from the Soil Moisture 
int sensorValue1 = 0;// value read from the Soil Moisture 
int sensorValue2 = 0;// value read from the Soil Moisture 
int sensorValue3 = 0;// value read from the Soil Moisture 
int sensorValue4 = 0;// value read from the Soil Moisture 

RF24 radio(7, 8); // CE, CSN
#define WHICH_NODE 2     // must be a number from 1 - 6 identifying the PTX node
const uint64_t wAddress[] = {0x7878787878LL, 0xB3B4B5B6F1LL, 0xB3B4B5B6CDLL};
const uint64_t PTXpipe = wAddress[ WHICH_NODE - 0];   // Pulls the address from the above array for this node's pipe
                  byte counter = 1; //used to count the packets sent
float averageT;
    float average (int sensorValue, int sensorValue1 ,int sensorValue2 ,int sensorValue3, int sensorValue4)
                   {
                     float value = (sensorValue + sensorValue1 + sensorValue2 + sensorValue3 + sensorValue4)/5;
                                return(value);
                    }


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode( potPin, INPUT);
  pinMode( potPin1, INPUT);
  pinMode( potPin2, INPUT);
  pinMode( potPin3, INPUT);
  pinMode( potPin4, INPUT); 
radio.begin();            //Start the nRF24 module
 radio.setPALevel(RF24_PA_LOW);  // "short range setting" - increase if you want more range AND have a good power supply
  radio.setChannel(108);          // the higher channels tend to be more "open"

    radio.openReadingPipe(0, PTXpipe);  //open data transfer pipe
    radio.stopListening();//go into transmit mode
}
void loop() 
{ // put your main code here, to run repeatedly:
sensorValue = analogRead(potPin);// read the analog in values
sensorValue = map(sensorValue, 1023, 416, 0, 100);//map or caliberate the values to a percentage form between 0 and 100
sensorValue = constrain(sensorValue, 0, 100);

sensorValue1 = analogRead(potPin1);
sensorValue1 = map(sensorValue1, 1023, 416, 0, 100);
sensorValue1 = constrain(sensorValue1, 0, 100);// in case the sensor value is outside the range seen during calibration

sensorValue2 = analogRead(potPin2);
sensorValue2 = map(sensorValue2, 1023, 416, 0, 100);
sensorValue2 = constrain(sensorValue2, 0, 100);

sensorValue3 = analogRead(potPin3);
sensorValue3 = map(sensorValue3, 1023, 416, 0, 100);
sensorValue3 = constrain(sensorValue3, 0, 100);

sensorValue4 = analogRead(potPin4);
sensorValue4 = map(sensorValue4, 1023, 416, 0, 100);
sensorValue4 = constrain(sensorValue4, 0, 100);

//calculate the average of the moisture values.
averageT = (sensorValue + sensorValue1 + sensorValue2 + sensorValue3 + sensorValue4)/numReadings;

 radio.write(&averageT, sizeof(averageT));
 radio.write(&check, sizeof(check));
 Serial.println(averageT);
  Serial.println(check);
delay(63); // delay for 63 milli seconds.
}
