    nofralloc
    clrlwi. r3, r3, 16
    lwz r5, -30900(r13)
    mulli r0, r3, 0x38
    add r5, r5, r0
    beq @L_801EEFC8
    cmplwi r3, 0x60
    ble @L_801EEFCC
@L_801EEFC8:
    li r5, 0x0
@L_801EEFCC:
    cmplwi r5, 0x0
    beq @L_801EEFEC
    cmpwi r4, 0x0
    bne @L_801EEFE4
    lhz r3, 0x4(r5)
    blr
@L_801EEFE4:
    lhz r3, 0x6(r5)
    blr
@L_801EEFEC:
    li r3, 0x0
    blr
