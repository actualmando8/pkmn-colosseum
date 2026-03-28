    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    clrlwi. r3, r3, 16
    stw r0, 0x14(r1)
    mulli r0, r3, 0x38
    lwz r6, -30900(r13)
    add r6, r6, r0
    beq @L_801EE8BC
    cmplwi r3, 0x60
    ble @L_801EE8C0
@L_801EE8BC:
    li r6, 0x0
@L_801EE8C0:
    cmplwi r6, 0x0
    beq @L_801EE8E4
    clrlslwi r0, r4, 16, 1
    add r3, r6, r0
    lhz r3, 0x24(r3)
    cmplwi r3, 0x0
    beq @L_801EE8E4
    extsb r4, r5
    bl fn_8019075C
@L_801EE8E4:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
