    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    clrlwi. r3, r3, 16
    stw r0, 0x14(r1)
    mulli r0, r3, 0x38
    lwz r5, -30900(r13)
    add r5, r5, r0
    beq @L_801EEE14
    cmplwi r3, 0x60
    ble @L_801EEE18
@L_801EEE14:
    li r5, 0x0
@L_801EEE18:
    cmplwi r5, 0x0
    beq @L_801EEE34
    lhz r3, 0xc(r5)
    cmplwi r3, 0x0
    beq @L_801EEE34
    clrlwi r4, r4, 24
    bl fn_8019075C
@L_801EEE34:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
