    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stfd f31, 0x20(r1)
    psq_st f31, 0x28(r1), 0, 0
    stmw r27, 0xc(r1)
    li r3, 0x0
    lis r4, lbl_80401C10@ha
    addi r31, r4, lbl_80401C10@l
    lis r0, 0xf000
    stw r3, lbl_8047AC48(r13)
    addi r30, r31, 0x0
    lfs f31, lbl_8047CCD0(r2)
    addi r29, r31, 0x1c0
    stw r0, lbl_8047AC4C(r13)
    addi r28, r31, 0x1b0
    li r27, 0x0
    stw r3, lbl_8047AC50(r13)
    @L_800F8180:
    li r6, 0x0
    li r0, 0x3
    stw r6, 0x0(r30)
    addi r3, r30, 0x18
    li r4, 0x0
    li r5, 0xc
    stw r6, 0x8(r30)
    stw r0, 0xc(r30)
    stb r6, 0x10(r30)
    stw r6, 0x14(r30)
    stw r6, 0x30(r30)
    bl memset
    addi r3, r30, 0x24
    li r4, 0x0
    li r5, 0xc
    bl memset
    li r4, 0x0
    addi r27, r27, 0x1
    stb r4, 0x34(r30)
    li r3, 0x3
    cmpwi r27, 0x4
    li r0, 0x2
    stb r4, 0x35(r30)
    stb r4, 0x36(r30)
    stb r4, 0x37(r30)
    stfs f31, 0x48(r30)
    stfs f31, 0x4c(r30)
    stfs f31, 0x50(r30)
    stfs f31, 0x54(r30)
    stw r3, 0x5c(r30)
    stw r4, 0x60(r30)
    stw r4, 0x64(r30)
    stb r4, 0x68(r30)
    addi r30, r30, 0x6c
    stw r0, 0x0(r29)
    addi r29, r29, 0x4
    stw r4, 0x0(r28)
    addi r28, r28, 0x4
    blt @L_800F8180
    li r3, 0x0
    bl fn_800ABCF4
    bl fn_800AAF38
    addi r3, r31, 0x1c0
    bl fn_800AB4FC
    lwz r3, lbl_8047AC4C(r13)
    bl fn_800AAE34
    li r3, 0xb
    bl SISetSamplingRate
    lis r3, fn_800F8268@ha
    addi r3, r3, fn_800F8268@l
    bl fn_800ABF5C
    psq_l f31, 0x28(r1), 0, 0
    lfd f31, 0x20(r1)
    lmw r27, 0xc(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
