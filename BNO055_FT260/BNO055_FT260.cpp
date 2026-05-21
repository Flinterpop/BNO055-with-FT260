
#include <iostream>
#include <vector>
#include <windows.h>
#include "LibFT260.h"
#include "conio.h"

#pragma comment(lib, "LibFT260.lib")

extern "C"
{
#include "bno055.h"
}



FT260_STATUS ftStatus;
FT260_HANDLE ftHandle;

struct bno055_t myBNO;


/*
 * bus_read function mapped to the BNO055 library
 */
s8 bno055_bus_read(u8 dev_addr, u8 reg_addr, u8* reg_data, u8 cnt) {
    FT260_STATUS status;
    DWORD bytes_read = 0;
    DWORD written = 0;

    // 1. Write the target register address (no stop bit, preparing for repeated-start)
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
    FT260_STATUS status{};
    uint8_t buffer[64]{}; // Ensure this is sized correctly (FT260 max write is 60 bytes)
    DWORD written = 0;

    // Construct buffer: Register address followed by data to write
    buffer[0] = reg_addr;
    memcpy(&buffer[1], reg_data, cnt);

    status = FT260_I2CMaster_Write(ftHandle, dev_addr, FT260_I2C_START_AND_STOP, buffer, cnt + 1, &written);

    if (status != FT260_OK) return -1;

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

    // 3. Bind functions to Bosch's structure
    myBNO.bus_read = bno055_bus_read;   
    myBNO.bus_write = bno055_bus_write; 
    myBNO.delay_msec = bno055_delay_msec;
    myBNO.dev_addr = BNO055_I2C_ADDR1;  //this 0x28 (default I2C address)
    bno055_init(&myBNO);  // Initialize the BNO055 API


}


void writeAt(const std::string& text, int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    std::cout << text;
}


int main() {

    // 1. Open the FT260 device
    ftStatus = FT260_Open(0, &ftHandle);
    if (ftStatus != FT260_OK) 
    {
        printf("Open failure\r\n");
        return 1;
    }
    else printf("Connected to FT260\r\n");

    // 2. Configure I2C
    FT260_I2CMaster_Init(ftHandle, 400); // 400kHz

    init_bno055_device();

    bno055_set_operation_mode(BNO055_OPERATION_MODE_NDOF);

    // Hide cursor optionally for smooth UI
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(out, &cursorInfo);
    cursorInfo.bVisible = false;
    SetConsoleCursorInfo(out, &cursorInfo);

    // Write headers that stay static
    std::cout << "=== Status Dashboard ===" << std::endl;
    std::cout << "Euler Angles Hdg,Roll,Pitch:" << std::endl;
    std::cout << "Calibration Accel/Gyro/Mag/Sys:" << std::endl;
    std::cout << "Calibration:Mag Cal\r\n3: Fully Calibrated\r\n2: Performance is OK\r\n1: Recommend Figure 8 Cal\r\n0: Do Fig 8 Cal\r\n" << std::endl;


    if (true)
    {
        bno055_set_operation_mode(BNO055_OPERATION_MODE_NDOF);

        struct bno055_euler_float_t eulerData;
        bno055_convert_float_euler_hpr_deg(&eulerData);
        
        unsigned char accel_calib_status = 0;
        unsigned char gyro_calib_status = 0;
        unsigned char mag_calib_status = 0;
        unsigned char sys_calib_status = 0;

        auto res = bno055_get_accel_calib_stat(&accel_calib_status);
        if (BNO055_ERROR == res) printf("BNO055_ERROR\r\n");
        //else if (BNO055_SUCCESS == res) printf("BNO055_SUCCESS\r\n");

        bno055_get_gyro_calib_stat(&gyro_calib_status);
        bno055_get_mag_calib_stat(&mag_calib_status);
        bno055_get_sys_calib_stat(&sys_calib_status);


        char buf[100];
        bool quit = false;
        do
        {

            bno055_convert_float_euler_hpr_deg(&eulerData);
            sprintf_s(buf, "%6.3f %6.3f %6.3f\r\n", eulerData.h, eulerData.p, eulerData.r);
            writeAt(buf, 35, 2);
            puts("Gravity");
            s16 gravity_y_s16;
            bno055_read_gravity_y(&gravity_y_s16);
            s8 temp_s8;
            bno055_read_temp_data(&temp_s8);
            sprintf_s(buf, "Gravity %f  temp: %d\r\n", gravity_y_s16, temp_s8);
            writeAt(buf, 35, 4);

            float accel_x_f, accel_y_f, accel_z_f;
            bno055_convert_float_accel_x_msq(&accel_x_f);
            bno055_convert_float_accel_y_msq(&accel_y_f);
            bno055_convert_float_accel_z_msq(&accel_z_f);
            sprintf_s(buf, "Accel:x: %f y: %f  z: %f\r\n", accel_x_f, accel_y_f, accel_z_f);
            writeAt(buf, 35, 5);

            //Sleep(200);
            if (_kbhit())
            {
                if (GetAsyncKeyState('C') & 0x8000) 
                {

                    puts("Calibrate");
                    auto res = bno055_get_accel_calib_stat(&accel_calib_status);
                    if (BNO055_ERROR == res) printf("BNO055_ERROR\r\n");
                    //else if (BNO055_SUCCESS == res) printf("BNO055_SUCCESS\r\n");

                    bno055_get_gyro_calib_stat(&gyro_calib_status);
                    bno055_get_mag_calib_stat(&mag_calib_status);
                    bno055_get_sys_calib_stat(&sys_calib_status);
                    sprintf_s(buf,"%02X %02X %02X %02X \r\n", accel_calib_status, gyro_calib_status, mag_calib_status, sys_calib_status);
                    writeAt(buf, 35, 3);
                    }
                if (GetAsyncKeyState('Q') & 0x8000)
                {
                    puts("Quit");
                    quit = true;
                }

                if (GetAsyncKeyState('G') & 0x8000)
                {
                    puts("Gravity");
                    s16 gravity_y_s16;
                    bno055_read_gravity_y(&gravity_y_s16);
                    s8 temp_s8;
                    bno055_read_temp_data(&temp_s8);
                    sprintf_s(buf, "Gravity %f  temp: %d\r\n", gravity_y_s16,temp_s8);
                    writeAt(buf, 35, 4);

                    float accel_x_f, accel_y_f, accel_z_f;
                    bno055_convert_float_accel_x_msq(&accel_x_f);
                    bno055_convert_float_accel_y_msq(&accel_y_f);
                    bno055_convert_float_accel_z_msq(&accel_z_f);
                    sprintf_s(buf, "Accel:x: %f y: %f  z: %f\r\n", accel_x_f, accel_y_f, accel_z_f);
                    writeAt(buf, 35, 5);


                    s16 quaternion_w_s16, quaternion_x_s16, quaternion_y_s16, quaternion_z_s16;
                    bno055_read_quaternion_w(&quaternion_w_s16);
                    bno055_read_quaternion_x(&quaternion_x_s16);
                    bno055_read_quaternion_y(&quaternion_y_s16);
                    bno055_read_quaternion_z(&quaternion_z_s16);

                    struct bno055_quaternion_t quaternion;
                    bno055_read_quaternion_wxyz(&quaternion);

                    sprintf_s(buf, "Quat:w/x/y/z: %f  %f  %f  %f \r\n", quaternion.w, quaternion.x, quaternion.y, quaternion.z);
                    writeAt(buf, 35, 6);





                }

                } 
        } while (false == quit);

    }


    FT260_Close(ftHandle);
    return 0;
}

