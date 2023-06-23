#include "inc/heater.h"
#include "../core/inc/heater_timer.h"

static volatile Heater_t* heaters[MAX_HEATERS];
static int heatersNumber = 0;

void heater_set_power_percentage(volatile Heater_t* heater, int powerPercentage)
{
    if(powerPercentage >= 0 && powerPercentage <= 100)
        heater->powerPercentage = powerPercentage;
}

int heater_get_current_power_percentage(volatile Heater_t* heater)
{
    return heater->powerPercentage;
}

void heater_disable(volatile Heater_t* heater)
{
    heater->isEnabled = 0;
}

void heater_enable(volatile Heater_t* heater)
{
    heater->isEnabled = 1;
}

void heater_init(volatile Heater_t* heaterPtr, GPIO_t* gpio)
{
    Heater_t heater = {
        .heaterGpio = gpio,
        .isEnabled = 0
    };

    gpio_configure_as_output(heater.heaterGpio);
    gpio_set_high(heater.heaterGpio);
    heater_set_power_percentage(&heater, 0);

    heaters[heatersNumber] = heaterPtr;
    heatersNumber++;
    *heaterPtr = heater;

    static bool firstInit = 1;
    if(firstInit)
    {
        heater_timer_init(interrupt);
        firstInit = 0;
    }
}

void interrupt()
{
    for(int i=0;i<heatersNumber;i++)
    {
        volatile Heater_t* currentHeater = heaters[i];

        bool isActive = currentHeater->currentHalfWave < currentHeater->powerPercentage;

        if(currentHeater->isEnabled && isActive && gpio_is_high(currentHeater->heaterGpio))
            gpio_set_low(currentHeater->heaterGpio);
        else if(!isActive && gpio_is_low(currentHeater->heaterGpio))
            gpio_set_high(currentHeater->heaterGpio);

        if(currentHeater->currentHalfWave < 99)
            currentHeater->currentHalfWave++;
        else
            currentHeater->currentHalfWave = 0;
    }
}