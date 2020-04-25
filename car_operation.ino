/*
 * EGEN 310 VEHICLE CODE
 * @author Andy Kirby
 * Written for an Arduino UNO paired with a HC05 BT Module and a 2 channel motor driver.
 * 
 * The program is structured to initialize the car with brakes on and ready to receive 
 * data from the bluetooth module. After this, the program continously loops, waiting
 * for commands and executing them.
 * 
 * Accepts commands for forwards, backwards, left, right, and brake.
 * Also has commands to control speed. Current states are tracked in state variables.
 * States like speed have adjustable parameters like step between speeds and maximum
 * speed allowed. This allows the car to be adjustable and prevents oversupplying the
 * motors.
 */

#include <SoftwareSerial.h> 
SoftwareSerial bt(3, 2); // RX | TX 

// CONSTANTS
boolean TESTING = true; // Set to true if your computer is connected
int MAX_SPEED = 80; //125; //200
int SPEED_STEP = 5; //25; //25

// PIN ASSIGNMENTS
int R_MOTOR = 9;
int IN_1 = 5;
int IN_2 = 6;
int L_MOTOR = 10;
int IN_3 = 7;
int IN_4 = 8;

// STATE VARIABLES
int command = 0; // The last received command
int direction = 0; // Current direction
int speed = 0; // Current speed (relates to power sent to motors)

// INITIALIZATION
void setup() 
{   
  if(TESTING) { Serial.begin(9600); } // TEST ONLY --- Start serial connection to computer COM port 
  bt.begin(9600); // Begin serial connection to HC-05 BT Module
  if(TESTING) { Serial.println("Ready to connect\nDefault password is 1234 or 000"); } // TEST ONLY

  // Initialize pins
  // pins are initialized as outputs with low states (so that the motor driver starts in the off state)
  pinMode(R_MOTOR, OUTPUT);
  pinMode(L_MOTOR, OUTPUT);
  pinMode(IN_1, OUTPUT);
  pinMode(IN_2, OUTPUT);
  pinMode(IN_3, OUTPUT);
  pinMode(IN_4, OUTPUT);
  digitalWrite(R_MOTOR, 0);
  digitalWrite(L_MOTOR, 0);
  digitalWrite(IN_1, 0);
  digitalWrite(IN_2, 0);
  digitalWrite(IN_3, 0);
  digitalWrite(IN_4, 0);
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
        move(command);
        break;
      case 'L': // LEFT
        move(command);
        break;
      case 'S': // STOP ALL
        direction = '0'; 
        stopAll();
        break;
      case '+': // SPEED UP
        // Make sure speed is within correct limits
        if(speed + SPEED_STEP < MAX_SPEED) { speed += SPEED_STEP; } 
        if(TESTING) { Serial.print("SPEED: "); Serial.println(speed); } // TEST ONLY
        move(direction);
        break;
      case '-': // SPEED DOWN
        // Make sure speed is within correct limits
        if(speed - SPEED_STEP >= 0) { speed -= SPEED_STEP; } 
        if(TESTING) { Serial.print("SPEED: "); Serial.println(speed); } // TEST ONLY
        move(direction);
        break;
       
      default: // DO NOTHING
        break;      
    }
  }
}

/* MOVE
 * Moves the car in the direction specified by updating motor 
 * driver controls.
 */
void move(int _direction) {
  if(TESTING) { Serial.println("MOVING...");} // TEST ONLY
  // Update direction state
  direction = _direction;

  // Write speed of 0 to both motors while control pins are set
  // This is per recommendation of manufacturer and to protect the circuitry
  analogWrite(R_MOTOR, 0);
  analogWrite(L_MOTOR, 0);
  // ----- Forward ----------------------------------------------
  if(direction == 'F') {
    // Set motor driver control pins for forward
    digitalWrite(IN_1, 1); digitalWrite(IN_2, 0); // R MOTOR
    digitalWrite(IN_3, 1); digitalWrite(IN_4, 0); // L MOTOR
  // ----- Backward----------------------------------------------  
  } else if(direction == 'B') {    
    // Set motor driver control pins for backward
    digitalWrite(IN_1, 0); digitalWrite(IN_2, 1); // R MOTOR
    digitalWrite(IN_3, 0); digitalWrite(IN_4, 1); // L MOTOR
  // ----- Left -------------------------------------------------    
  } else if(direction == 'L') {
    // Set motor driver control pins
    digitalWrite(IN_1, 1); digitalWrite(IN_2, 0); // R MOTOR - F
    digitalWrite(IN_3, 0); digitalWrite(IN_4, 1); // L MOTOR - B
  // ----- Right------------------------------------------------- 
  } else if(direction == 'R') { 
    // Set motor driver control pins
    digitalWrite(IN_1, 0); digitalWrite(IN_2, 1); // R MOTOR - B
    digitalWrite(IN_3, 1); digitalWrite(IN_4, 0); // L MOTOR - F
  } else {
    stopAll();
    return; // Exit early
  }

  // Write the speed to both the motors
  analogWrite(R_MOTOR, speed);
  analogWrite(L_MOTOR, speed); 
}

/* STOPALL
 * Immediately applies a break command to the motor driver and then
 * drives no speed/power to the motors.
 */
void stopAll() {
  if(TESTING) { Serial.println("STOPPING ALL...");} // TEST ONLY
  
  // Set motor driver control pins for brake
  digitalWrite(IN_1, 0); digitalWrite(IN_2, 0); // MOTOR 1
  digitalWrite(IN_3, 0); digitalWrite(IN_4, 0); // MOTOR 2
    
  // Write the no speed to both the motors
  analogWrite(R_MOTOR, 0);
  analogWrite(L_MOTOR, 0);
}
