    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stfd f31, 0x10(r1)
    psq_st f31, 0x18(r1), 0, 0
    stw r31, 0xc(r1)
    mr r31, r3
    fmr f31, f1
    lbz r0, 0x2(r3)
    cmplwi r0, 0x0
    beq @L_800DCB5C
    bl fn_800D37CC
    cmpwi r3, 0x32
    bne @L_800DCB30
    lfs f0, lbl_8047CA88(r2)
    fmuls f31, f31, f0
    @L_800DCB30:
    stfs f31, 0x64(r31)
    lis r3, fn_801C027C@ha
    addi r6, r3, fn_801C027C@l
    lis r4, 0x1
    lwz r3, 0xc(r31)
    subi r5, r4, 0x1
    lfs f1, 0x64(r31)
    li r4, 0x7
    li r7, 0x1
    creqv 6, 6, 6
    bl fn_801C028C
    @L_800DCB5C:
    psq_l f31, 0x18(r1), 0, 0
    lwz r0, 0x24(r1)
    lfd f31, 0x10(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
