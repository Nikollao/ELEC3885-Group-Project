/**
@file STEPPER.h

@brief Header file containing member functions and variables

*/

#ifndef STEPPER_H
#define STEPPER_H


// Defines

#define CHAMBER_1 1 // STARTING POSITION = 0 steps
#define CHAMBER_2 2 // STARTING POSITION = 1359 steps anti-clockwise
#define CHAMBER_3 3 // STARTING POSITION = 1359 steps clockwise
#define CLOCKWISE 1
#define ANTICLOCKWISE 0
#define STEPS 4076 // STEPS = number of steps in a revolution * gear ratio = (360 / 5.625) * 63.684 = 4076


#include "mbed.h"

/**
@brief Library for controlling a RoHs 28BYJ-48 Unipolar Stepper Motor using a ULN2003A Motor Driver for a medical application.
@brief A three armed mechansim will be attached to the motor shaft and each arm will be connected to a separate drug chamber.
@brief Drug Chamber 1 will be at 0 degrees, Drug Chamber 2 will be at 120 degrees and Drug Chamber 3 will be at 240 degrees.
@brief Acknowledgement to Mohannad Rawashdeh for his arduino code outlined in the second link below.
@see http://cdn.instructables.com/ORIG/FQY/RHST/HMVJ7U24/FQYRHSTHMVJ7U24.pdf
@see http://www.instructables.com/id/BYJ48-Stepper-Motor/

@brief Revision 1.0

@author Daniel Gibbons
@date   January 2017
 *
 * Example:
 * @code

 #include "mbed.h"
 #include "STEPPER.h"
 
 #define CHAMBER_1 1
 #define CHAMBER_2 2
 #define CHAMBER_3 3
 
// Create object for stepper motor
// IN1 , IN2 , IN3 , IN4
STEPPER stepper(PTB2,PTB3,PTB10,PTB11); 

// Setup UART connection for debugging
Serial pc(USBTX,USBRX);

int current_drug_chamber = CHAMBER_1;

 int main() {
     
    pc.baud(115200); // make sure terminal software matches this

    while(1) {

        pc.printf("ROTATE STEPPER MOTOR TO CHAMBER 3"); // print to serial connection
       
        current_drug_chamber = stepper.rotateChamber(CHAMBER_3);
        
        wait(2); // delay of 2 seconds
        
        pc.printf("ROTATE STEPPER MOTOR TO CHAMBER 1"); // print to serial connection
        
        current_drug_chamber = stepper.rotateChamber(CHAMBER_1);
        
        wait(2); // delay of 2 seconds

    }
    
}
 * @endcode
 */

class STEPPER
{
public:

    /** Create a STEPPER object connected to four GPIO pins
    *
    * @param IN1 - Input 1 to ULN2003A
    * @param IN2 - Input 2 to ULN2003A
    * @param IN3 - Input 3 to ULN2003A
    * @param IN4 - Input 4 to ULN2003A
    *
    */
    STEPPER(PinName IN1Pin, PinName IN2Pin, PinName IN3Pin, PinName IN4Pin);
    
    /** Turns the stepper motor in either direction by 'steps_to_move'
    *
    * @param steps_to_move - positive to turn clockwise and negative to turn anticlockwise
    * 
    */
    void rotate(int steps_to_move);
    
    /** Rotates the stepper motor so 'new_chamber' is aligned with the coils
    *
    * @param new_chamber - number of the drug chamber that has been chosen
    *
    */
    int rotateChamber(int new_chamber);


private:
    
    /** Steps the motor one step in either direction
    *
    */
    void stepMotor();
    
    /** Finds the next set of inputs to the ULN2003A based on the direction the motor is spinning
    *
    */
    void findNextInput();
    
    

private: 
    
    BusOut* motor_inputs; // four inputs to ULN2003A motor driver
    
    int sequence_number; // determines the values written to 'motor_inputs'
    
    int direction; // 1 = clockwise and 0 = anti-clockwise
    
    int steps_left; // number of steps that need to be rotated
    
    int current_chamber; // the drug chamber which is currently at the top of the three armed propellor
    
    int control[8];
    
    
};

#endif