# meta-custom

This repository contains the `meta-custom` Yocto layer.

## Dependencies

This layer depends on the following layers (kirkstone branch):

- [poky](https://github.com/yoctoproject/poky)
- [meta-oe](https://github.com/openembedded/meta-openembedded/tree/kirkstone/meta-oe)
- [meta-raspberrypi](https://github.com/agherzan/meta-raspberrypi)

## Adding the meta-custom layer to your build

1. `bitbake-layers add-layer meta-custom`

2. Add the path to `meta-custom` to build/conf/bblayers.conf

## Description

The `meta-custom` layer provides the following customizations for a minimal Raspberry Pi Zero 2 W image:

- Enable UART with console access
- Enable I2C and SPI along with corresponding device drivers
- Add modules to image: `libgpiod`, `i2c-tools`, Broadcom Wi-Fi, and Wi-Fi utilities
- Add test applications to /usr/bin:  
  - Get temperature from BMP280 via I2C
  - SPI loopback test
  - Receive 4 bytes from STM32 via SPI
  - GPIO25 blink test and GPIO24 falling edge input test
  - Receive and ack GPIO signal from STM32
- Add application that receives score and temperature from [STM32 flappy bird](https://github.com/limax2012/flappy-bird) via SPI
- Set root filesystem partition size to ~200 MiB
- Set root password to `toor`
