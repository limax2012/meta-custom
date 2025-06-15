SUMMARY = "Receive and ack data from STM32"
DESCRIPTION = "Recipe to build spi_gpio_master.c application"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI += "file://src"

S = "${WORKDIR}/src"

DEPENDS += "libgpiod"

do_compile() {
    ${CC} ${CFLAGS} ${LDFLAGS} -o spi_gpio_master spi_gpio_master.c -lgpiod
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 spi_gpio_master ${D}${bindir}
}
