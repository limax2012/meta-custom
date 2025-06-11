SUMMARY = "SPI Receive from STM32 Test"
DESCRIPTION = "Recipe to build spi_receive_test.c application"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI += "file://src"

S = "${WORKDIR}/src"

TARGET_CC_ARCH += "${LDFLAGS}"

do_compile() {
    ${CC} -o spi_receive_test spi_receive_test.c
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 spi_receive_test ${D}${bindir}
}
