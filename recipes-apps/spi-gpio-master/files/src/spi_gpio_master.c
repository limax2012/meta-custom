#include <gpiod.h>
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>

#define GPIO_CHIP "/dev/gpiochip0"
#define GPIO_READY 24   // STM32 -> Pi (READY signal)
#define GPIO_ACK   25   // Pi -> STM32 (ACK signal)
#define CONSUMER "spi-gpio-master"

int main(void) {
    // ------------------ Setup SPI ------------------
    int spi_fd = open("/dev/spidev0.0", O_RDWR);
    if (spi_fd < 0) {
        perror("open SPI");
        return 1;
    }

    uint8_t mode = SPI_MODE_0;
    uint8_t bits = 8;
    uint32_t speed = 50000;

    ioctl(spi_fd, SPI_IOC_WR_MODE, &mode);
    ioctl(spi_fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
    ioctl(spi_fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);

    // ------------------ Setup GPIO ------------------
    struct gpiod_chip *chip = gpiod_chip_open(GPIO_CHIP);
    if (!chip) {
        perror("gpiod_chip_open");
        return 1;
    }

    struct gpiod_line *line_ready = gpiod_chip_get_line(chip, GPIO_READY);
    struct gpiod_line *line_ack = gpiod_chip_get_line(chip, GPIO_ACK);
    if (!line_ready || !line_ack) {
        perror("gpiod_chip_get_line");
        gpiod_chip_close(chip);
        return 1;
    }

    if (gpiod_line_request_rising_edge_events(line_ready, CONSUMER) < 0) {
        perror("gpiod_line_request_rising_edge_events");
        gpiod_chip_close(chip);
        return 1;
    }

    if (gpiod_line_request_output(line_ack, CONSUMER, 0) < 0) {
        perror("gpiod_line_request_output");
        gpiod_chip_close(chip);
        return 1;
    }

    // ------------------ Main Loop ------------------
    struct gpiod_line_event event;
    while (1) {
        printf("Waiting for READY signal from STM32...\n");

        if (gpiod_line_event_wait(line_ready, NULL) <= 0) {
            perror("gpiod_line_event_wait");
            break;
        }

        if (gpiod_line_event_read(line_ready, &event) < 0) {
            perror("gpiod_line_event_read");
            break;
        }

        if (event.event_type != GPIOD_LINE_EVENT_RISING_EDGE) continue;

        // ------------------ SPI Transfer ------------------
        usleep(50000);

        uint8_t tx_dummy[4] = { 0x00, 0x00, 0x00, 0x00 };
        uint8_t rx_buf[4] = {0};

        struct spi_ioc_transfer tr = {
            .tx_buf = (unsigned long)tx_dummy,
            .rx_buf = (unsigned long)rx_buf,
            .len = 4,
            .speed_hz = speed,
            .bits_per_word = bits,
        };

        if (ioctl(spi_fd, SPI_IOC_MESSAGE(1), &tr) < 1) {
            perror("SPI_IOC_MESSAGE");
            break;
        }

        // Parse score and temp (big-endian)
        int16_t score = (rx_buf[0] << 8) | rx_buf[1];
        int16_t temp  = (rx_buf[2] << 8) | rx_buf[3];

        printf("Received -> Score: %d, Temp: %d\n", score, temp);

        // ------------------ Send ACK ------------------
        gpiod_line_set_value(line_ack, 1);
        usleep(1000); // 1 ms pulse
        gpiod_line_set_value(line_ack, 0);
        printf("ACK sent to STM32\n\n");

        usleep(500000);
    }

    // Cleanup
    gpiod_line_release(line_ready);
    gpiod_line_release(line_ack);
    gpiod_chip_close(chip);
    close(spi_fd);
    return 0;
}
