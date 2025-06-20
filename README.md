# meta-custom

This repository contains the `meta-custom` Yocto layer.

## Dependencies

This layer depends on the following layers (kirkstone branch):

- [poky](https://github.com/yoctoproject/poky)
- [meta-oe](https://github.com/openembedded/meta-openembedded/tree/kirkstone/meta-oe)
- [meta-raspberrypi](https://github.com/agherzan/meta-raspberrypi)

## Adding the meta-custom layer to your build

1. `bitbake-layers add-layer meta-custom`
2. Add the path to `meta-custom` to `build/conf/bblayers.conf`

## Description

The `meta-custom` layer provides the following customizations for a minimal Raspberry Pi Zero 2 W image:

- Enable UART with console access
- Enable I2C and SPI along with their corresponding device drivers
- Add modules to image: `libgpiod`, `curl`, `ca-certificates`, `i2c-tools`, Broadcom Wi-Fi, and Wi-Fi utilities
- Add test applications to `/usr/bin`:
  - Get temperature from BMP280 via I2C
  - SPI loopback test
  - Receive 4 bytes from STM32 via SPI
  - GPIO25 blink test and GPIO24 falling edge input test
  - Receive and ack GPIO signal from STM32
- Add [STM32 Flappy Bird](https://github.com/limax2012/flappy-bird) comms application (`/usr/bin/spi_logger.c`):
  - Receives score and temperature via SPI
  - Sends data as an entry to Firebase
- Set root filesystem partition size to ~200 MiB
- Set root password to `toor`

## Firebase Integration ([STM32 Flappy Bird](https://github.com/limax2012/flappy-bird))

To enable Firebase score submission from the STM32 flappy bird application, you must:

1. Use `wpa_supplicant` and `udhcpc` to connect to Wi-Fi.
2. Run `rdate -s time.nist.gov` to set the correct system time for TLS.
3. Create `/etc/firebase.conf` with the following format:

```
URL=https://flappy-bird-scores-default-rtdb.firebaseio.com/.json
AUTH=your_firebase_database_secret_or_key
```
