// An Arduino library for RadSens Geiger counter board
// (c) 2022 Vurdalakov. MIT license
// https://github.com/vurdalakov/radsensboard

#include <Arduino.h>
#include <Wire.h>

#include "RadSensBoard.h"

#define REGISTER_DEVICE_ID 0x00
#define REGISTER_FIRMWARE_VERSION 0x01
#define REGISTER_RADATION_LEVEL_DYNAMIC 0x03
#define REGISTER_RADATION_LEVEL_STATIC 0x06
#define REGISTER_PULSE_COUNT 0x09
#define REGISTER_BOARD_ADDRESS 0x10
#define REGISTER_HV_GENERATOR_STATE 0x11
#define REGISTER_CALIBRATION_VALUE 0x12
#define REGISTER_LED_INDICATION_STATE 0x14

RadSensBoard::RadSensBoard()
{
    _boardAddress = RADSENSBOARD_DEFAULT_I2C_ADDRESS;
}

RadSensBoard::RadSensBoard(uint8_t boardAddress)
{
    _boardAddress = boardAddress;
}

RadSensBoard::~RadSensBoard()
{
}

/* Initializes library and checks connection to RadSens board. Returns `true` in success case or `false` if connection to board had failed.*/
bool RadSensBoard::init()
{
    Wire.begin();
    Wire.beginTransmission(_boardAddress);
    return (1 == Wire.write(0x0)) && (0 == Wire.endTransmission(true)) && readData();
}

/* Reads data from the RadSens board. */
bool RadSensBoard::readData()
{
    Wire.requestFrom(_boardAddress, sizeof(_rawData));

    for (int i = 0; i < sizeof(_rawData); i++)
    {
        _rawData[i] = Wire.read();
    }

    if (_rawData[REGISTER_DEVICE_ID] != 0x7D)
    {
        return false;
    }

    _radiationLevelDynamic = ((uint32_t)_rawData[REGISTER_RADATION_LEVEL_DYNAMIC] << 16) | ((uint16_t)_rawData[REGISTER_RADATION_LEVEL_DYNAMIC + 1] << 8) | _rawData[REGISTER_RADATION_LEVEL_DYNAMIC + 2];
    _radiationLevelDynamic /= 10000;
    _radiationLevelStatic = ((uint32_t)_rawData[REGISTER_RADATION_LEVEL_STATIC] << 16) | ((uint16_t)_rawData[REGISTER_RADATION_LEVEL_STATIC + 1] << 8) | _rawData[REGISTER_RADATION_LEVEL_STATIC + 2];
    _radiationLevelStatic /= 10000;
    _pulseCount += (_rawData[REGISTER_PULSE_COUNT] << 8) | _rawData[REGISTER_PULSE_COUNT + 1];

    return true;
}

/* Gets radiation level (in mR/h) based on dynamic window (depends on current radiation level, but less than 123 sec). */
float RadSensBoard::getRadiationLevelDynamic()
{
    return _radiationLevelDynamic;
}

/* Gets radiation level (in mR/h) based on static window (500 sec.). */
float RadSensBoard::getRadiationLevelStatic()
{
    return _radiationLevelStatic;
}

/* Gets the accumulated number of pulses registered by the RadSens board. */
uint32_t RadSensBoard::getPulseCount()
{
    return _pulseCount;
}

/* Resets the accumulated number of pulses registered by the RadSens board. */
void RadSensBoard::resetPulseCount()
{
    _pulseCount = 0;
}

/* Gets the RadSens board firmware version. */
uint8_t RadSensBoard::getFirmwareVersion()
{
    return _rawData[REGISTER_FIRMWARE_VERSION];
}

/* Gets the RadSens board I2C address. Default value os 0x66. */
uint8_t RadSensBoard::getBoardAddress()
{
    return _rawData[REGISTER_BOARD_ADDRESS];
}

/* Gets a value indicating whether the the high-voltage generator is on. */
bool RadSensBoard::getHighVoltageGeneratorState()
{
    return 1 == _rawData[REGISTER_HV_GENERATOR_STATE];
}

/* Turns the high-voltage generator on or off. Default is on. */
bool RadSensBoard::setHighVoltageGeneratorState(bool enable)
{
    return writeRegister(REGISTER_HV_GENERATOR_STATE, enable ? 1 : 0);
}

/* Gets the calibration value of the RadSens board (number of pulses per µR). Default value is 105. */
uint16_t RadSensBoard::getCalibrationValue()
{
    return ((uint16_t)_rawData[REGISTER_CALIBRATION_VALUE + 1] << 8) | _rawData[REGISTER_CALIBRATION_VALUE];
}

/* Sets the calibration value of the RadSens board (number of pulses per µR). */
bool RadSensBoard::setCalibrationValue(uint16_t calibrationValue)
{
    return writeRegister(REGISTER_CALIBRATION_VALUE + 1, calibrationValue >> 8) && writeRegister(REGISTER_CALIBRATION_VALUE, calibrationValue & 0xFF);
}

/* Gets a value indicating whether the the LED indication is enabled. Default is on. */
bool RadSensBoard::getLedIndicationState()
{
    return 1 == _rawData[REGISTER_LED_INDICATION_STATE];
}

/* Turns the LED indication on or off. */
bool RadSensBoard::setLedIndicationState(bool enable)
{
    return writeRegister(REGISTER_LED_INDICATION_STATE, enable ? 1 : 0);
}

bool RadSensBoard::writeRegister(uint8_t registerAddress, uint8_t registerValue)
{
    Wire.beginTransmission(_boardAddress);
    Wire.write(registerAddress);
    Wire.write(registerValue);
    return Wire.endTransmission(true);
}
