/**
@file ACTUATOR.h

@brief Header file containing member functions and variables

*/

#ifndef ACTUATOR_H
#define ACTUATOR_H


// Defines

#define FORWARDS 0
#define BACKWARDS 1
#define STEPS 200 // STEPS = number of steps in a revolution * gear ratio = (360 / 1.8) * 1 = 200


#include "mbed.h"

/**
@brief Library for full-stepping a generic bipolar stepper motor linear actuator controlled by a L293D IC.
@brief Acknowledgement to Mohannad Rawashdeh for his arduino code outlined in the second link below.
@see http://cdn.instructables.com/ORIG/FQY/RHST/HMVJ7U24/FQYRHSTHMVJ7U24.pdf
@see http://www.instructables.com/id/BYJ48-Stepper-Motor/

@brief Revision 1.0

@author Daniel Gibbons
@date   February 2017
 *
 * Example:
 * @code

 #include "mbed.h"
 #include "ACTUATOR.h"
 
 
// Create object for Linear Actuator
// IN1 , IN2 , IN3 , IN4
ACTUATOR actuator(PTB2,PTB3,PTB10,PTB11); 

// Setup UART connection for debugging
Serial pc(USBTX,USBRX);

int current_distance;

 int main() {
    
    
    wait(3); // wait 3 seconds
       

    while(1) {
        
        pc.printf("FORWARDS"); // print to serial connection
        rotate(280);
        wait(5); // delay of 5 seconds

        pc.printf("BACKWARDS"); // print to serial connection
        rotate(-280);
        wait(5); // delay of 5 seconds

    }
    
}
 * @endcode
 */

class ACTUATOR
{
public:

    /** Create a ACTUATOR object connected to four GPIO pins
    *
    * @param IN1 - Input 1 to L293D IN1
    * @param IN2 - Input 2 to L293D IN1
    * @param IN3 - Input 3 to L293D IN1
    * @param IN4 - Input 4 to L293D IN4
    *
    */
    ACTUATOR(PinName IN1Pin, PinName IN2Pin, PinName IN3Pin, PinName IN4Pin);
    
    /** Turns the stepper motor in either direction by 'steps_to_move'
    *
    * @param steps_to_move - positive to actuate forwards/anti-clockwise and negative to actuate backwards/clockwise
    * 
    */
    void rotate(int steps_to_move);
    
    /** Actuates the nut by a given distance (mm) in either direction
    *
    * @param distance_to_move - positive to actuate forwards/anti-clockwise and negative to actuate backwards/clockwise
    * 
    */
    int actuate(int distance_to_move);
  


private:
    
    /** Steps the motor one step in either direction
    *
    */
    void stepMotor();
    
    /** Finds the next set of inputs to the L293D based on the direction the motor is turning
    *
    */
    void findNextInput();
    
    

private: 
    
    BusOut* motor_inputs; // four inputs to L293D IC
    
    int sequence_number; // determines the values written to 'motor_inputs'
    
    int direction; // 0 = FORWARDS and 1 = BACKWARDS
    
    int steps_left; // number of steps that need to be rotated
    
    int current_distance; // the distance the nut on the screw of the actuator is from the motor
    
    int control[4];
    
    
};

#endif
