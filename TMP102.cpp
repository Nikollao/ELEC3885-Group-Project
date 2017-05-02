/**
@file TMP102.cpp

@brief Implementation of member functions

*/
#include "mbed.h"
#include "TMP102.h"

TMP102::TMP102(PinName sdaPin, PinName sclPin, PinName alertPin)
{
    sensor = new I2C(sdaPin,sclPin); // create new I2C instance and initialise
    
    alert = new DigitalIn(alertPin);
    
    leds = new BusOut(LED_RED,LED_GREEN,LED_BLUE);

}

void TMP102::init()
{
    sensor->frequency(400000);       // I2C Fast Mode - 400kHz
    
    int nack;
    char config_reg_data[3];    
    char config_reg = CONFIG_REG;
   
    nack = sensor->write(TMP102_W_ADD,&config_reg,1); // write CONFIG_REG address to pointer register, returns 0 if successful
    
    if (nack > 0){
        error();  // flash error message if nack is received
    }
        
    config_reg_data[0] = CONFIG_REG;
    config_reg_data[1] = 0x74; // 12 bit, 4 consecutive fault alert, active high alert pin, comparator mode
    config_reg_data[2] = 0x80; // 4 Hz conversion rate
    
    nack = sensor->write(TMP102_W_ADD,config_reg_data,3); // write to configuration register, returns 0 if successful
    
    if (nack > 0) {
        error();  // flash error message if nack is received
    }    
}    

float TMP102::read_temperature()
{
    int nack;  
    char temp_data[2];
    char temp_reg = TEMP_REG;


    nack = sensor->write(TMP102_W_ADD,&temp_reg,1);  // write TEMP_REG address to pointer register, returns 0 if successful 
    
    if (nack > 0) {
        error();  // flash error message if nack is received
    }
    
    nack = sensor->read(TMP102_R_ADD,temp_data,2);  // read 2 bytes from TEMP_REG and store in temp_data, returns 0 if successful
    
    if (nack > 0) {
        error();  // flash error message if nack is received
    }    

    int raw_temperature = (temp_data[0] << 4) | (temp_data[1] >> 4); // 12 MSB of the register

    float temperature = raw_temperature*0.0625F;
    
    return temperature;
}

void TMP102::low_threshold(float low_t)
{
    int nack;
    int T; 
    char low_threshold_reg_data[3];
    char t_low_reg = T_LOW_REG;
    
    
    T = floor(low_t/ 0.0625F);
    
    low_threshold_reg_data[0] = T_LOW_REG; // register address
    low_threshold_reg_data[1] = ((T&0xFF0) >> 4); // First 8 MSB of low threshold temperature
    low_threshold_reg_data[2] = ((T&0x00F) << 4); // 4 LSB of low threshold temperature
    
    
    nack = sensor->write(TMP102_W_ADD,&t_low_reg,1);  // write T_LOW_REG address to pointer register, returns 0 if successful
    
    if (nack > 0) {
        error();  // flash error message if nack is received
    }
    
    nack = sensor->write(TMP102_W_ADD,low_threshold_reg_data,3); // write low threshold temperature to T_LOW_REG, returns 0 if successful
    
    if (nack > 0) {
        error(); // flash error message if nack is received
    }  
}

void TMP102::high_threshold(float high_t)
{
    int nack; 
    int T; 
    char high_threshold_reg_data[3];
    char t_high_reg = T_HIGH_REG;

    T = floor(high_t / 0.0625F);
    
    high_threshold_reg_data[0] = T_HIGH_REG; // register address
    high_threshold_reg_data[1] = ((T&0xFF0) >> 4); // First 8 MSB of high threshold temperature
    high_threshold_reg_data[2] = ((T&0x00F) << 4); // 4 LSB of high threshold temperature
    
    
    nack = sensor->write(TMP102_W_ADD,&t_high_reg,1);  // write T_HIGH_REG address to pointer register, returns 0 if successful
    
    if (nack > 0) {
        error();  // flash error message if nack is received
    }

    nack = sensor->write(TMP102_W_ADD,high_threshold_reg_data,3); // write high threshold temperature to T_HIGH_REG, returns 0 if successful
    
    if (nack > 0) {
        error(); // flash error message if nack is received
    }
      
}        

int TMP102::alert_status()
{
    int alert_status = alert->read(); // read ALERT pin
    
    return alert_status;
    
}    

void TMP102::error()
{
    leds->write(0);
    
    while(1) {
        
        wait(0.2);
        
    }    
}