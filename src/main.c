#include "core/inc/millis.h"
#include "modules/inc/seven_seg_display.h"
#include "modules/inc/pid.h"
#include "modules/inc/heater.h"
#include "modules/inc/thermocouple.h"
#include "modules/inc/settings_storage.h"
#include "inc/config.h"
#include "inc/ui.h"
#include "serial_communication/inc/serial_communication.h"
#include "serial_communication/inc/command_handler.h"

//DISPLAY GPIO
SETUP_PIN(DISPLAY_SEGMENT_A, B, 1);                  //PB1
SETUP_PIN(DISPLAY_SEGMENT_B, D, 6);                  //PD6
SETUP_PIN(DISPLAY_SEGMENT_C, D, 4);                  //PD4
SETUP_PIN(DISPLAY_SEGMENT_D, B, 0);                  //PB0
SETUP_PIN(DISPLAY_SEGMENT_E, D, 7);                  //PD7
SETUP_PIN(DISPLAY_SEGMENT_F, D, 2);                  //PD2
SETUP_PIN(DISPLAY_SEGMENT_G, B, 7);                  //PB7
SETUP_PIN(DISPLAY_SEGMENT_DP, B, 2);                 //PB2

SETUP_PIN(DISPLAY_REGISTER_0, D, 5);                 //PD5
SETUP_PIN(DISPLAY_REGISTER_1, B, 6);                 //PB6
SETUP_PIN(DISPLAY_REGISTER_2, D, 3);                 //PD3


//Buttons GPIOs
//SETUP_PIN(KEYBOARD_INCREASE_TEMP_GPIO, C, 2);        //PC2
//SETUP_PIN(KEYBOARD_DECREASE_TEMP_GPIO, C, 1);        //PC1

//Heater0 GPIO
SETUP_PIN(HEATER0_GPIO, C, 5);                       //PC5

//Thermocouple GPIOs
SETUP_PIN(THERMOCOUPLE0_CS, C, 4);                   //PC4
SETUP_PIN(THERMOCOUPLE0_SCLK, C, 3);                 //PC3
SETUP_PIN(THERMOCOUPLE0_MISO, C, 2);                 //PC2


static Thermocouple_t thermocouple0;
static Heater_t heater0;
static PID_t pids[PIDS_NUMBER];

static SevenSegDisplay_t sevenSegDisplay =
{
    .segments[0] = &DISPLAY_SEGMENT_A,
    .segments[1] = &DISPLAY_SEGMENT_B,
    .segments[2] = &DISPLAY_SEGMENT_C,
    .segments[3] = &DISPLAY_SEGMENT_D,
    .segments[4] = &DISPLAY_SEGMENT_E,
    .segments[5] = &DISPLAY_SEGMENT_F,
    .segments[6] = &DISPLAY_SEGMENT_G,
    .segments[7] = &DISPLAY_SEGMENT_DP,
    .registers[0] = &DISPLAY_REGISTER_0,
    .registers[1] = &DISPLAY_REGISTER_1,
    .registers[2] = &DISPLAY_REGISTER_2
};

//End definitions


static void init_drivers()
{
    //Init display
    seven_seg_display_init(&sevenSegDisplay);

    //Init heater and thermocouple
    heater_init(&heater0, &HEATER0_GPIO);
    thermoCouple_init(&thermocouple0, &THERMOCOUPLE0_CS, &THERMOCOUPLE0_SCLK, &THERMOCOUPLE0_MISO);

    const float pid_period = PID_PERIOD_MS/1000.0;

    //Init PID
    float kp = get_pid_coefficient(KP, 0);
    float ki = get_pid_coefficient(KI, 0);
    float kd = get_pid_coefficient(KD, 0);
    pid_init(&pids[0], kp, ki, kd, pid_period, &thermocouple0, &heater0);

    ui_init(&sevenSegDisplay);

    command_handler_init(&pids[0], PIDS_NUMBER);
    serial_communication_init(9600, handle_command);
}

int main(void)
{
    init_drivers();

    while(1)
    {   
        serial_process();

        MILLIS_DELAY(UPDATE_PID, PID_PERIOD_MS)
        {
            pid_process(&pids[0]);
        }

        MILLIS_DELAY(SHOW_CURRENT_TEMPERATURE, DISPLAY_TEMP_UPDATE_TIME)
        {
            ui_print_current_temperature(pids[0].currentTemperature);
        }
        
        ui_update();
    }
    
    return 0;
}