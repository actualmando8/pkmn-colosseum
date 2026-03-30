    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x0
    stw r0, 0x14(r1)
    clrlwi r0, r3, 16
    stw r31, 0xc(r1)
    lwz r5, lbl_80478B00(r13)
    lhz r3, 0x0(r5)
    mtctr r3
    cmpwi r3, 0x0
    ble @L_800F72CC
    @L_800F72A0:
    lwz r3, 0xc(r5)
    add r31, r3, r4
    lbz r3, 0x4(r31)
    cmplwi r3, 0x0
    beq @L_800F72C4
    lhz r3, 0x6(r31)
    cmplw r3, r0
    bne @L_800F72C4
    b @L_800F72D0
    @L_800F72C4:
    addi r4, r4, 0x16c
    bdnz @L_800F72A0
    @L_800F72CC:
    li r31, 0x0
    @L_800F72D0:
    cmplwi r31, 0x0
    bne @L_800F72E0
    li r3, 0x0
    b @L_800F7304
    @L_800F72E0:
    li r0, 0x4
    stb r0, 0x4(r31)
    lwz r3, 0xc(r31)
    cmplwi r3, 0x0
    beq @L_800F7300
    bl fn_800F05A0
    li r0, 0x0
    stw r0, 0xc(r31)
    @L_800F7300:
    li r3, 0x0
    @L_800F7304:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
