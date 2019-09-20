# Parking Lot System

The main objective of the Parking Lot System is to control the gate in order to let the vehicles enter and exit the parking place. The system has the below requirements:

-	vehicle detection on both sides of the gate;
-	the gate has to remain open until the car has passed through it completely or until the time has run out;
-	the system has to operate on the batteries.

The system was implemented on PIC18F25K22 microcontroller. For detecting the vehicles 2 simple ultrasonic sensors HC-SR04 were used. The gate was controlled by SG-90 servo motor. The system has LCD screen to indicate the state of the gates.

The following state diagram was designed to fulfil the requirements of the system:
![state diagram](https://github.com/Anna-Little-Bird/Parking-Lot-System/blob/master/Photos/State%20Diagram.jpg)

The initial state of the gate is closed, the LCD is showing “CLOSED”. When the car is approaching the gate one of the ultrasound sensors detects it and launches either of the processes: WELCOME if the car enters the parking lot or GOODBYE if the car exits. The gate opens, the LCD start showing the message depending on the current state of the system. After 5 seconds the system goes back to its initial state.
