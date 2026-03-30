    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stfd f31, 0x20(r1)
    psq_st f31, 0x28(r1), 0, 0
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r30, r3
    mr r31, r4
    lbz r0, 0x1(r3)
    addi r4, r31, 0x4
    stb r0, 0x0(r31)
    lwz r3, 0xc(r3)
    bl fn_801A49C0
    lwz r3, 0xc(r30)
    addi r4, r31, 0x10
    bl fn_801A48F4
    lbz r0, 0x2(r30)
    lwz r29, 0x1c(r31)
    cmplwi r0, 0x0
    beq @L_800DC944
    lwz r3, 0xc(r30)
    bl fn_801A66E0
    lwz r0, 0x58(r30)
    cmplw r29, r0
    bgt @L_800DC944
    stw r29, 0x60(r30)
    lwz r3, 0x8(r30)
    lwz r0, 0x60(r30)
    lwz r4, 0x4(r3)
    slwi r0, r0, 2
    lwz r3, 0xc(r30)
    lwzx r4, r4, r0
    bl fn_801A426C
    lwz r3, 0xc(r30)
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
    lwz r3, 0xc(r30)
    crxor 6, 6, 6
    bl fn_801C028C
    lfs f0, lbl_8047AAF4(r13)
    stfs f0, 0x6c(r30)
    @L_800DC944:
    lbz r0, 0x2(r30)
    lfs f0, 0x20(r31)
    cmplwi r0, 0x0
    beq @L_800DC958
    stfs f0, 0x68(r30)
    @L_800DC958:
    lbz r0, 0x2(r30)
    lfs f31, 0x24(r31)
    cmplwi r0, 0x0
    beq @L_800DC9A8
    bl fn_800D37CC
    cmpwi r3, 0x32
    bne @L_800DC97C
    lfs f0, lbl_8047CA88(r2)
    fmuls f31, f31, f0
    @L_800DC97C:
    stfs f31, 0x64(r30)
    lis r3, fn_801C027C@ha
    addi r6, r3, fn_801C027C@l
    lis r4, 0x1
    lwz r3, 0xc(r30)
    subi r5, r4, 0x1
    lfs f1, 0x64(r30)
    li r4, 0x7
    li r7, 0x1
    creqv 6, 6, 6
    bl fn_801C028C
    @L_800DC9A8:
    lwz r0, 0x28(r31)
    stw r0, 0x5c(r30)
    lbz r0, 0x2(r31)
    stb r0, 0x70(r30)
    lbz r0, 0x3(r31)
    stb r0, 0x71(r30)
    lbz r0, 0x3(r30)
    cmplwi r0, 0x0
    beq @L_800DC9EC
    lbz r0, 0x2(r30)
    cmplwi r0, 0x0
    beq @L_800DC9EC
    li r3, 0x1
    li r0, 0x0
    stb r3, 0x3(r30)
    stb r0, 0x70(r30)
    stb r3, 0x71(r30)
    @L_800DC9EC:
    psq_l f31, 0x28(r1), 0, 0
    lwz r0, 0x34(r1)
    lfd f31, 0x20(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
