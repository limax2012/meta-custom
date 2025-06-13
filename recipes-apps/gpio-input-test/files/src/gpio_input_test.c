#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>

#define GPIO_CHIP "/dev/gpiochip0"
#define GPIO_LINE 24
#define CONSUMER "gpio24-falling-test"

int main(void) {
    struct gpiod_chip *chip;
    struct gpiod_line *line;
    struct gpiod_line_event event;
    int ret;

    chip = gpiod_chip_open(GPIO_CHIP);
    if (!chip) {
        perror("gpiod_chip_open");
        return 1;
    }

    line = gpiod_chip_get_line(chip, GPIO_LINE);
    if (!line) {
        perror("gpiod_chip_get_line");
        gpiod_chip_close(chip);
        return 1;
    }

    ret = gpiod_line_request_falling_edge_events(line, CONSUMER);
    if (ret < 0) {
        perror("gpiod_line_request_falling_edge_events");
        gpiod_chip_close(chip);
        return 1;
    }

    while (1) {
        ret = gpiod_line_event_wait(line, NULL);
        if (ret < 0) {
            perror("gpiod_line_event_wait");
            break;
        }

        ret = gpiod_line_event_read(line, &event);
        if (ret < 0) {
            perror("gpiod_line_event_read");
            break;
        }

        if (event.event_type == GPIOD_LINE_EVENT_FALLING_EDGE) {
            printf("Falling edge detected on GPIO24\n");
        }
    }

    gpiod_line_release(line);
    gpiod_chip_close(chip);
    return 0;
}
