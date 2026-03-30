    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r5, 0x32
    stw r0, 0x14(r1)
    addi r0, r5, 0x2b62
    stw r31, 0xc(r1)
    mr r31, r4
    stw r30, 0x8(r1)
    mr r30, r3
    lwz r6, 0x0(r3)
    and r0, r6, r0
    stw r0, 0x0(r4)
    bl fn_800E3D00
    mr r4, r3
    addi r3, r31, 0x4
    bl fn_800E01D0
    mr r3, r30
    bl fn_800E3CF8
    mr r4, r3
    addi r3, r31, 0x10
    bl fn_800E01D0
    mr r3, r30
    bl fn_800E3CF0
    mr r4, r3
    addi r3, r31, 0x1c
    bl fn_800E01D0
    lwz r0, 0x90(r30)
    stw r0, 0x28(r31)
    lwz r0, 0xa8(r30)
    stw r0, 0x2c(r31)
    lfs f0, 0x98(r30)
    stfs f0, 0x30(r31)
    lfs f0, 0x94(r30)
    stfs f0, 0x34(r31)
    lwz r0, 0x8c(r30)
    stw r0, 0x40(r31)
    lfs f0, 0xb0(r30)
    stfs f0, 0x38(r31)
    lfs f0, 0xac(r30)
    stfs f0, 0x3c(r31)
    lwz r0, 0xa4(r30)
    stw r0, 0x44(r31)
    lwz r0, 0x0(r31)
    rlwinm. r0, r0, 0, 14, 14
    beq @L_800E9E1C
    lwz r3, 0x8(r30)
    lwz r30, 0x10(r3)
    cmplwi r30, 0x0
    bne @L_800E9D40
    la r3, lbl_8047CBE8(r2)
    li r4, 0x3e4
    la r5, lbl_8047CBF0(r2)
    bl fn_80196E10
    @L_800E9D40:
    addic. r0, r31, 0x48
    bne @L_800E9D5C
    lis r4, lbl_80270EA8@ha
    la r3, lbl_8047CBE8(r2)
    addi r5, r4, lbl_80270EA8@l
    li r4, 0x3e5
    bl fn_80196E10
    @L_800E9D5C:
    lwz r3, 0x38(r30)
    cmplwi r30, 0x0
    lwz r0, 0x3c(r30)
    stw r3, 0x48(r31)
    stw r0, 0x4c(r31)
    lwz r0, 0x40(r30)
    stw r0, 0x50(r31)
    bne @L_800E9D8C
    la r3, lbl_8047CBE8(r2)
    li r4, 0x2ec
    la r5, lbl_8047CBF0(r2)
    bl fn_80196E10
    @L_800E9D8C:
    lfs f0, 0x1c(r30)
    cmplwi r30, 0x0
    stfs f0, 0x54(r31)
    bne @L_800E9DAC
    la r3, lbl_8047CBE8(r2)
    li r4, 0x2fa
    la r5, lbl_8047CBF0(r2)
    bl fn_80196E10
    @L_800E9DAC:
    lfs f0, 0x20(r30)
    cmplwi r30, 0x0
    stfs f0, 0x58(r31)
    bne @L_800E9DCC
    la r3, lbl_8047CBE8(r2)
    li r4, 0x308
    la r5, lbl_8047CBF0(r2)
    bl fn_80196E10
    @L_800E9DCC:
    lfs f0, 0x24(r30)
    cmplwi r30, 0x0
    stfs f0, 0x5c(r31)
    bne @L_800E9DEC
    la r3, lbl_8047CBE8(r2)
    li r4, 0x351
    la r5, lbl_8047CBF0(r2)
    bl fn_80196E10
    @L_800E9DEC:
    addic. r0, r31, 0x60
    bne @L_800E9E04
    la r3, lbl_8047CBE8(r2)
    li r4, 0x352
    la r5, lbl_8047CBF8(r2)
    bl fn_80196E10
    @L_800E9E04:
    lwz r3, 0x2c(r30)
    lwz r0, 0x30(r30)
    stw r3, 0x60(r31)
    stw r0, 0x64(r31)
    lwz r0, 0x34(r30)
    stw r0, 0x68(r31)
    @L_800E9E1C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
