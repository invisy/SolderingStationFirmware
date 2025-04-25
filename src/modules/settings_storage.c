#include "inc/settings_storage.h"

Q15_t get_pid_coefficient(PIDCoefficientType coefficientType, uint8_t pidId)
{
    return storage_read_dword((uint32_t*)coefficientType + PID_SECTION_SIZE*pidId);
}

void set_pid_coefficient(PIDCoefficientType coefficientType, uint8_t pidId, Q15_t value)
{
    storage_write_dword((uint32_t*)coefficientType + PID_SECTION_SIZE*pidId, value);
}