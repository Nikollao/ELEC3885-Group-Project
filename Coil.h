/**
@file Coil.h

@brief Header file containing member functions and variables

*/

#ifndef COIL_H
#define COIL_H


#include "mbed.h"

/**
@brief Library for generating a magnetic field in two coils in parallel
@see https://my.vanderbilt.edu/stormlab/files/2016/04/SA_2016_Drug_Delivery_Accepted.pdf

@brief Revision 1.0

@author Daniel Gibbons
@date   January 2017
 *
 * Example:
 * @code

 #include "mbed.h"
 #include "Coil.h"
 
// Create object for COIL
COIL coil(PTD1);

// Setup UART connection for debugging
Serial pc(USBTX,USBRX);

int main()
{
    init_serial();
    
    init_K64F();
    
    wait (2);
    
    
    while (1) {
      
      coil.on(); // turn coil off
      
      pc.printf("COIL SWITCHED ON"); 
      
      wait(5); // wait 5 seconds
      
      
      coil.off(); // turn coil off
      
      pc.printf("COIL SWITCHED OFF"); 
      
      wait(5); // wait 5 seconds
      
    }
    
}
 * @endcode
 */

class COIL
{
public:

    /** Create a COIL object connected to a PwmOut pin
    *
    * @param gatePin - mbed PwmOut pin 
    * 
    */
    COIL(PinName gatePin);
    
    /** Biases MOSFET to pass 700mA current through the coils
    * 
    */
    void on();
    
    /** Biases MOSFET to stop the current through the coils
    * 
    */
    void off();
    

private: 
    PwmOut* gate;
};

#endif