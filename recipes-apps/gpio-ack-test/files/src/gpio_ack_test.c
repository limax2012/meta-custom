#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>

#define GPIO_CHIP "/dev/gpiochip0"
#define GPIO_LINE_RECEIVE 24
#define GPIO_LINE_SEND 25
#define CONSUMER_RECEIVE "gpio-receiver"
#define CONSUMER_SEND "gpio-sender"

int main(void) {
    struct gpiod_chip *chip;
    struct gpiod_line *line_receive, *line_send;
    struct gpiod_line_event event;
    int ret;

    chip = gpiod_chip_open(GPIO_CHIP);
    if (!chip) {
        perror("gpiod_chip_open");
        return 1;
    }

    line_receive = gpiod_chip_get_line(chip, GPIO_LINE_RECEIVE);
    if (!line_receive) {
        perror("gpiod_chip_get_line (receive)");
        gpiod_chip_close(chip);
        return 1;
    }

    line_send = gpiod_chip_get_line(chip, GPIO_LINE_SEND);
    if (!line_send) {
        perror("gpiod_chip_get_line (send)");
        gpiod_chip_close(chip);
        return 1;
    }

    ret = gpiod_line_request_rising_edge_events(line_receive, CONSUMER_RECEIVE);
    if (ret < 0) {
        perror("gpiod_line_request_rising_edge_events");
        gpiod_chip_close(chip);
        return 1;
    }

    ret = gpiod_line_request_output(line_send, CONSUMER_SEND, 0);
    if (ret < 0) {
        perror("gpiod_line_request_output");
        gpiod_chip_close(chip);
        return 1;
    }

    while (1) {
        ret = gpiod_line_event_wait(line_receive, NULL);
        if (ret < 0) {
            perror("gpiod_line_event_wait");
            break;
        }

        ret = gpiod_line_event_read(line_receive, &event);
        if (ret < 0) {
            perror("gpiod_line_event_read");
            break;
        }

        if (event.event_type == GPIOD_LINE_EVENT_RISING_EDGE) {
            printf("Received rising edge\n");
            sleep(1);
            printf("Sending ACK\n");
            gpiod_line_set_value(line_send, 1);
            usleep(1000);
            gpiod_line_set_value(line_send, 0);
        }
    }

    gpiod_line_release(line_receive);
    gpiod_line_release(line_send);
    gpiod_chip_close(chip);
    return 0;
}
