#pragma once

#include <hugo_tiny_block_defines.h>

#define I2C_COMMAND_GET_CHARGING_STATE      I2C_COMMAND_RESERVED_MAX + 1 // charging, not charging
//#define I2C_COMMAND_GET_INA_A0A1            I2C_COMMAND_RESERVED_MAX + 2 // A0, A1
#define I2C_COMMAND_SET_INA_A0A1            I2C_COMMAND_RESERVED_MAX + 3
#define I2C_COMMAND_GET_INA_I2C_ADDRESS     I2C_COMMAND_RESERVED_MAX + 4

//not managed by attiny
#define I2C_COMMAND_INA219_COFIGURATION     0x00
#define I2C_COMMAND_INA219_SHUNT_VOLTAGE    0x01
#define I2C_COMMAND_INA219_BUSVOLTAG        0x02
#define I2C_COMMAND_INA219_POWER            0x03
#define I2C_COMMAND_INA219_CURRENT          0x04
#define I2C_COMMAND_INA219_CALIBRATION      0x05

typedef enum
{
    INA219_BRNG_16V = 0,
    INA219_BRNG_32V = 1
} ina219_brng_t;

typedef enum
{
    INA219_PG_GD1_40MV  = 0b00,
    INA219_PG_GD2_80MV  = 0b01,
    INA219_PG_GD4_160MV = 0b10,
    INA219_PG_GD8_320MV = 0b11
} ina219_pg_t;

typedef enum
{
    INA219_BADC_RES_9BIT    = 0b0000,
    INA219_BADC_RES_10BIT   = 0b0001,
    INA219_BADC_RES_11BIT   = 0b0010,
    INA219_BADC_RES_12BIT   = 0b0011
} ina219_badc_res_t;

typedef enum
{
    INA219_SADC_9BIT_84         = 0b0000,
    INA219_SADC_10BIT_148       = 0b0001,
    INA219_SADC_11BIT_276       = 0b0010,
    INA219_SADC_12BIT_532       = 0b0011,
    INA219_SADC_2SAM_1060       = 0b1001,
    INA219_SADC_4SAM_2130       = 0b1010,
    INA219_SADC_8SAM_4260       = 0b1011,
    INA219_SADC_16SAM_8510      = 0b1100,
    INA219_SADC_32SAM_17020     = 0b1101,
    INA219_SADC_64SAM_34050     = 0b1110,
    INA219_SADC_128SAM_68100    = 0b1111
} ina219_sadc_t;

typedef enum
{
    INA219_MODE_POWER_DOWN              = 0b000,
    INA219_MODE_SHUNT_TRIGERED          = 0b001,
    INA219_MODE_BUS_TRIGERED            = 0b010,
    INA219_MODE_SHUNT_BUS_TRIGERED      = 0b011,
    INA219_MODE_ADC_OFF                 = 0b100,
    INA219_MODE_SHUNT_CONTINUOUS        = 0b101,
    INA219_MODE_BUS_CONTINUOUS          = 0b110,
    INA219_MODE_SHUNT_BUS_CONTINUOUS    = 0b111,
} ina219_mode_t;

typedef struct config_reg_t{
    unsigned mode : 3;  // Operating Mode
    unsigned sadc : 4;  // Shunt ADC Resolution/Averaging
    unsigned badc : 4;  // Bus ADC Resolution/Averaging
    unsigned pg :   2;  // PGA (Shunt Voltage Only)
    unsigned brng : 1;  // Bus Voltage Range
    unsigned none : 1;  // not used
    unsigned rst : 1;   // Reset Bit
} config_reg_t;
