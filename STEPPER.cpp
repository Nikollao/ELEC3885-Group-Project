/**
@file STEPPER.cpp

@brief Implementation of member functions

*/
#include "mbed.h"
#include "STEPPER.h"


STEPPER::STEPPER(PinName IN1Pin, PinName IN2Pin, PinName IN3Pin, PinName IN4Pin)
{
    
    // create a new instance of the BusOut class for the motor inputs
    
    motor_inputs = new BusOut(IN1Pin, IN2Pin, IN3Pin, IN4Pin);
    
    sequence_number = 0;
    
    current_chamber = CHAMBER_1;
     
    control[0] = 0x01;
    control[1] = 0x03;
    control[2] = 0x02;
    control[3] = 0x06;
    control[4] = 0x04;
    control[5] = 0x0C;
    control[6] = 0x08;
    control[7] = 0x09;

}

void STEPPER::rotate(int steps_to_move)
{
    
    steps_left = abs(steps_to_move);  // how many steps to take


    if (steps_to_move > 0) { // positive rotation

        direction = CLOCKWISE;

    }

    if (steps_to_move < 0) { // negative rotation

        direction = ANTICLOCKWISE;

    }

  
    while(steps_left > 0) {

        wait_ms(2);
        stepMotor();
        steps_left--;

    }

}

int STEPPER::rotateChamber(int new_chamber)
{
    
    if (((current_chamber == CHAMBER_1) && (new_chamber == CHAMBER_2)) | ((current_chamber == CHAMBER_2) && (new_chamber == CHAMBER_3)) | ((current_chamber == CHAMBER_3) && (new_chamber == CHAMBER_1))) { // 1->2 or 2->3 OR 3->1
        
        // STEP 120 DEGREES IN ANTI CLOCKWISE DIRECTION
        
        direction = ANTICLOCKWISE;
        
        steps_left = 1359;
        
        while(steps_left > 0) {

            wait_ms(2);
            stepMotor();
            steps_left--;

        }
        
    }
    else if (((current_chamber == CHAMBER_1) && (new_chamber == CHAMBER_3)) | ((current_chamber == CHAMBER_2) && (new_chamber == CHAMBER_1)) | ((current_chamber == CHAMBER_3) && (new_chamber == CHAMBER_2))) { // 2->1 or 3->1 OR 1->3
        
        // STEP 120 DEGREES IN CLOCKWISE DIRECTION
        
        direction = CLOCKWISE;
        
        steps_left = 1359; 
        
        while(steps_left > 0) {

            wait_ms(2);
            stepMotor();
            steps_left--;

        }
        
    }
    else { // current_chamber == new_chamber
    
        // do nothing
        
    }

    current_chamber = new_chamber;
    
    return current_chamber;
    
}    

void STEPPER::stepMotor(void)
{
     
    /* SWITCHING SEQUENCE
    *
    * STEP NUMBER -> 0 1 2 3 4 5 6 7
    *
    * IN1 (BLUE)   - 0 0 0 0 0 1 1 1
    * IN2 (PINK)   - 0 0 0 1 1 1 0 0
    * IN3 (YELLOW) - 0 1 1 1 0 0 0 0
    * IN4 (ORANGE) - 1 1 0 0 0 0 0 1
    *
    * CLOCKWISE : STEP 0 ----> STEP 7
    * ANTI-CLOCKWISE : STEP 7 ----> STEP 0 
    *
    */ 
    
    motor_inputs->write(control[sequence_number]);

    findNextInput();

}   

void STEPPER::findNextInput(void)
{
    
    if (direction == CLOCKWISE)
        sequence_number++;

    if (direction == ANTICLOCKWISE)
        sequence_number--;
        
    if (sequence_number > 7)
        sequence_number = 0; // reset sequence number for CLOCKWISE rotation

    if (sequence_number < 0)
        sequence_number = 7; // reset sequence number for ANTICLOCKWISE rotation

}   