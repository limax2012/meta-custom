# meta-custom

This repository contains the `meta-custom` Yocto layer.

## Dependencies

This layer depends on the following layers (kirkstone branch):

- [poky](https://github.com/yoctoproject/poky)
- [meta-raspberrypi](https://github.com/agherzan/meta-raspberrypi)

## Table of Contents

1. Adding the `meta-custom` layer to your build
2. Description

## 1. Adding the meta-custom layer to your build

1. `bitbake-layers add-layer meta-custom`

2. Add the path to `meta-custom` to build/conf/bblayers.conf

## 2. Description

The `meta-custom` layer provides the following customizations for a minimal Raspberry Pi Zero 2 W image:

- Enable UART with console access on `serial0`
- Enable I2C-1 via Device Tree and the I2C character device driver in the kernel
- Add `i2c-tools` to the image
- Set root filesystem partition size to ~200 MiB
- Set the root password to `toor`
