    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stfd f31, 0x20(r1)
    psq_st f31, 0x28(r1), 0, 0
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    fmr f31, f1
    mr r30, r3
    mr r3, r5
    mr r31, r4
    addi r5, r1, 0x8
    bl fn_800A3A9C
    addi r3, r1, 0x8
    bl fn_800A3B38
    fdivs f1, f31, f1
    addi r3, r1, 0x8
    mr r4, r3
    bl fn_800A3AC0
    mr r4, r31
    mr r5, r30
    addi r3, r1, 0x8
    bl fn_800A3A78
    psq_l f31, 0x28(r1), 0, 0
    lwz r0, 0x34(r1)
    lfd f31, 0x20(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
