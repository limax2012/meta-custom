#include <gpiod.h>
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <curl/curl.h>

#define GPIO_CHIP "/dev/gpiochip0"
#define GPIO_READY 24
#define GPIO_ACK   25
#define CONSUMER "spi-gpio-master"
#define CONFIG_PATH "/etc/firebase.conf"

char firebase_url[512] = {0};
char firebase_auth[256] = {0};

void load_firebase_config(const char *filepath) {
    FILE *f = fopen(filepath, "r");
    if (!f) {
        perror("fopen(firebase.conf)");
        exit(1);
    }

    char line[1024];
    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "URL=", 4) == 0) {
            strncpy(firebase_url, line + 4, sizeof(firebase_url) - 1);
            firebase_url[strcspn(firebase_url, "\r\n")] = 0;
        } else if (strncmp(line, "AUTH=", 5) == 0) {
            strncpy(firebase_auth, line + 5, sizeof(firebase_auth) - 1);
            firebase_auth[strcspn(firebase_auth, "\r\n")] = 0;
        }
    }

    fclose(f);

    if (strlen(firebase_url) == 0 || strlen(firebase_auth) == 0) {
        fprintf(stderr, "Missing URL or AUTH in %s\n", filepath);
        exit(1);
    }
}

void upload_to_firebase(int16_t score, int16_t temp) {
    CURL *curl = curl_easy_init();
    if (!curl) return;

    char url[768];
    snprintf(url, sizeof(url), "%s?auth=%s", firebase_url, firebase_auth);

    char json_data[256];
    snprintf(json_data, sizeof(json_data),
             "{\"score\":%d,\"temp\":%d,\"timestamp\":{\".sv\":\"timestamp\"}}",
             score, temp);

    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "spi-logger/1.0");
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "Firebase upload failed: %s\n", curl_easy_strerror(res));
    }

    curl_easy_cleanup(curl);
}

int main(void) {
    load_firebase_config(CONFIG_PATH);
    curl_global_init(CURL_GLOBAL_DEFAULT);

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

        int16_t score = (rx_buf[0] << 8) | rx_buf[1];
        int16_t temp  = (rx_buf[2] << 8) | rx_buf[3];

        printf("Received -> Score: %d, Temp: %d\n", score, temp);

        gpiod_line_set_value(line_ack, 1);
        usleep(1000);
        gpiod_line_set_value(line_ack, 0);
        printf("ACK sent to STM32\n");

        if (score > 0) {
            upload_to_firebase(score, temp);
        } else {
            printf("Score is 0 or negative, skipping upload.");
        }

        printf("\n");

        usleep(100000);
    }

    curl_global_cleanup();
    gpiod_line_release(line_ready);
    gpiod_line_release(line_ack);
    gpiod_chip_close(chip);
    close(spi_fd);
    return 0;
}
