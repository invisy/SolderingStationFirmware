#include "inc/heater.h"
#include "../core/inc/heater_timer.h"
#include <util/atomic.h>

static Heater_t* heaters[MAX_HEATERS];
static uint8_t heatersNumber = 0;

void heater_set_power_percentage(Heater_t* heater, uint8_t powerPercentage)
{
    if(powerPercentage >= 0 && powerPercentage <= 100)
        heater->powerPercentage = powerPercentage;
}

uint8_t heater_get_current_power_percentage(Heater_t* heater)
{
    return heater->powerPercentage;
}

void heater_disable(Heater_t* heater)
{
    heater->isEnabled = 0;
}

void heater_enable(Heater_t* heater)
{
    heater->isEnabled = 1;
}

void heater_init(Heater_t* heaterPtr, GPIO_t* gpio)
{
    heaterPtr->heaterGpio = gpio;
    heaterPtr->isEnabled = 0;

    gpio_configure_as_output(heaterPtr->heaterGpio);
    gpio_set_high(heaterPtr->heaterGpio);
    heater_set_power_percentage(heaterPtr, 0);

    heaters[heatersNumber] = heaterPtr;
    heatersNumber++;

    static bool firstInit = 1;
    if(firstInit)
    {
        heater_timer_init(interrupt);
        firstInit = 0;
    }
}

void interrupt()
{
    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        for(uint8_t i=0;i<heatersNumber;i++)
        {
            Heater_t* currentHeater = heaters[i];

            bool isActive = currentHeater->currentHalfWave < currentHeater->powerPercentage;
            
            if(currentHeater->isEnabled && isActive && gpio_is_high(currentHeater->heaterGpio))
                gpio_set_low(currentHeater->heaterGpio);
            else if((!currentHeater->isEnabled || !isActive) && gpio_is_low(currentHeater->heaterGpio))
                gpio_set_high(currentHeater->heaterGpio);

            if(currentHeater->currentHalfWave < 99)
                currentHeater->currentHalfWave++;
            else
                currentHeater->currentHalfWave = 0;
        }
    }
}