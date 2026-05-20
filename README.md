# BNO055-with-FT260
Project to use BNO055 9 DOF IMU with FT260 based I2C to USB interface

I needed to work with a BNO055 IMU for an embedded work project. In order to understand all of the components of this I developed a MSVC C++ project.

The Bosch BNO055 is a 9 DOF MEMs sensor. I used a version packaged on an Adafruit board. Details are here: https://learn.adafruit.com/adafruit-bno055-absolute-orientation-sensor/overview

The BNO055 supports I2C and UART interfaces and I was proved with a Mikroe USB to I2C Click board. Details are here: https://www.mikroe.com/usb-i2c-click

The Mikroe board uses a FT260 device to provide USB services. FT260 is a widely used USB interface manufactuired by [FTDI](https://ftdichip.com/)
FTDI also provides a FT260 Test Utility. Link https://ftdichip.com/wp-content/uploads/2025/10/FT260_Test_Utility_v1.0.0.2.zip 
FT260 Librar App Note: https://ftdichip.com/wp-content/uploads/2025/10/AN_565_UserGuide_For_FT260_Test_Utility_v1.0.pdf

### Things to learn:
1. I2C
2. FT260 and USB on Windows
3. BNO055

You don't need to find any API docs for the Mikroe board. All you need is the pinout. However you need to understand the FT260. FTDI provides a library called LibFT260. 
The GITHub repo is at 



https://github.com/boschsensortec/BNO055_SensorAPI



