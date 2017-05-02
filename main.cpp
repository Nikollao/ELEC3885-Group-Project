/**
@file main.cpp

@brief Program and Function implementation

*/


#include "main.h"


int main()
{

    initialisation();


    while (1) {

        stateSelection();

    }

}

void ble_recv_connection()
{   
    while(ble.readable()) {
        char temp = ble.getc();
        if (temp == 'c') {
            connection_status = true;
        }
    }
}

void ble_recv_command()
{   
    
    while(ble.readable()) {
        
        char temp = ble.getc();
        
        if (temp == '?') {
            new_drug_chamber = ble.getc() - 0x30;
            command_received = true;
        }
        else if (temp == '!') {
            required_magnet_position = ble.getc() - 0x30;
            command_received = true;
        }
        else if (temp == 'o') {
            command_confirmed = true;
        }
        else if (temp == 't') {
                float current_temperature = tmp102.read_temperature();
                char message[6];
                sprintf(message,"t%.2f",current_temperature);
                ble.puts(message);
        }   
        else if (temp == 'd') {
                int drug_remaining_1 = 4 - magnet_position_1;
                int drug_remaining_2 = 4 - magnet_position_2;
                int drug_remaining_3 = 4 - magnet_position_3;
                char message[6];
                sprintf(message,"d%d%d%d00",drug_remaining_1,drug_remaining_2,drug_remaining_3);
                ble.puts(message); 
        }
        else if (temp == 'h') {
                int alert = tmp102.alert_status();
                char message[6];
                sprintf(message,"h%d0000",alert);
                ble.puts(message); 
        }
        else if (temp == 'b') {
            connection_status = false;
        }
        else {
            echo_sent = false;
            command_received = false;
            command_confirmed = false;
        }
        
    }
    
}

void initSerial()
{
    pc.baud(9600);  // ensure terminal software matches
}

void initK64F()
{
    // on-board LEDs are active-low, so set they are set high to turn them off
    leds = 7;

    sw2.fall(&sw2_isr);
    sw3.fall(&sw3_isr);

    sw2.mode(PullNone);
    sw3.mode(PullNone);

}

void sw2_isr()
{
    g_sw2_flag = 1;
}

void sw3_isr()
{
    g_sw3_flag = 1;
}

void temperature_isr()
{
    g_temperature_flag = 1;
}

void checkTemperature()
{
    if (tmp102.alert_status()) {

        g_next_state = fsm[g_current_state].nextState[1];

    } else {

        g_next_state = fsm[g_current_state].nextState[0];

    }

}

int getMagnetPosition(int drug_chamber)
{

    switch (drug_chamber) {

        case 1:
            return magnet_position_1;
        case 2:
            return magnet_position_2;
        case 3:
            return magnet_position_3;
        default:
            return 0;

    }

}

int getMagnetPositionSensor(int drug_chamber)
{

    switch (drug_chamber) {

        case 1:
            return magnet_position_1_sensor;
        case 2:
            return magnet_position_2_sensor;
        case 3:
            return magnet_position_3_sensor;
        default:
            return 0;

    }

}

void updateMagnetPosition(int drug_chamber)
{
    
    switch (drug_chamber) {
        
        case 1:
            magnet_position_1 = current_magnet_position;
            break;
        case 2:
            magnet_position_2 = current_magnet_position;
            break;
        case 3:
            magnet_position_3 = current_magnet_position;
            break;
        default:
            break;
        
    }

}

void updateMagnetPositionSensor(int drug_chamber)
{
    
    switch (drug_chamber) {
        
        case 1:
            magnet_position_1_sensor = current_magnet_position_sensor;
            break;
        case 2:
            magnet_position_2_sensor = current_magnet_position_sensor;
            break;
        case 3:
            magnet_position_3_sensor = current_magnet_position_sensor;
            break;
        default:
            break;
        
    }

}

void initialisation()
{   
    // set baud rate for communication with terminal and BLE module, initialize K64F on-board peripherals and configuration register of TMP102
    initSerial();
    initK64F(); 
    tmp102.init(); 
    
    // delay due to unstable disance sensor output when it first receives power
    distance_sensor.init(); 
    
    // read temperature inside the body
    float base_temperature = tmp102.read_temperature(); 
    
    // set threshold temperatures
    tmp102.low_threshold(base_temperature + 0.5F);
    tmp102.high_threshold(base_temperature + 1);

    wait(2);
    
    g_next_state = fsm[g_current_state].nextState[0];
    
}

void connectBluetooth()
{
    g_current_state = g_next_state;
    
    leds = fsm[g_current_state].output;
    
    ble.attach(&ble_recv_connection, Serial::RxIrq);
    
    while (1) {
        
        if (connection_status == true) {
            break;
        }
        
        sleep();
        
    }
    
    g_next_state = fsm[g_current_state].nextState[0];

}

void bluetoothConnected()
{ 
    g_current_state = g_next_state;
    
    leds = fsm[g_current_state].output;
    
    ble.attach(&ble_recv_command, Serial::RxIrq);

    g_next_state = fsm[g_current_state].nextState[0];

}
void awaitingCommandBLE()
{  
    g_current_state = g_next_state;

    leds = fsm[g_current_state].output;
    
    required_magnet_position = 0;
    
    new_drug_chamber = 0;
    
    echo_sent = false;
    
    command_received = false;
    
    command_confirmed = false;
    
    while(1) {
        
        if (command_received == true && echo_sent == false) {

            echo_sent = true;
            command_received = false;
            char message[6];
            sprintf(message,"?%d!%d00",new_drug_chamber,required_magnet_position);
            ble.puts(message);
            
        }
            
        if (command_confirmed == true) {
                    
            g_next_state = fsm[g_current_state].nextState[0];
            break;
                
        }    
        
        if (connection_status == false) {
            
            g_next_state = fsm[g_current_state].nextState[2];
            break;
            
        }    
       
        sleep();
    }
    
    
    
}

void commandReceived()
{ 
    g_current_state = g_next_state;
    
    wait(3);
    
    g_next_state = fsm[g_current_state].nextState[0];
}

void selectChamber()
{
    g_current_state = g_next_state;

    leds = fsm[g_current_state].output;

    current_drug_chamber = stepper.rotateChamber(new_drug_chamber);
    
    wait(2);
    
    g_next_state = fsm[g_current_state].nextState[0];

}

void dispenseDrug()
{
    g_current_state = g_next_state;

    current_magnet_position = getMagnetPosition(current_drug_chamber); // gets position of magnet in the current chamber that is loaded (0,1,2,3,4)
    
    float goal_distance = (required_magnet_position * (30/4)) + 20; // goal distance from distance sensor to the magnet (30 mm = usable length of chamber) (20 mm = length of coil outside chamber + 2.5mm either side of coil);

    coil.on(); // switch on coils

    while (goal_distance > distance_sensor.read_distance()) {// loop until magnet reaches the required position
        
        if (tmp102.alert_status()) {
            break;
        }
        
        wait(0.5);
    }

    coil.off(); // switch off coils

    current_magnet_position = required_magnet_position;

    updateMagnetPosition(current_drug_chamber);
    
    current_magnet_position_sensor = (distance_sensor.read_distance() - 20) / 30;
    
    updateMagnetPositionSensor(current_drug_chamber);

    checkTemperature();
    
    char message[6];
    
    sprintf(message,"s00000");
    
    ble.puts(message);

}



void temperatureExceeded()
{ 
    g_current_state = g_next_state;

    temp_check.attach(&temperature_isr,1);

    while(1) {

        if (g_temperature_flag) {

            g_temperature_flag = 0;

            if (tmp102.alert_status() == 0) {

                g_next_state = fsm[g_current_state].nextState[0];
                
                temp_check.detach();
                
                break;

            }

        }

        sleep();

    }

}

void bluetoothLost()
{
    g_current_state = g_next_state;

    leds = fsm[g_current_state].output; // static red

    wait(5);

    g_next_state = fsm[g_current_state].nextState[0];
    
}

void stateSelection()
{
    if (g_next_state != g_current_state) {
        
        (*(fsm[g_next_state].stateFunction))();

    }

}
