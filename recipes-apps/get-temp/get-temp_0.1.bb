SUMMARY = "Get BMP280 Temperature"
DESCRIPTION = "Recipe to build get_temp.c application"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

SRC_URI += "file://src"

S = "${WORKDIR}/src"

TARGET_CC_ARCH += "${LDFLAGS}"

do_compile() {
    ${CC} -o get_temp get_temp.c
}

do_install() {
    install -d ${D}${bindir}
    install -m 0755 get_temp ${D}${bindir}
}
