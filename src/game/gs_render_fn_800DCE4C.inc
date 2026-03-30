    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    lwz r0, lbl_8047AAF0(r13)
    lwz r31, lbl_8047AAEC(r13)
    mtctr r0
    cmplwi r0, 0x0
    ble @L_800DCE88
    @L_800DCE70:
    lbz r0, 0x0(r31)
    cmplwi r0, 0x0
    bne @L_800DCE80
    b @L_800DCE8C
    @L_800DCE80:
    addi r31, r31, 0x74
    bdnz @L_800DCE70
    @L_800DCE88:
    li r31, 0x0
    @L_800DCE8C:
    cmplwi r31, 0x0
    bne @L_800DCE9C
    li r3, 0x0
    b @L_800DCFA8
    @L_800DCE9C:
    stw r3, 0x8(r31)
    lwz r3, 0x8(r31)
    lwz r3, 0x0(r3)
    bl fn_801A4344
    stw r3, 0xc(r31)
    li r5, 0x1
    li r4, 0x0
    stb r5, 0x0(r31)
    stb r4, 0x1(r31)
    stb r4, 0x3(r31)
    lwz r3, 0x8(r31)
    lwz r0, 0x4(r3)
    cmplwi r0, 0x0
    beq @L_800DCFA0
    stb r5, 0x2(r31)
    lfs f0, lbl_8047CA70(r2)
    stfs f0, 0x64(r31)
    stw r5, 0x5c(r31)
    stb r4, 0x70(r31)
    stw r4, 0x58(r31)
    b @L_800DCEFC
    @L_800DCEF0:
    lwz r3, 0x58(r31)
    addi r0, r3, 0x1
    stw r0, 0x58(r31)
    @L_800DCEFC:
    lwz r3, 0x8(r31)
    lwz r0, 0x58(r31)
    lwz r3, 0x4(r3)
    slwi r0, r0, 2
    lwzx r0, r3, r0
    cmplwi r0, 0x0
    bne @L_800DCEF0
    lbz r0, 0x2(r31)
    cmplwi r0, 0x0
    beq @L_800DCFA4
    lwz r3, 0xc(r31)
    bl fn_801A66E0
    lwz r0, 0x58(r31)
    cmplwi r0, 0x0
    blt @L_800DCFA4
    li r0, 0x0
    stw r0, 0x60(r31)
    lwz r3, 0x8(r31)
    lwz r0, 0x60(r31)
    lwz r4, 0x4(r3)
    slwi r0, r0, 2
    lwz r3, 0xc(r31)
    lwzx r4, r4, r0
    bl fn_801A426C
    lwz r3, 0xc(r31)
    lfs f1, lbl_8047CA78(r2)
    bl fn_801A6370
    lfs f0, lbl_8047CA78(r2)
    lis r3, fn_800DD128@ha
    addi r6, r3, fn_800DD128@l
    li r4, 0x7
    stfs f0, lbl_8047AAF4(r13)
    lis r3, 0x1
    subi r5, r3, 0x1
    li r7, 0x0
    lwz r3, 0xc(r31)
    crxor 6, 6, 6
    bl fn_801C028C
    lfs f0, lbl_8047AAF4(r13)
    stfs f0, 0x6c(r31)
    b @L_800DCFA4
    @L_800DCFA0:
    stb r4, 0x2(r31)
    @L_800DCFA4:
    mr r3, r31
    @L_800DCFA8:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
