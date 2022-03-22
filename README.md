# RadSensBoard

**RadSensBoard** is an Arduino library for RadSens Geiger counter board. It provides a simple access to all the RadSens board registers.

* Tested with [PlatformIO for VSCode](https://platformio.org/) and [RadSens board 1.5](https://www.tindie.com/products/climateguard/dosimeter-board-with-i2c-radsens-board-arduino/) with CTC-5 tube.
* RadSensBoard library is distributed under the terms of the [MIT license](https://opensource.org/licenses/MIT).
* RadSensBoard library repository is https://github.com/vurdalakov/radsensboard

## Links

* [RadSensBoard library GitHuib repository](https://github.com/vurdalakov/radsensboard)
* [RadSens board datasheet](https://github.com/climateguard/RadSens/tree/master/extras)
* [Manufacturer website](https://climateguard.info/)
* [Manufacturer GitHub repository](https://github.com/climateguard/RadSens)
* [ESPHome custom component sample for RadSens](https://github.com/maaad/RadSens1v2)

## Example

```
#include <Arduino.h>
#include <RadSensBoard.h>

RadSensBoard radSensBoard;

void setup()
{
    Serial.begin(115200);

    radSensBoard.init();
}

void loop()
{
    radSensBoard.readData();
 
    Serial.print("Radiation level (mR/h): ");
    Serial.println(radSensBoard.getRadiationLevelDynamic(), 3);

    delay(2000);
}
```

## Reference

#### init()

Initializes library and checks connection to RadSens board. Returns `true` in success case or `false` if connection to board had failed.

#### readData()

Reads data from the RadSens board.

#### getRadiationLevelDynamic()

Gets radiation level (in mR/h) based on dynamic window. Dynamic window size depends on current radiation level, but is less than 123 sec.

#### getRadiationLevelStatic()

Gets radiation level (in mR/h) based on static window of 500 sec.

#### getPulseCount()

Gets the accumulated number of pulses registered by the RadSens board.

#### resetPulseCount()

Resets the accumulated number of pulses registered by the RadSens board.

#### getFirmwareVersion()

Gets the RadSens board firmware version.

#### getBoardAddress()

Gets the RadSens board I2C address. Default value is `0x66`.

#### setBoardAddress()

Sets the RadSens board I2C address.

#### getHighVoltageGeneratorState()

Gets a value indicating whether the the high-voltage generator is on. Default is on.

#### setHighVoltageGeneratorState()

Turns the high-voltage generator on or off.

#### getCalibrationValue()

Gets the calibration value of the RadSens board (number of pulses per µR). Default value is 105.

#### setCalibrationValue()

Sets the calibration value of the RadSens board (number of pulses per µR).

#### getLedIndicationState()

Gets a value indicating whether the the LED indication is enabled. Default is on.

#### setLedIndicationState()

Turns the LED indication on or off.
