    nofralloc
    stwu r1, -0x90(r1)
    mflr r0
    stw r0, 0x94(r1)
    stw r31, 0x8c(r1)
    stw r30, 0x88(r1)
    bne cr1, @L_800F746C
    stfd f1, 0x28(r1)
    stfd f2, 0x30(r1)
    stfd f3, 0x38(r1)
    stfd f4, 0x40(r1)
    stfd f5, 0x48(r1)
    stfd f6, 0x50(r1)
    stfd f7, 0x58(r1)
    stfd f8, 0x60(r1)
    @L_800F746C:
    addi r11, r1, 0x98
    addi r0, r1, 0x8
    lis r12, 0x200
    lis r31, lbl_80271068@ha
    stw r3, 0x8(r1)
    addi r30, r1, 0x70
    addi r31, r31, lbl_80271068@l
    stw r5, 0x10(r1)
    mr r5, r30
    stw r4, 0xc(r1)
    stw r6, 0x14(r1)
    stw r7, 0x18(r1)
    stw r8, 0x1c(r1)
    stw r9, 0x20(r1)
    stw r10, 0x24(r1)
    stw r12, 0x70(r1)
    stw r11, 0x74(r1)
    stw r0, 0x78(r1)
    bl fn_800F6D18
    mr. r30, r3
    bne @L_800F74C8
    li r3, 0x0
    b @L_800F75E4
    @L_800F74C8:
    lbz r0, 0x4(r30)
    cmplwi r0, 0x0
    bne @L_800F74E8
    lwz r4, 0x8(r30)
    addi r3, r31, 0x120
    crxor 6, 6, 6
    bl fn_800DD970
    b @L_800F7570
    @L_800F74E8:
    cmplwi r0, 0x3
    bne @L_800F74FC
    li r0, 0x0
    stb r0, 0x4(r30)
    b @L_800F7570
    @L_800F74FC:
    lwz r3, 0x14(r30)
    addi r0, r3, 0x1
    stw r0, 0x14(r30)
    lbz r4, 0x0(r3)
    cmpwi r4, 0x26
    blt @L_800F7524
    addi r3, r31, 0x150
    crxor 6, 6, 6
    bl fn_800DD38C
    b @L_800F7548
    @L_800F7524:
    lis r3, lbl_803155D0@ha
    clrlslwi r0, r4, 24, 2
    addi r3, r3, lbl_803155D0@l
    lwzx r12, r3, r0
    cmplwi r12, 0x0
    beq @L_800F7548
    mr r3, r30
    mtctr r12
    bctrl
    @L_800F7548:
    lwz r3, 0x28(r30)
    cmpwi r3, 0x0
    ble @L_800F74C8
    subic. r3, r3, 0x1
    addi r0, r3, 0x1
    mtctr r0
    blt @L_800F74C8
    @L_800F7564:
    subi r3, r3, 0x1
    bdnz @L_800F7564
    b @L_800F74C8
    @L_800F7570:
    lbz r0, 0x4(r30)
    cmplwi r0, 0x4
    bne @L_800F7584
    li r0, 0x0
    stb r0, 0x4(r30)
    @L_800F7584:
    lwz r3, 0x28(r30)
    cmpwi r3, 0x0
    bgt @L_800F75A8
    addi r3, r31, 0x14
    crxor 6, 6, 6
    bl fn_800DD38C
    lwz r0, 0x6c(r30)
    stw r0, 0x68(r1)
    b @L_800F75C0
    @L_800F75A8:
    subi r3, r3, 0x1
    slwi r0, r3, 2
    stw r3, 0x28(r30)
    add r3, r30, r0
    lwz r0, 0x6c(r3)
    stw r0, 0x68(r1)
    @L_800F75C0:
    lwz r12, 0x10(r30)
    lwz r4, 0x68(r1)
    cmplwi r12, 0x0
    stw r4, 0x6c(r1)
    beq @L_800F75E0
    mr r3, r30
    mtctr r12
    bctrl
    @L_800F75E0:
    lwz r3, 0x6c(r1)
    @L_800F75E4:
    lwz r0, 0x94(r1)
    lwz r31, 0x8c(r1)
    lwz r30, 0x88(r1)
    mtlr r0
    addi r1, r1, 0x90
    blr
