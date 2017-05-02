/**
@file TMP102.h

@brief Header file containing member functions and variables

*/

#ifndef TMP102_H
#define TMP102_H

// addresses - ADDO connected to ground
#define TMP102_R_ADD    0x91 
#define TMP102_W_ADD    0x90

// registers
#define TEMP_REG        0x00 // 2 bytes - First 12 bits store data
#define CONFIG_REG      0x01 // 2 bytes
#define T_LOW_REG       0x02 // 2 bytes - First 12 bits store data
#define T_HIGH_REG      0x03 // 2 bytes - First 12 bits store data


#include "mbed.h"

/**
@brief Library for interfacing with TMP102 Temperature Sensor in I2C
@brief Acknowledgement to Craig A. Evans for his basic TMP102 library
@see http://www.ti.com.cn/cn/lit/ds/symlink/tmp102.pdf

@brief Revision 1.0

@author Daniel Gibbons
@date   November 2016
 *
 * Example:
 * @code

 #include "mbed.h"
 #include "TMP102.h"
 
// Create object for TMP102
TMP102 tmp102(I2C_SDA,I2C_SCL,PTB23);

// Setup UART connection for debugging
Serial pc(USBTX,USBRX);

 int main() {
     
     tmp102.init(); // initialise TMP102 temperature sensor
     
     pc.baud(115200) // make sure terminal software matches this

    while(1) {

        float temperature = tmp102.read_temperature(); // read temperature from sensor
        
        pc.printf("Temperature = %.2f degrees\n",temperature); // print to serial connection
       
        wait(0.5); // delay of 0.5 seconds before next temperature reading

    }
}
 * @endcode
 */

class TMP102
{
public:

    /** Create a TMP102 object connected to the specified I2C pins and a GPIO pin
    *
    * @param sdaPin - mbed SDA pin 
    * @param sclPin - mbed SCL pin
    * @param alertPin - mbed GPIO pin
    * 
    */
    TMP102(PinName sdaPin, PinName sclPin, PinName alertPin);
    
    /** Initialise configuration register of TMP102
    *
    */
    void init();
    
    /** Read temperature from TMP102
    *
    * @returns temperature in degrees (float)
    * 
    */
    float read_temperature();
    
    /** Set low threshold temperature for ALERT
    * 
    */
    void low_threshold(float low_t);
    
    /** Set high threshold temperature for ALERT
    *
    */
    void high_threshold(float high_t);
    
    /** Reads status of ALERT pin (1 = ALERT) (0 = NO ALERT)
    *
    * @returns status of ALERT pin (1 = ALERT) (0 = NO ALERT)
    *
    */
    int alert_status();

private: 
    /** Error message if I2C communications breakdown
    *
    */ 
    void error();


private: 
    I2C* sensor; // create instance of I2C class
    
    DigitalIn* alert; // create instance of DigitalIn class
    
    BusOut* leds;
    
};

#endif