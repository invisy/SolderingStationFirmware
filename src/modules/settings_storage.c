#include "inc/settings_storage.h"

float get_pid_coefficient(PIDCoefficientType coefficientType, char pidId)
{
    return storage_read_float((float*)coefficientType + PID_SECTION_SIZE*pidId);
}

void set_pid_coefficient(PIDCoefficientType coefficientType, char pidId, float value)
{
    storage_write_float((float*)coefficientType + PID_SECTION_SIZE*pidId, value);
}