
#include <iostream>
#include <vector>
#include <windows.h>
#include "LibFT260.h"
//#include "ft260.h"


#pragma comment(lib, "LibFT260.lib")

extern "C"
{
#include "bno055.h"
}



FT260_STATUS ftStatus;
FT260_HANDLE ftHandle;

uint8_t chipID[256];
uint8_t regAddr = 0x00;
uint8_t bnoAddr = 0x28; // Default BNO055 I2C address


void writeLocatioValuePair(int index, int value)
{
    DWORD written;
    chipID[0] = index;
    chipID[1] = value;
    FT260_I2CMaster_Write(ftHandle, bnoAddr, FT260_I2C_START_AND_STOP, &regAddr, 2, &written);

}

int readLocationAt(int target_location, int bytes_to_read = 1)
{
    DWORD written;
    DWORD bytesRead = 0;
    //uint8_t target_location = index;   // The internal memory address you want to read
    uint8_t buffer[10]{};               // Buffer to hold read data

    // Step 1: Write the target location to the slave
    FT260_I2CMaster_Write(ftHandle, bnoAddr, FT260_I2C_REPEATED_START_AND_STOP, &target_location, 1, &written);

    // Step 2: Read the data back from that location
    FT260_I2CMaster_Read(ftHandle, bnoAddr, FT260_I2C_REPEATED_START_AND_STOP, buffer, bytes_to_read, &bytesRead);
    printf("Read buffer: %02X\r\n", buffer[0]);


    printf("read %d bytes\r\n", bytesRead);
    return chipID[0];
}


/*
s8 BNO055_I2C_bus_write(u8 dev_addr, u8 reg_addr, u8* reg_data, u8 wr_len)  //BNO055_BUS_WRITE_FUNC
{
    DWORD written;
    s8 retVal = FT260_I2CMaster_Write(ftHandle, bnoAddr, FT260_I2C_START_AND_STOP, reg_data, wr_len, &written);

    return retVal;
}


s8 BNO055_I2C_bus_read(u8 dev_addr, u8 reg_addr, u8* reg_data, u8 r_len) //BNO055_BUS_READ_FUNC
{
    DWORD bytesRead;
    s8 retVal = FT260_I2CMaster_Read(ftHandle, bnoAddr, FT260_I2C_REPEATED_START_AND_STOP, reg_data, r_len, &bytesRead);

    return retVal;
}


void delay(u32 m)
{
    Sleep(m);
}
*/

/*
 * bus_read function mapped to the BNO055 library
 */
s8 bno055_bus_read(u8 dev_addr, u8 reg_addr, u8* reg_data, u8 cnt) {
    FT260_STATUS status;
    DWORD bytes_read = 0;
    DWORD written = 0;

    // 1. Write the target register address (no stop bit, preparing for repeated-start)
    //FT260_I2CMaster_Write(ftHandle, bnoAddr, FT260_I2C_START_AND_STOP, &regAddr, 2, &written);
    status = FT260_I2CMaster_Write(ftHandle, dev_addr, FT260_I2C_START, &reg_addr, 1, &written);

    if (status != FT260_OK) {
        return -1; // Error handling
    }

    // 2. Read the requested number of bytes from the device
    status = FT260_I2CMaster_Read(ftHandle, dev_addr, FT260_I2C_REPEATED_START_AND_STOP, reg_data, cnt, &bytes_read);

    if (status != FT260_OK || bytes_read != cnt) {
        return -1;
    }

    return 0; // Success
}

/*
 * bus_write function mapped to the BNO055 library
 */
s8 bno055_bus_write(u8 dev_addr, u8 reg_addr, u8* reg_data, u8 cnt)
{
    FT260_STATUS status;
    uint8_t buffer[64]; // Ensure this is sized correctly (FT260 max write is 60 bytes)
    DWORD written = 0;

    // Construct buffer: Register address followed by data to write
    buffer[0] = reg_addr;
    memcpy(&buffer[1], reg_data, cnt);

    status = FT260_I2CMaster_Write(ftHandle, dev_addr, FT260_I2C_START_AND_STOP, buffer, cnt + 1, &written);

    if (status != FT260_OK) {
        return -1;
    }

    return 0; // Success
}

/*
 * Delay function mapped to the BNO055 library
 */
void bno055_delay_msec(u32 msec)
{
    Sleep(msec);
}


void init_bno055_device() {
    // 1. Initialize FT260 and assign ft260_handle...
    // FT260_OpenByVidPid(...);

    // 2. Reset and set I2C clock to 100kHz
    FT260_I2CMaster_Reset(ftHandle);
    FT260_I2CMaster_Init(ftHandle, 100);

    /*
    // 3. Bind functions to Bosch's structure
    struct bno055_t bno055_dev;
    bno055_dev.bus_read = bno055_bus_read;
    bno055_dev.bus_write = bno055_bus_write;
    bno055_dev.delay_msec = bno055_delay_msec;
    bno055_dev.dev_addr = BNO055_I2C_ADDR1;

    // 4. Initialize the BNO055 API
    bno055_init(&bno055_dev);
    */
}





int main() {

    // 1. Open the FT260 device
    ftStatus = FT260_Open(0, &ftHandle);
    if (ftStatus != FT260_OK) {
        printf("Open failure\r\n");
        return 1;
    }
    else   printf("Connected to FT260\r\n");

    // 2. Configure I2C
    FT260_I2CMaster_Init(ftHandle, 400); // 400kHz


    if (false) //this works
    {
        // Write register address
        DWORD written;
        FT260_I2CMaster_Write(ftHandle, bnoAddr, FT260_I2C_START_AND_STOP, &regAddr, 1, &written);

        // Read 1 byte
        DWORD bytesRead = 0;
        FT260_I2CMaster_Read(ftHandle, bnoAddr, FT260_I2C_START_AND_STOP, &chipID[0], 256, &bytesRead);
        printf("read %d bytes\r\n", bytesRead);

        printf("BNO055 Chip ID: %0X\r\n", (int)chipID[0]); // Should be 0xA0);
        printf("BNO055 Chip ID: %0X\r\n", (int)chipID[1]); // Should be 0xFB);
        printf("BNO055 Chip ID: %0X\r\n", (int)chipID[2]); // Should be 0x32);


        printf("00: ");
        for (int x = 0; x < 256; x++)
        {
            printf("%02X ", chipID[x]);
            if (0 == (x + 1) % 16) printf("\r\n%02X: ", x / 16);
        }


        for (int b = 0; b < 6; b++)
        {
            int page = readLocationAt(b);
            printf("\r\nPage: %d\r\n", page);
        }
    }



    init_bno055_device();


    struct bno055_t myBNO;
    myBNO.bus_read = bno055_bus_read;   //BNO055_BUS_WRITE_FUNC(dev_addr, reg_addr, reg_data, wr_len) 
    myBNO.bus_write = bno055_bus_write;   //BNO055_BUS_WRITE_FUNC(dev_addr, reg_addr, reg_data, wr_len) 
    myBNO.delay_msec = bno055_delay_msec;
    myBNO.dev_addr = BNO055_I2C_ADDR1;
    bno055_init(&myBNO);


    bno055_set_operation_mode(BNO055_OPERATION_MODE_NDOF);

    struct bno055_euler_float_t eulerData;
    bno055_convert_float_euler_hpr_deg(&eulerData);
    printf("Euler Data %f  %f  %f\r\n", eulerData.h, eulerData.p, eulerData.r);

    unsigned char accel_calib_status = 0;
    unsigned char gyro_calib_status = 0;
    unsigned char mag_calib_status = 0;
    unsigned char sys_calib_status = 0;

    auto res = bno055_get_accel_calib_stat(&accel_calib_status);
    if (BNO055_ERROR == res) printf("BNO055_ERROR\r\n");
    else if (BNO055_SUCCESS == res) printf("BNO055_SUCCESS\r\n");

    bno055_get_gyro_calib_stat(&gyro_calib_status);
    bno055_get_mag_calib_stat(&mag_calib_status);
    bno055_get_sys_calib_stat(&sys_calib_status);

    printf("Status: %02X %02X %02X %02X \r\n", accel_calib_status, gyro_calib_status, mag_calib_status, sys_calib_status);


    for (int i = 0; i < 100; i++)
    {
        bno055_convert_float_euler_hpr_deg(&eulerData);
        printf("Euler Data %f  %f  %f\r\n", eulerData.h, eulerData.p, eulerData.r);
        Sleep(200);
    }

    // 4. Close
    FT260_Close(ftHandle);



    return 0;
}

