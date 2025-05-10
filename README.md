# meta-custom Yocto layer

This repository contains the `meta-custom` Yocto layer.

## Dependencies

This layer depends on the following layers (kirkstone branch):

- [poky](https://github.com/yoctoproject/poky)
- [meta-raspberrypi](https://github.com/agherzan/meta-raspberrypi)

## Table of Contents

1. Adding the `meta-custom` layer to your build
2. Description

## 1. Adding the meta-custom layer to your build

```bash
bitbake-layers add-layer meta-custom
```

## Description

The `meta-custom` layer provides the following customizations for a minimal Raspberry Pi Zero 2 W image:

- Enables UART with console access on `serial0`
- Enables I2C-1 via Device Tree and the I2C character device driver in the kernel
- Adds `i2c-tools` to the image
- Sets root filesystem partition size to ~200 MiB
- Sets the root password to `toor`