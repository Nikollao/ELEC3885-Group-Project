/**
@file ACTUATOR.cpp

@brief Implementation of member functions

*/
#include "mbed.h"
#include "ACTUATOR.h"


ACTUATOR::ACTUATOR(PinName IN1Pin, PinName IN2Pin, PinName IN3Pin, PinName IN4Pin)
{
    
    // create a new instance of the BusOut class for the motor inputs
    
    motor_inputs = new BusOut(IN1Pin, IN2Pin, IN3Pin, IN4Pin);
    
    sequence_number = 0;
    
    current_distance = 0;
    
    control[0] = 0x0A;
    
    control[1] = 0x06;
    
    control[2] = 0x05;
    
    control[3] = 0x09;

}

void ACTUATOR::rotate(int steps_to_move)
{
    
    steps_left = abs(steps_to_move);  // how many steps to take


    if (steps_to_move > 0) { // forwards actuation

        direction = FORWARDS;

    }

    if (steps_to_move < 0) { // backwards actuation

        direction = BACKWARDS;

    }

  
    while(steps_left > 0) {

        wait_ms(2);
        stepMotor();
        steps_left--;

    }

}

int ACTUATOR::actuate(int distance_to_move)
{
    
    steps_left = abs(distance_to_move) / 0.1514F;  // how many steps to take


    if (distance_to_move > 0) { // forwards actuation

        direction = FORWARDS;

    }

    if (distance_to_move < 0) { // backwards actuation

        direction = BACKWARDS;

    }

  
    while(steps_left > 0) {

        wait_ms(2);
        stepMotor();
        steps_left--;

    }
    
    current_distance = current_distance + distance_to_move;
    
    return current_distance;
    
}


void ACTUATOR::stepMotor(void)
{
     
    /* SWITCHING SEQUENCE
    *
    * INPUTS -> IN1 IN2 IN3 IN4 
    *
    * 0       -  1   0   1   0 
    * 1       -  0   1   1   0 
    * 2       -  0   1   0   1 
    * 3       -  1   0   0   1 
    *
    * FORWARDS : STEP 3 ----> STEP 0
    * BACKWARDS : STEP 0 ----> STEP 3
    *
    */ 
    
    motor_inputs->write(control[sequence_number]);
    
    findNextInput();

}   

void ACTUATOR::findNextInput(void)
{
    
    if (direction == BACKWARDS)
        sequence_number++;

    if (direction == FORWARDS)
        sequence_number--;
        
    if (sequence_number > 3)
        sequence_number = 0; // reset sequence number for BACKWARDS actuation

    if (sequence_number < 0)
        sequence_number = 3; // reset sequence number for FORWARDS actuation

}   