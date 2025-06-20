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
- Set root filesystem partition size to ~200 MiB
- Set root password to `toor`

## Receiving and uploading data from [STM32 Flappy Bird](https://github.com/limax2012/flappy-bird)

`/usr/bin/spi_logger` waits to receive score and temperature via SPI before uploading the data to Firebase.

Steps to enable Firebase score submission:
1. Use `wpa_passphrase "SSID" "password" >> /etc/wpa_supplicant.conf` to set Wi-Fi details.
2. Use `wpa_supplicant -B -i wlan0 -c /etc/wpa_supplicant.conf` and `udhcpc -i wlan0` to connect to Wi-Fi.
3. Run `rdate -s time.nist.gov` to set the correct system time for TLS.
4. Create `/etc/firebase.conf` with the following format:
```
URL=https://flappy-bird-scores-default-rtdb.firebaseio.com/.json
AUTH=your_firebase_database_secret_or_key
```
