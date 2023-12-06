//Brenton Yasuoka
//Updated December 5, 2023
// This code is a copy of the receiver code used on an arduino that was hooked up to two Electronic Speed Controllers, and a nRF24L01 Module 
// an Electronic Speed Controller is more very easily used when called a Servo object. 

// Include Libraries
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

// Create an RF24 object
RF24 radio(7, 8);  // CE, CSN

Servo Rservo;  // Create servo object to control a servo

Servo Lservo;

//const int motorPin = 5;  // Define the PWM pin connected to the motor

struct Data_Package {
  int Rval=0;    // Variable to read the value from the analog pin
  int Lval = 0; //variable to read the value from the linear potentiometer pin
  char f[36]="";
};

Data_Package data; // Declare an instance of the Data_Package struct

// Address through which two modules communicate.
const byte address[6] = "00001"; // COULD use an address like this...

void setup() {
  Serial.begin(9600);
  
  radio.begin();
  radio.setChannel(2471);  // Set channel number here, between 0 to 125
  radio.openReadingPipe(0,address);// this is from the testing code uncomment below code once you delete this
  //radio.openWritingPipe(0xD0A7A7A7A7LL);
  radio.setPALevel(RF24_PA_HIGH);//sets the power level of transmitting/receiving
  Rservo.attach(3,1000,2000);  // Attaches the servo on pin 9 to the servo object
  Lservo.attach(5,1000,2000);  // Attaches the servo on pin 9 to the servo object

  // Set module as receiver
  radio.startListening();
}

void loop() {
  // Check whether there is data to be received
  if (radio.available()) {
    radio.read(&data, sizeof(Data_Package)); // Read the whole data and store it into the 'data' structure

    //Serial.print("val"); // This should present the value of the potentiometer after it's already been adjusted for the servo
    Serial.println(data.Rval); // Print the value read from the 'data' structure
    Serial.println(data.Lval); //Print the Value of the Lpot
    //Serial.print(data.f); // Should present "Test" in the serial window
    //Serial.println(data.f); // Print the value of 'f' from the 'data' structure
    Rservo.write(data.Rval); // Sets the servo position according to the scaled value (Move to receiver)
    Lservo.write(data.Lval); // Sets the servo position according to the scaled value (Move to receiver)

  }
  delay(10);
}
