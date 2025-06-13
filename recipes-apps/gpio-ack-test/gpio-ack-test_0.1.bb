SUMMARY = "Test to receive and ack GPIO signals from STM32"
DESCRIPTION = "Recipe to build gpio_ack_test.c application"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI += "file://src"

S = "${WORKDIR}/src"

DEPENDS += "libgpiod"

do_compile() {
    ${CC} ${CFLAGS} ${LDFLAGS} -o gpio_ack_test gpio_ack_test.c -lgpiod
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 gpio_ack_test ${D}${bindir}
}
