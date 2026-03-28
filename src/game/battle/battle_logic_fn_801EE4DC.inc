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
    beq @L_801EE50C
    cmplwi r3, 0x60
    ble @L_801EE510
@L_801EE50C:
    li r4, 0x0
@L_801EE510:
    cmplwi r4, 0x0
    beq @L_801EE530
    lhz r31, 0xa(r4)
    li r3, 0x0
    bl fn_801EF1E4
    mulli r4, r31, 0xc
    addi r0, r4, 0x6
    sthx r30, r3, r0
@L_801EE530:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
