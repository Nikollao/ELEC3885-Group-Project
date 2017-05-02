/**
@file GP2Y0A51SK0F.cpp

@brief Implementation of member functions

*/
#include "mbed.h"
#include "GP2Y0A51SK0F.h"

GP2Y0A51SK0F::GP2Y0A51SK0F(PinName sensor_outputPin)
{
    
    sensor_output = new AnalogIn(sensor_outputPin);

}


void GP2Y0A51SK0F::init()
{
    wait_ms(30); // output voltage is unstable up to 25.2 ms after device is switched on
}    

float GP2Y0A51SK0F::read_distance()
{
    
    float adc_raw = sensor_output->read(); // read sensor output voltage
    
    
    float distance = (1434*pow(adc_raw,4))-(3441.7F*pow(adc_raw,3))+(2913.9F*pow(adc_raw,2))-(1092.7F*adc_raw)+172.16F; // equation from MS Excel
    
    
    return distance;
       
       
}    

