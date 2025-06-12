SUMMARY = "Blink test on GPIO25"
DESCRIPTION = "Recipe to build gpio_25_test.c application"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI += "file://src"

S = "${WORKDIR}/src"

DEPENDS += "libgpiod"

do_compile() {
    ${CC} ${CFLAGS} ${LDFLAGS} -o gpio25_test gpio25_test.c -lgpiod
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 gpio25_test ${D}${bindir}
}
