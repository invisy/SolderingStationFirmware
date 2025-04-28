#include "inc/serial_communication.h"
#include "inc/commands_list.h"
#include "inc/command_handler.h"
#include "../modules/inc/settings_storage.h"
#include "string.h"

PID_t* pids[1];
uint8_t pidsNumber = 0;


static void handle_get_pid_devices_number_cmd(volatile uint8_t* data, uint8_t data_length, command_response_t* response)
{
    if(data_length != 0)
        (*response).response_code = WRONG_ARGUMENTS_LENGTH;
    
    get_pid_devices_number_response responseModel = { .number = pidsNumber };
    uint8_t modelSize = sizeof(responseModel);

    (*response).response_code = OK;
    (*response).data_length = modelSize;
    memcpy((*response).data, &responseModel, modelSize);
}

static void handle_get_pid_current_temperature_cmd(volatile uint8_t* data, uint8_t data_length, command_response_t* response)
{
    if(data_length != sizeof(get_pid_current_temperature_request))
        (*response).response_code = WRONG_ARGUMENTS_LENGTH;
    
    get_pid_current_temperature_request* request = (get_pid_current_temperature_request*)data;

    if(request->pid_id >= pidsNumber)
        (*response).response_code = WRONG_ARGUMENT_VALUE;

    get_pid_current_temperature_response responseModel = { .temperature = pids[request->pid_id]->currentTemperature };
    uint8_t modelSize = sizeof(responseModel);

    (*response).response_code = OK;
    (*response).data_length = modelSize;
    memcpy((*response).data, &responseModel, modelSize);
}

static void handle_get_pid_expected_temperature_cmd(volatile uint8_t* data, uint8_t data_length, command_response_t* response)
{
    if(data_length != sizeof(get_pid_expected_temperature_request))
        (*response).response_code = WRONG_ARGUMENTS_LENGTH;
    
    get_pid_expected_temperature_request* request = (get_pid_expected_temperature_request*)data;

    if(request->pid_id >= pidsNumber)
        (*response).response_code = WRONG_ARGUMENT_VALUE;

    get_pid_expected_temperature_response responseModel = { .temperature = pids[request->pid_id]->expectedTemperature };
    uint8_t modelSize = sizeof(responseModel);

    (*response).response_code = OK;
    (*response).data_length = modelSize;
    memcpy((*response).data, &responseModel, modelSize);
}

static void handle_get_pid_coefficients_cmd(volatile uint8_t* data, uint8_t data_length, command_response_t* response)
{
    if(data_length != sizeof(get_pid_coefs_request))
        (*response).response_code = WRONG_ARGUMENTS_LENGTH;
    
    get_pid_coefs_request* request = (get_pid_coefs_request*)data;

    if(request->pid_id >= pidsNumber)
        (*response).response_code = WRONG_ARGUMENT_VALUE;

    get_pid_coefs_response responseModel = 
    { 
        .kp = pids[request->pid_id]->kp, 
        .ki = pids[request->pid_id]->ki, 
        .kd = pids[request->pid_id]->kd
    };
    uint8_t modelSize = sizeof(responseModel);

    (*response).response_code = OK;
    (*response).data_length = modelSize;
    memcpy((*response).data, &responseModel, modelSize);
}

static void handle_set_pid_expected_temperature_cmd(volatile uint8_t* data, uint8_t data_length, command_response_t* response)
{
    if(data_length != sizeof(set_expected_temperature_request))
        (*response).response_code = WRONG_ARGUMENTS_LENGTH;
    
    set_expected_temperature_request* request = (set_expected_temperature_request*)data;

    if(request->pid_id >= pidsNumber || request->temperature < 0 || request->temperature > 512)
        (*response).response_code = WRONG_ARGUMENT_VALUE;

    
    pids[request->pid_id]->expectedTemperature = request->temperature;

    (*response).response_code = OK;
    (*response).data_length = 0;
}

static void handle_set_pid_kp_coef_cmd(volatile uint8_t* data, uint8_t data_length, command_response_t* response)
{
    if(data_length != sizeof(set_pid_kp_coef_request))
        (*response).response_code = WRONG_ARGUMENTS_LENGTH;
    
    set_pid_kp_coef_request* request = (set_pid_kp_coef_request*)data;

    if(request->pid_id >= pidsNumber || request->kp < 0 || request->kp > 100)
        (*response).response_code = WRONG_ARGUMENT_VALUE;

    set_pid_coefficient(PID_KP_ADDRESS, request->pid_id, request->kp);
    pids[request->pid_id]->kp = request->kp;

    (*response).response_code = OK;
    (*response).data_length = 0;
}

static void handle_set_pid_ki_coef_cmd(volatile uint8_t* data, uint8_t data_length, command_response_t* response)
{
    if(data_length != sizeof(set_pid_ki_coef_request))
        (*response).response_code = WRONG_ARGUMENTS_LENGTH;
    
    set_pid_ki_coef_request* request = (set_pid_ki_coef_request*)data;

    if(request->pid_id >= pidsNumber || request->ki < 0 || request->ki > 100)
        (*response).response_code = WRONG_ARGUMENT_VALUE;

    set_pid_coefficient(PID_KI_ADDRESS, request->pid_id, request->ki);
    pids[request->pid_id]->ki = request->ki;

    (*response).response_code = OK;
    (*response).data_length = 0;
}

static void handle_set_pid_kd_coef_cmd(volatile uint8_t* data, uint8_t data_length, command_response_t* response)
{
    if(data_length != sizeof(set_pid_kd_coef_request))
        (*response).response_code = WRONG_ARGUMENTS_LENGTH;
    
    set_pid_kd_coef_request* request = (set_pid_kd_coef_request*)data;

    if(request->pid_id >= pidsNumber || request->kd < 0 || request->kd > 100)
        (*response).response_code = WRONG_ARGUMENT_VALUE;

    set_pid_coefficient(PID_KD_ADDRESS, request->pid_id, request->kd);
    pids[request->pid_id]->kd = request->kd;

    (*response).response_code = OK;
    (*response).data_length = 0;
}

command_response_t handle_command(uint16_t cmd, volatile uint8_t* data, uint8_t data_length)
{
    command_response_t response = { .response_code = WRONG_COMMAND, .data_length = 0 };

    switch(cmd)
    {
        case GET_PID_DEVICES_NUMBER:
            handle_get_pid_devices_number_cmd(data, data_length, &response);
            break;
        case GET_PID_CURRENT_TEMPERATURE:
            handle_get_pid_current_temperature_cmd(data, data_length, &response);
            break;
        case GET_PID_EXPECTED_TEMPERATURE:
            handle_get_pid_expected_temperature_cmd(data, data_length, &response);
            break;
        case GET_PID_COEFS:
            handle_get_pid_coefficients_cmd(data, data_length, &response);
            break;
        case SET_PID_EXPECTED_TEMPERATURE:
            handle_set_pid_expected_temperature_cmd(data, data_length, &response);
            break;
        case SET_PID_KP_COEF:
            handle_set_pid_kp_coef_cmd(data, data_length, &response);
            break;
        case SET_PID_KI_COEF:
            handle_set_pid_ki_coef_cmd(data, data_length, &response);
            break;
        case SET_PID_KD_COEF:
            handle_set_pid_kd_coef_cmd(data, data_length, &response);
            break;
        default:
        {
            response.response_code = WRONG_COMMAND;
            response.data_length = 0;
        }
    }

    return response;
}

void command_handler_init(PID_t* pid, uint8_t length)
{
    pids[0] = pid;
    pidsNumber = length;
}