/**
@file main.h

@brief Header file containing objects, function declarations and global variables

*/

#ifndef MAIN_H
#define MAIN_H


#define CHAMBER_1 1
#define CHAMBER_2 2
#define CHAMBER_3 3


#include "mbed.h"
#include "Coil.h"
#include "GP2Y0A51SK0F.h"
#include "STEPPER.h"
#include "TMP102.h"

/**
@namespace pc
@brief Setup UART connection for debugging
*/
Serial pc(USBTX,USBRX);
/**
@namespace ble
@brief UART connection for ble
*/
RawSerial  ble(PTC17,PTC16);
/**
@namespace coil
@brief Create object for switching on and off the set of coils
*/
COIL coil(PTD1);
/**
@namespace distance_sensor
@brief Create object for GP2Y0A51SK0F Distance Sensor for finding the position of the magnet
*/
GP2Y0A51SK0F distance_sensor(PTB2);
/**
@namespace tmp102
@brief Create object for TMP102 Temperature Sensor (SCL = PTE24) (SDA = PTE25)
*/
TMP102 tmp102(I2C_SDA,I2C_SCL,PTD1);
/**
@namespace stepper
@brief Create object for RoHs 28BYJ-48 Stepper Motor for rotating the drug chambers
*/
STEPPER stepper(PTB3,PTB10,PTB11,PTC11);
/**
@namespace temp_check
@brief Create Ticker for checking providing periodic feedback to the app
*/
Ticker temp_check;
/**
@namespace leds
@brief K64F on-board leds
*/
BusOut leds(LED_RED,LED_GREEN,LED_BLUE);
/**
@namespace sw2
@brief Interrupt for K64F on-board SW2
*/
InterruptIn sw2(SW2);
/**
@namespace sw3
@brief Interrupt for K64F on-board SW3
*/
InterruptIn sw3(SW3);


///
/// Initialisation Functions
///
void initSerial(); /*!< setup serial port */
void initK64F(); /*!< set-up the on-board LEDs and switches */
///
/// Interrupt Service Routines (ISRs)
///
void sw2_isr(); /*!< interrupt service routine for K64F SW2 */
void sw3_isr(); /*!< interrupt service routine for K64F SW3 */
void temperature_isr(); /*!< interrupt service routine for reading the temperature */
void ble_recv_connection(); /*!< interrupt service routine for establishing a connection */
void ble_recv_command(); /*!< interrupt service routine for receiving commands */
///
/// Temperature Check Function 
///
void checkTemperature(); /*!< checks the status of the tmp102 alert pin and chooses the next state of the program based on the result */
///
/// Magnet Position Functions
///
int getMagnetPosition(int current_drug_chamber); /*!< gets position of magnet in the current chamber that is aligned with the coils */
int getMagnetPositionSensor(int current_drug_chamber); /*!< gets position of magnet in the current chamber that is aligned with the coils */
void updateMagnetPosition(int current_drug_chamber); /*!< stores the position of the magnet in the current chamber */
void updateMagnetPositionSensor(int current_drug_chamber); /*!< stores the position of the magnet in the current chamber */
///
/// State Functions
///
void initialisation(); /*!< state 0 protocol */
void connectBluetooth(); /*!< state 1 protocol */
void bluetoothConnected(); /*!< state 2 protocol */
void awaitingCommandBLE(); /*!< state 3 protocol */
void commandReceived(); /*!< state 4 protocol */
void selectChamber(); /*!< state 5 protocol */
void dispenseDrug(); /*!< state 6 protocol */
void temperatureExceeded(); /*!< state 7 protocol */
void bluetoothLost(); /*!< state 8 protocol */
void stateSelection(); /*!< executes state transition */


typedef void (*STATE_FUNCTION)(); /*!< defines a void function so the state protocols can be stored in fsm */


struct State {

    int output; /*!< value written to the bus for the on-board leds to indicate the current state */


    /* array of next states which depend on the current temperature and the bluetooth connection status

       - nextState[0] ----> Next State in Drug Delivery Sequence

       - nextState[1] ----> Temperature Threshold Exceeded

       - nextState[2] ----> Bluetooth Connection Lost

    */

    int nextState[3]; /*!< array of next states which depend on the current temperature and the bluetooth connection status */
    
    STATE_FUNCTION stateFunction; /*!< variable to store address of each state protocol function in fsm */

};

typedef const struct State FSM; /*!< defines the struct above so each element of nfsm can be of this type */

FSM fsm[9] = {

    {7,{1,7,8},&initialisation},  // 0 - Initialisation (LEDs off)

    {3,{2,7,8},&connectBluetooth},  // 1 - Attempt Bluetooth Connection (LEDs flashing blue)

    {3,{3,7,8},&bluetoothConnected},  // 2 - Bluetooth Connected (LEDs static blue)

    {5,{4,7,8},&awaitingCommandBLE},  // 3 - Awaiting Command (LEDs static green)

    {5,{5,7,8},&commandReceived},  // 4 - Command Received (LEDs flashing green)

    {2,{6,7,8},&selectChamber},  // 5 - Adjust Chamber Alignment (LEDs static purple)

    {2,{3,7,8},&dispenseDrug},  // 6 - Dispense Drug (LEDs flashing purple)

    {6,{3,7,8},&temperatureExceeded},  // 7 - Temperature Threshold Exceeded (Shutdown) (LEDs flashing red)

    {6,{1,7,8},&bluetoothLost}   // 8 - Bluetooth Connection Lost (LEDs static red)

};



bool command_received = false; /*!< (1 if a command has been received) (0 if no command has been received) */

bool command_confirmed = false; /*!< (1 if a command has been confirmed) (0 if the command has not been confirmed) */

bool connection_status = false; /*!< (1 if MCR is connected to app) (0 if MCR is not connected to app) */

bool echo_sent = false; /*!< (1 when MCR echoes chamber and quantity command received from app) (0 when MCR has not received a new command) */

int current_drug_chamber = CHAMBER_1; /*!< current chamber that is loaded */

int new_drug_chamber = 0; /*!< chamber of drug to be released (1 , 2 , 3) */

int magnet_position_1 = 0; /*!< magnet position in chamber 1 */

int magnet_position_2 = 0; /*!< magnet position in chamber 2 */

int magnet_position_3 = 0; /*!< magnet position in chamber 3 */

int magnet_position_1_sensor = 0; /*!< magnet position in chamber 1 according to distance sensor */

int magnet_position_2_sensor = 0; /*!< magnet position in chamber 2 according to distance sensor */

int magnet_position_3_sensor = 0; /*!< magnet position in chamber 3 according to distance sensor */

int required_magnet_position = 0; /*!< goal position of magnet in the chamber to release a given quantity of drug (1-25%,2-50%,3-75%,4-100%) */

int current_magnet_position = 0; /*!< current position of magnet in the current chamber that is loaded */

float current_magnet_position_sensor = 0; /*!< current position of magnet in the current chamber that is loaded according to distance sensor */

int g_current_state = 0; /*!< current state the program is in */

int g_next_state = 0; /*!< next state the program is going to enter */

volatile int g_temperature_flag = 0; /*!< flag for temperature isr */

volatile int g_sw2_flag = 0; /*!< flag for K64F SW2 interrupt */

volatile int g_sw3_flag = 0;/*!< flag for K64F SW3 interrupt */

#endif