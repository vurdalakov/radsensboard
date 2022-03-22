// An Arduino library for RadSens Geiger counter board
// (c) 2022 Vurdalakov. MIT license
// https://github.com/vurdalakov/radsensboard

#include <Arduino.h>
#include <RadSensBoard.h>

RadSensBoard radSensBoard;

void setup()
{
    Serial.begin(115200);
    Serial.println();

    if (!radSensBoard.init())
    {
        Serial.println("Device not found");
    }

    Serial.print("Firmware version: ");
    Serial.println(radSensBoard.getFirmwareVersion());

    Serial.print("Board I2C address: 0x");
    Serial.println(radSensBoard.getBoardAddress(), HEX);

    Serial.print("Calibration value: ");
    Serial.println(radSensBoard.getCalibrationValue());

    Serial.print("High-voltage generator: ");
    Serial.println(radSensBoard.getHighVoltageGeneratorState());

    Serial.print("LED Indication state: ");
    Serial.println(radSensBoard.getLedIndicationState());
}

void loop()
{
    if (radSensBoard.readData())
    {
        Serial.println("--------------------------------------------");

        Serial.print("Radiation level (dynamic window): ");
        Serial.print(radSensBoard.getRadiationLevelDynamic(), 3);
        Serial.println(" mR/h");

        Serial.print("Radiation level (static window):  ");
        Serial.print(radSensBoard.getRadiationLevelStatic(), 3);
        Serial.println(" mR/h");

        Serial.print("Pulse count:                      ");
        Serial.println(radSensBoard.getPulseCount());
    }
    else
    {
        Serial.println("Error reading data from the board");
    }

    delay(2000);
}
