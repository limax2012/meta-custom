# meta-custom

This repository contains the `meta-custom` Yocto layer.

## Dependencies

This layer depends on the following layers (kirkstone branch):

- [poky](https://github.com/yoctoproject/poky)
- [meta-raspberrypi](https://github.com/agherzan/meta-raspberrypi)

## Adding the meta-custom layer to your build

1. `bitbake-layers add-layer meta-custom`

2. Add the path to `meta-custom` to build/conf/bblayers.conf

## Description

The `meta-custom` layer provides the following customizations for a minimal Raspberry Pi Zero 2 W image:

- Enable UART with console access
- Enable I2C and SPI along with corresponding device drivers
- Add modules to image: `i2c-tools`, Broadcom Wi-Fi, and Wi-Fi utilities
- Add I2C test application (get temperature from BMP280) to /usr/bin
- Set root filesystem partition size to ~200 MiB
- Set root password to `toor`
