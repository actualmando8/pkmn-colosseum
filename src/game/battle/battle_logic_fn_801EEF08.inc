    nofralloc
    clrlwi. r3, r3, 16
    lwz r4, -30900(r13)
    mulli r0, r3, 0x38
    add r4, r4, r0
    beq @L_801EEF24
    cmplwi r3, 0x60
    ble @L_801EEF28
@L_801EEF24:
    li r4, 0x0
@L_801EEF28:
    cmplwi r4, 0x0
    beq @L_801EEF38
    lhz r3, 0x8(r4)
    blr
@L_801EEF38:
    li r3, 0x0
    blr
