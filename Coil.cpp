/**
@file Coil.cpp

@brief Implementation of member functions

*/
#include "mbed.h"
#include "Coil.h"

COIL::COIL(PinName gatePin)
{
    
    gate = new PwmOut(gatePin);
    
    gate->period_ms(10); // PWM period = 10 ms ---> PWM frequency = 100 Hz
    
    gate->write(1); // duty cycle = 1;
    
}


void COIL::on()
{
    
    gate->write(0.33);
    
} 

void COIL::off()
{
    
    gate->write(1);
    
}      



