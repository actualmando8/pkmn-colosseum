    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    lis r4, lbl_80272B40@ha
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    addi r31, r4, lbl_80272B40@l
    stw r30, 0x8(r1)
    beq @L_80138798
    mr r30, r3
    lhz r3, 0xa(r3)
    lhz r4, 0xc(r30)
    bl fn_800F9318
    cmplwi r3, 0x0
    bne @L_80138734
    lhz r4, 0xa(r30)
    addi r3, r31, 0x0
    lhz r5, 0xc(r30)
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    b @L_801387A8
    @L_80138734:
    lhz r0, 0x8(r30)
    mulli r3, r0, 0x97c
    bl fn_800E3534
    clrlwi. r0, r3, 16
    sth r3, 0x0(r30)
    bne @L_80138760
    addi r3, r31, 0x60
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    b @L_801387A8
    @L_80138760:
    bl fn_800E27B0
    stw r3, 0x4(r30)
    li r4, 0x0
    lhz r0, 0x8(r30)
    lwz r3, 0x4(r30)
    mulli r5, r0, 0x97c
    bl memset
    mr r3, r30
    li r4, 0x1
    bl fn_80138838
    li r0, 0x0
    li r3, 0x1
    sth r0, 0x10(r30)
    b @L_801387A8
    @L_80138798:
    addi r3, r31, 0xb0
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    @L_801387A8:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
