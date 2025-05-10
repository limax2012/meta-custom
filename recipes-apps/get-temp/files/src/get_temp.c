#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <fcntl.h>

int main() {
    // Variable init
    const unsigned char bmp280_addr = 0x76;
    const char filename[] = "/dev/i2c-1";
    int file;
    char ctrl_meas[2] = {0xF4, 0x21};
    char temp_start_reg[1] = {0xFA};
    char temp_data[3];
    int32_t raw_temp;

    // Open I2C device
    if ((file = open(filename, O_RDWR)) < 0) {
        printf("Failed to open the I2C device.\n");
        exit(1);
    }

    // Set slave address
    if (ioctl(file, I2C_SLAVE, bmp280_addr) < 0) {
        printf("Failed to set slave address.\n");
        exit(1);
    }

    // osrs_t = x1, osrs_p = skip, mode = forced
    if (write(file, ctrl_meas, 2) != 2) {
        printf("Failed to write data acquisition options to I2C device.\n");
    }
    usleep(5000);

    // Burst read temperature data
    if (write(file, temp_start_reg, 1) != 1) {
        printf("Failed to write start address to I2C device.\n");
        exit(1);
    }
    if (read(file, temp_data, 3) != 3) {
        printf("Failed to read from I2C device.\n");
        exit(1);
    }

    // Combine raw data
    raw_temp = ((int32_t)temp_data[0] << 12) | ((int32_t)temp_data[1] << 4) | (temp_data[2] >> 4);
    printf("Raw temperature reading: %d\n", raw_temp);

    return 0;
}
