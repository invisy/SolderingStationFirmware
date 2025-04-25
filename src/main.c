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

//Heater0 GPIO
SETUP_PIN(HEATER0_GPIO, C, 5);                       //PC5

//Thermocouple GPIOs
SETUP_PIN(THERMOCOUPLE0_SCLK, C, 4);                 //PC4
SETUP_PIN(THERMOCOUPLE0_CS, C, 3);                   //PC3
SETUP_PIN(THERMOCOUPLE0_MISO, C, 2);                 //PC2

//Buttons GPIOs
SETUP_PIN(KEYBOARD_DECREASE_TEMP_GPIO, C, 1);        //PC1
SETUP_PIN(KEYBOARD_INCREASE_TEMP_GPIO, C, 0);        //PC0
SETUP_PIN(STAND_DETECT_GPIO, B, 4);        //PB4


static Thermocouple_t thermocouple0;
static Heater_t heater0;
static PID_t pid0;

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

    // Configuring buttons
    gpio_configure_as_input(&KEYBOARD_INCREASE_TEMP_GPIO);
    gpio_configure_as_input(&KEYBOARD_DECREASE_TEMP_GPIO);
    gpio_configure_as_input(&STAND_DETECT_GPIO);
    gpio_pullup_on(&KEYBOARD_INCREASE_TEMP_GPIO);
    gpio_pullup_on(&KEYBOARD_DECREASE_TEMP_GPIO);
    gpio_pullup_on(&STAND_DETECT_GPIO);

    //Init PID0
    Q15_t kp = get_pid_coefficient(KP, 0);
    Q15_t ki = get_pid_coefficient(KI, 0);
    Q15_t kd = get_pid_coefficient(KD, 0);

    const Q15_t pid_period = Q15_div(Q15_from_int(PID_PERIOD_MS), Q15_from_int(1000));
    pid_init(&pid0, kp, ki, kd, pid_period);

    ui_init(&sevenSegDisplay);

    command_handler_init(&pid0, PIDS_NUMBER);
    serial_communication_init(9600, handle_command);
}

int main(void)
{
    init_drivers();
    uint32_t show_expected_temperature_cycles = 0;
    uint32_t number_of_cycles = 1000/DISPLAY_TEMP_UPDATE_TIME*2;
    
    uint8_t is_on_stand = 0;
    Q15_t is_on_stand_remembered_temperature = 0;

    heater_enable(&heater0);

    while(1)
    {   
        serial_process();

        MILLIS_DELAY(UPDATE_PID, PID_PERIOD_MS)
        {
            Q15_t temp = thermoCouple_get_temperature(&thermocouple0);
            if(temp > Q15_from_int(PROTECTION_TEMPERATURE))
            {
                //Overheating protection
                pid0.expectedTemperature = 0;
                heater_disable(&heater0);
            }

            uint8_t pid_result = pid_process(&pid0, temp);
            heater_set_power_percentage(&heater0, pid_result);
        }

        MILLIS_DELAY(SHOW_CURRENT_TEMPERATURE, DISPLAY_TEMP_UPDATE_TIME)
        {
            if (show_expected_temperature_cycles > 0)
            {
                show_expected_temperature_cycles--;
                if (is_on_stand)
                    ui_print_desired_temperature(Q15_to_int(is_on_stand_remembered_temperature));
                else
                    ui_print_desired_temperature(Q15_to_int(pid0.expectedTemperature));
            }
            else
            {
                ui_print_current_temperature(Q15_to_int(pid0.currentTemperature), is_on_stand);
            }
        }
        
        if(gpio_is_low(&KEYBOARD_INCREASE_TEMP_GPIO))
        {
            static int8_t confirm_timeout = 0;

            MILLIS_DELAY(KEYBOARD_TIMEOUT, 15)
            {
                if(gpio_is_low(&KEYBOARD_INCREASE_TEMP_GPIO))
                    confirm_timeout++;
            }

            if(confirm_timeout == 2)
            {
                confirm_timeout = 0;
                MILLIS_DELAY(KEYBOARD_INCREASE_TEMP_CLICK, BUTTONS_PAUSE)
                {
                    if (is_on_stand)
                    {
                        if (is_on_stand_remembered_temperature < Q15_from_int(MAX_HEATER_TEMPERATURE))
                            is_on_stand_remembered_temperature += Q15_from_int(10);
                    }
                    else if(pid0.expectedTemperature < Q15_from_int(MAX_HEATER_TEMPERATURE))
                    {
                        pid0.expectedTemperature += Q15_from_int(10);
                    }
                    show_expected_temperature_cycles = number_of_cycles;
                }
            }
        } 
        
        if(gpio_is_low(&KEYBOARD_DECREASE_TEMP_GPIO))
        {
            static int8_t confirm_timeout = 0;

            MILLIS_DELAY(KEYBOARD_TIMEOUT, 15)
            {
                if(gpio_is_low(&KEYBOARD_DECREASE_TEMP_GPIO))
                    confirm_timeout++;
            }
            
            if(confirm_timeout == 2)
            {
                confirm_timeout = 0;
                MILLIS_DELAY(KEYBOARD_DECREASE_TEMP_CLICK, BUTTONS_PAUSE)
                {
                    if (is_on_stand)
                    {
                        if (is_on_stand_remembered_temperature >= Q15_from_int(10))
                            is_on_stand_remembered_temperature -= Q15_from_int(10);
                    
                    }
                    else if(pid0.expectedTemperature >= Q15_from_int(10))
                    {
                        pid0.expectedTemperature -= Q15_from_int(10);
                    }
                    show_expected_temperature_cycles = number_of_cycles;
                }
            }
        }

        MILLIS_DELAY(STAND_DETECT, STAND_PAUSE)
        {
            static int8_t confirm_timeout = 0;
            MILLIS_DELAY(KEYBOARD_TIMEOUT, 15)
            {
                if(gpio_is_low(&STAND_DETECT_GPIO))
                    confirm_timeout++;
            }

            if(confirm_timeout == 2 && gpio_is_low(&STAND_DETECT_GPIO))
            {
                confirm_timeout = 0;
                if (is_on_stand == 0)
                {
                    is_on_stand_remembered_temperature = pid0.expectedTemperature;
                    pid0.expectedTemperature = 0;
                }
                is_on_stand = 1;
            }
            else
            {
                if (is_on_stand == 1)
                {
                    is_on_stand = 0;
                    pid0.expectedTemperature = is_on_stand_remembered_temperature;
                }
            }
        }
        
        ui_update();
    }
    
    return 0;
}