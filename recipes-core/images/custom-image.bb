SUMMARY = "My custom Linux image."

IMAGE_INSTALL = "packagegroup-core-boot ${CORE_IMAGE_EXTRA_INSTALL}"

IMAGE_LINGUAS = " "

LICENSE = "MIT"

inherit core-image
inherit extrausers

# Set rootfs to 200 MiB by default
IMAGE_OVERHEAD_FACTOR ?= "1.0"
IMAGE_ROOTFS_SIZE ?= "204800"

# Change root password
EXTRA_USERS_PARAMS = "\
  usermod -p '\$6\$j.DU3bT9XE2JCyAJ\$M7TmaLOrEnwSxLNEozocNulLW9LrUP9i0Lhe8CI4avM4vWspUGWaq1JYIFkoR9U/uSSMe2x7eRUClX.RVqnXo/' root \
"

IMAGE_INSTALL += "get-temp"
IMAGE_INSTALL += "spi-loopback-test"
IMAGE_INSTALL += "spi-receive-test"
IMAGE_INSTALL += "gpio-output-test"
IMAGE_INSTALL += "gpio-input-test"
IMAGE_INSTALL += "gpio-ack-test"

IMAGE_INSTALL += "\
  libgpiod \
  libgpiod-tools \
  i2c-tools \
  kernel-module-brcmfmac \
  linux-firmware-bcm43430 \
  iw \
  wpa-supplicant \
  wireless-regdb-static \
"
