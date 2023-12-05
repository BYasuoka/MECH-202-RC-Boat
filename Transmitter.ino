//Brenton Yasuoka
//Updated December 5, 2023
// This code is a copy of the transmitter code used on an arduino that was hooked up to two joysticks, and a nRF24L01 Module 

// Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Create an RF24 object
RF24 radio(7, 8);  // CE, CSN

const int Lpot = 0;  // Define the analog pin used to connect the joystick (Left)
const int Rpot =   1;  // Define the analog pin used to connect the joystick (Right)
struct Data_Package {
  int Rval;    // variable to read the value from the analog pin (Right)
  int Lval; //Left Linear potentiometer Variable
  char f[36] = "Brenton is still figuring life out"; //Test message for RF module
};

Data_Package data; // Create an instance of the Data_Package struct

// Address through which two modules communicate.
const byte address[6] = "00001"; // COULD use an address like this...

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.setChannel(2471);  // Set channel number here, between 0 to 125
  radio.openWritingPipe(address); //code from the testing code
  radio.setPALevel(RF24_PA_HIGH); //sets the power level of transmitting/receiving
  radio.stopListening();
}

void loop() {
  // Read the value of the potentiometer
  data.Rval = analogRead(Rpot);
 
  data.Rval = map(data.Rval, 0,1023,0,50); //Map values from 0-50 to increase slosh 
  data.Rval = map(data.Rval, 0, 50, 550, 2650); //sloshed values will be mapped again to the actual tuned frequencies of the ESC

  data.Lval = analogRead(Lpot); // See comments above this is just for left joystick
   Serial.println(data.Lval);
  data.Lval = map(data.Lval, 0, 1023, 0, 50);
  data.Lval = map(data.Lval, 0, 50, 550, 2650);



  // Send the data structure to the receiver
  radio.write(&data, sizeof(Data_Package));

  Serial.println(data.f);
  Serial.println(data.Rval);
   Serial.println(data.Lval);

  delay(10);
}
