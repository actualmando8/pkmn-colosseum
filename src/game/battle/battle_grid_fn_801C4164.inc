    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stfd f31, 0x18(r1)
    stw r31, 0x14(r1)
    stw r30, 0x10(r1)
    fmr f31, f1
    mr r30, r3
    fmr f1, f2
    mr r3, r4
    mr r4, r5
    mr r5, r30
    bl fn_801C423C
    fmr f1, f31
    mr r31, r3
    mr r3, r30
    bl fn_801C41C8
    mr r3, r31
    lwz r0, 0x24(r1)
    lfd f31, 0x18(r1)
    lwz r31, 0x14(r1)
    lwz r30, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
