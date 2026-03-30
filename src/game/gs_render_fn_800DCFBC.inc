    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    lwz r0, lbl_8047AAF0(r13)
    lwz r31, lbl_8047AAEC(r13)
    mtctr r0
    cmplwi r0, 0x0
    ble @L_800DCFF8
    @L_800DCFE0:
    lbz r0, 0x0(r31)
    cmplwi r0, 0x0
    bne @L_800DCFF0
    b @L_800DCFFC
    @L_800DCFF0:
    addi r31, r31, 0x74
    bdnz @L_800DCFE0
    @L_800DCFF8:
    li r31, 0x0
    @L_800DCFFC:
    cmplwi r31, 0x0
    bne @L_800DD00C
    li r3, 0x0
    b @L_800DD0A4
    @L_800DD00C:
    li r8, 0x0
    lfs f1, lbl_8047CA78(r2)
    stw r8, 0x10(r31)
    li r7, 0x4
    lfs f0, lbl_8047CA8C(r2)
    li r6, 0x80
    stfs f1, 0x14(r31)
    addi r5, r31, 0x10
    addi r4, r31, 0x24
    addi r0, r31, 0x54
    stfs f1, 0x18(r31)
    addi r3, r31, 0x38
    stfs f1, 0x1c(r31)
    stw r8, 0x20(r31)
    stw r8, 0x24(r31)
    stfs f1, 0x28(r31)
    stfs f1, 0x2c(r31)
    stfs f1, 0x30(r31)
    stw r8, 0x34(r31)
    stfs f0, 0x54(r31)
    stw r8, 0x38(r31)
    stw r8, 0x3c(r31)
    sth r7, 0x40(r31)
    sth r8, 0x42(r31)
    stb r6, 0x44(r31)
    stb r6, 0x45(r31)
    stb r6, 0x46(r31)
    stb r8, 0x47(r31)
    stw r5, 0x48(r31)
    stw r4, 0x4c(r31)
    stw r0, 0x50(r31)
    bl fn_801A4344
    stw r3, 0xc(r31)
    li r4, 0x1
    li r0, 0x0
    mr r3, r31
    stb r4, 0x0(r31)
    stb r0, 0x1(r31)
    @L_800DD0A4:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
