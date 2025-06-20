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

Setup for Wi-Fi/Firebase credentiald and startup script:
1. Create `/etc/firebase.conf` with the following format:
```
URL=https://flappy-bird-scores-default-rtdb.firebaseio.com/.json
AUTH=your_firebase_database_secret_or_key
```
2. Configure Wi-Fi credentials:
```
wpa_passphrase "SSID" "password" >> /etc/wpa_supplicant.conf
```
3. Install the startup script:
    1. Create `/etc/init.d/stm32_flappy_uploader` and paste the contents of [`stm32_flappy_uploader`](https://gist.github.com/limax2012/316031352eaaaa38f115e1450bfb2bf2)
    2. Make it executable: `chmod +x stm32_flappy_uploader`
    3. Register it to run at boot: `update-rc.d stm32_flappy_uploader defaults`
