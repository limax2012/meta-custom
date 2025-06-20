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
  - Receive and upload data from [STM32 Flappy Bird](https://github.com/limax2012/flappy-bird)
- Set root filesystem partition size to ~200 MiB
- Set root password to `toor`

## Receiving and Uploading Data from [STM32 Flappy Bird](https://github.com/limax2012/flappy-bird)

`/usr/bin/spi_logger` listens for score and temperature data over SPI and uploads it to Firebase.

### Hardware Connections

- **Power**
  - Pi gets power via USB
  - Shares 3.3V and GND with STM32
- **GPIO Pins**
  - Input pin `GPIO24`: STM32 ready signal
  - Output pin `GPIO25`: ACK signal to STM32
- **SPI Bus**
  - `MOSI`, `MISO`, `SCLK`, `CE0`: Connect Pi (SPI master) to STM32 (SPI slave)

### Setup: Wi-Fi, Firebase, and Startup Script

1. **Create `/etc/firebase.conf`:**
     ```
     URL=https://flappy-bird-scores-default-rtdb.firebaseio.com/.json
     AUTH=your_firebase_database_secret_or_key
     ```

2. **Add credentials to `wpa_supplicant.conf`:**
    ```
    wpa_passphrase "SSID" "password" >> /etc/wpa_supplicant.conf
    ```

3. **Startup Script**
   1. Create `/etc/init.d/stm32_flappy_uploader` and paste content from [gist](https://gist.github.com/limax2012/316031352eaaaa38f115e1450bfb2bf2)
   2. Make it executable: `chmod +x /etc/init.d/stm32_flappy_uploader`
   3. Enable on boot: `update-rc.d stm32_flappy_uploader defaults`
