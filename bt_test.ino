/*
 * Code to print out what you are receiving from the BT HC-05 Module.
 */

#include <SoftwareSerial.h> 
SoftwareSerial bt(2, 3); // RX | TX 

// CONSTANTS
boolean TESTING = true; // Set to true if your computer is connected
int LED = 8; // The port our led is on
int R_MOTOR = 10;
int L_MOTOR = 11;

// STATE VARIABLES
int command = 0; // The last recieved command
int speed = 0; // Current speed (relates to power sent to motors)

// INITIALIZATION
void setup() 
{   
  if(TESTING) { Serial.begin(9600); } // TEST ONLY --- Start serial connection to computer COM port 
  bt.begin(9600); // Begin serial connection to HC-05 BT Module
  pinMode(LED, OUTPUT); // Setup up led as output
  if(TESTING) { Serial.println("Ready to connect\nDefault password is 1234 or 000"); } // TEST ONLY
} 
void loop() 
{ 
  // SCAN FOR BLUETOOTH COMMANDS
  if (bt.available()) 
  {
    command = bt.read(); // Update last command
    if(TESTING) { Serial.print("COMMAND RECIEVED: "); Serial.println(char(command)); } // TEST ONLY
    // ------------------------------------------------
    // A new command was received, execute that command
    // ------------------------------------------------
    switch(command) {
      case 'F': // FORWARD
        move(command);
        break;
      case 'B': // BACKWARD
        move(command);
        break;
      case 'R': // RIGHT
        turn(command);
        break;
      case 'L': // LEFT
        turn(command);
        break;
      case 'S': // STOP ALL
        stopAll();
        break;
       
      default: // DO NOTHING
        break;      
    }
  }
}

void move(int direction) {
  if(TESTING) { Serial.println("MOVING...");} // TEST ONLY
}

void turn(int direction) {
  
}

void stopAll() {
  
}
