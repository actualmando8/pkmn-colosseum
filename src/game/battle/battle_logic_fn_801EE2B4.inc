    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    clrlwi. r3, r3, 16
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    mulli r0, r3, 0x38
    mr r30, r4
    lwz r5, -30900(r13)
    add r4, r5, r0
    beq @L_801EE2E4
    cmplwi r3, 0x60
    ble @L_801EE2E8
@L_801EE2E4:
    li r4, 0x0
@L_801EE2E8:
    cmplwi r4, 0x0
    beq @L_801EE314
    lhz r31, 0xa(r4)
    li r3, 0x0
    bl fn_801EF1E4
    mulli r5, r31, 0x18
    mr r0, r3
    mr r4, r30
    addi r3, r5, 0x490
    add r3, r0, r3
    bl fn_800F9E70
@L_801EE314:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
