    nofralloc
    clrlwi. r3, r3, 16
    lwz r4, -30900(r13)
    mulli r0, r3, 0x38
    add r4, r4, r0
    beq @L_801EF010
    cmplwi r3, 0x60
    ble @L_801EF014
@L_801EF010:
    li r4, 0x0
@L_801EF014:
    cmplwi r4, 0x0
    beq @L_801EF024
    lhz r3, 0x2(r4)
    blr
@L_801EF024:
    li r3, 0x0
    blr
