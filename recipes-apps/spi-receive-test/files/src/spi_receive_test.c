#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

int main() {
    int fd = open("/dev/spidev0.0", O_RDWR);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    uint8_t mode = SPI_MODE_0;
    uint8_t bits = 8;
    uint32_t speed = 100000;
    ioctl(fd, SPI_IOC_WR_MODE, &mode);
    ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);

    while (1) {
        uint8_t tx_dummy[4] = { 0x00, 0x00, 0x00, 0x00 };
        uint8_t rx_buf[4] = {0};

        struct spi_ioc_transfer tr = {
            .tx_buf = (unsigned long)tx_dummy,
            .rx_buf = (unsigned long)rx_buf,
            .len = 4,
            .speed_hz = speed,
            .bits_per_word = bits,
        };

        int ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
        if (ret < 1) {
            perror("SPI_IOC_MESSAGE");
            return 1;
        }

        printf("Received: ");
        for (int i = 0; i < 4; i++)
            printf("0x%02X ", rx_buf[i]);
        printf("\n");

        usleep(2000000);
    }

    close(fd);
    return 0;
}
