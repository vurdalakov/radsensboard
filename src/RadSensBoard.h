// An Arduino library for RadSens Geiger counter board
// (c) 2022 Vurdalakov. MIT license
// https://github.com/vurdalakov/radsensboard

#ifndef __RADSENSBOARD_H__
#define __RADSENSBOARD_H__

#include <stdint.h>

#define RADSENSBOARD_DEFAULT_I2C_ADDRESS 0x66

class RadSensBoard
{
private:

    uint8_t _boardAddress;
    uint8_t _rawData[21] = {0};
    float _radiationLevelDynamic;
    float _radiationLevelStatic;
    uint32_t _pulseCount = 0;

    bool writeRegister(uint8_t registerAddress, uint8_t registerValue);

public:
    RadSensBoard();
    RadSensBoard(uint8_t boardAddress);
    ~RadSensBoard();

    // board access
    bool init();
    bool readData();

    // radiation data
    float getRadiationLevelDynamic();
    float getRadiationLevelStatic();
    uint32_t getPulseCount();
    void resetPulseCount();

    // board configuration
    uint8_t getFirmwareVersion();
    uint8_t getBoardAddress();
    bool getHighVoltageGeneratorState();
    bool setHighVoltageGeneratorState(bool enable);
    uint16_t getCalibrationValue();
    bool setCalibrationValue(uint16_t calibrationValue);
    bool getLedIndicationState();
    bool setLedIndicationState(bool enable);
};

#endif // __RADSENSBOARD_H__
