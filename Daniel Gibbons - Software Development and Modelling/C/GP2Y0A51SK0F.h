/**
@file GP2Y0A51SK0F.h

@brief Header file containing member functions and variables

*/

#ifndef GP2Y0A51SK0F_H
#define GP2Y0A51SK0F_H


#include "mbed.h"

/**
@brief Library for interfacing with GP2Y0A51SK0F Distance Sensor
@see http://www.sharp-world.com/products/device/lineup/data/pdf/datasheet/gp2y0a51sk_e.pdf

@brief Revision 1.0

@author Daniel Gibbons
@date   November 2016
 *
 * Example:
 * @code

 #include "mbed.h"
 #include "GP2Y0A51SK0F.h"
 
// Create object for GP2Y0A51SK0F
GP2Y0A51SK0F distance_sensor(PTB2);

// Setup UART connection for debugging
Serial pc(USBTX,USBRX);

 int main() {
     
     GP2Y0A51SK0F.init();
     
     pc.baud(115200) // make sure terminal software matches this

    while(1) {

        float distance = distance_sensor.read_distance(); // read distance from sensor
        
        pc.printf("Distance = %.2f mm \n",distance); // print to serial connection
       
        wait(0.5); // delay of 5 seconds before next distance reading

    }
}
 * @endcode
 */

class GP2Y0A51SK0F
{
public:

    /** Create a GP2Y0A51SK0F object connected to an AnalogIn pin
    *
    * @param outputPin - mbed AnalogIn pin 
    * 
    */
    GP2Y0A51SK0F(PinName outputPin);
    
    /** Waits 30 ms as the output voltage is unstable for up to 25.2 ms after the sensor is switched on 
    * 
    */
    void init();
    
    /** Reads distance from GP2Y0A51SK0F distance sensor in mm
    *
    * @returns distance in mm
    * 
    */
    float read_distance();
    

private: 
    AnalogIn* sensor_output;
};

#endif
