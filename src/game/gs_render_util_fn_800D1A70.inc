    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stfd f31, 0x30(r1)
    psq_st f31, 0x38(r1), 0, 0
    stw r31, 0x2c(r1)
    mr r31, r3
    lwz r3, 0xc(r3)
    lbz r0, 0x50(r3)
    cmplwi r0, 0x1
    bne @L_800D1AE0
    addi r4, r1, 0x1c
    addi r5, r1, 0x18
    bl fn_8019431C
    lwz r3, 0xc(r31)
    bl fn_801944F8
    fmr f31, f1
    lwz r3, 0xc(r31)
    bl fn_801944D0
    fmr f0, f1
    lis r3, -32704
    fmr f3, f31
    lfs f1, 0x1c(r1)
    lfs f2, 0x18(r1)
    addi r3, r3, 432
    fmr f4, f0
    bl fn_800E0678
    b @L_800D1B18
@L_800D1AE0:
    addi r4, r1, 0x10
    addi r5, r1, 0x8
    addi r6, r1, 0x14
    addi r7, r1, 0xc
    bl fn_801942C0
    lis r3, -32704
    lfs f1, 0x10(r1)
    lfs f2, 0x8(r1)
    addi r3, r3, 432
    lfs f3, 0x14(r1)
    lfs f4, 0xc(r1)
    lfs f5, -27916(r2)
    lfs f6, -27872(r2)
    bl fn_800E0698
@L_800D1B18:
    lis r3, -32704
    addi r3, r3, 432
    psq_l f31, 0x38(r1), 0, 0
    lwz r0, 0x44(r1)
    lfd f31, 0x30(r1)
    lwz r31, 0x2c(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
