    nofralloc
    clrlwi. r3, r3, 16
    lwz r4, -30900(r13)
    mulli r0, r3, 0x38
    add r4, r4, r0
    beq @L_801EEE60
    cmplwi r3, 0x60
    ble @L_801EEE64
@L_801EEE60:
    li r4, 0x0
@L_801EEE64:
    lbz r3, 0x0(r4)
    blr
