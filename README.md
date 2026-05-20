# BNO055-with-FT260
Project to use BNO055 9 DOF IMU with FT260 based I2C to USB interface

I needed to work with a BNO055 IMU for an embedded work project. In order to understand all of the components of this I developed this MSVC C++ project.

The Bosch BNO055 is a 9 DOF MEMs sensor. I used a version packaged on an Adafruit board. Details are here: https://learn.adafruit.com/adafruit-bno055-absolute-orientation-sensor/overview

The BNO055 supports I2C and UART interfaces and I was proved with a Mikroe USB to I2C Click board. Details are here: https://www.mikroe.com/usb-i2c-click

The Mikroe board uses a FT260 device to provide USB services. FT260 is a widely used USB interface manufactuired by [FTDI](https://ftdichip.com/)

FTDI provides a FT260 Test Utility (with C# source code). Link https://ftdichip.com/wp-content/uploads/2025/10/FT260_Test_Utility_v1.0.0.2.zip 

I used the FT260 C/C++ library available for download from this page https://ftdichip.com/software-examples/
Direct link: https://ftdichip.com/wp-content/uploads/2025/08/LibFT260-v1.1.9.zip

FT260 Library App Note: https://ftdichip.com/wp-content/uploads/2025/10/AN_565_UserGuide_For_FT260_Test_Utility_v1.0.pdf
https://ftdichip.com/software-examples/


### Things to learn:
1. I2C
2. FT260 and USB on Windows
3. BNO055

You don't need to find any API docs for the Mikroe board. All you need is the pinout. However you need to understand the FT260 and the use of the FT260 library.

Bosch has provided a small library for the BNO055 which is available from https://github.com/boschsensortec/BNO055_SensorAPI

I used many refernces to figure this out. I quite enjoyed a document written by By Phillip Dupree (Mechatronics Lord) 
https://forums.adafruit.com/viewtopic.php?t=92598
https://e2e.ti.com/cfs-file/__key/communityserver-discussions-components-files/908/1738.Guide-_2D00_-How-to-Integrate-BNO055-Library-into-Tiva-Launchpad-Project.pdf



### So What

The entire project is available from this Repo. The Docs folder has most of the sources I used for this. 
