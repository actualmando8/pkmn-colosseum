    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stfd f31, 0x18(r1)
    fmr f31, f1
    stw r31, 0x14(r1)
    mr r31, r4
    stw r30, 0x10(r1)
    mr. r30, r3
    beq @L_801A83A0
    rlwinm. r0, r31, 0, 29, 29
    beq @L_801A8390
    fmr f1, f31
    lwz r3, 0x14(r30)
    bl fn_801C29C4
@L_801A8390:
    fmr f1, f31
    lwz r3, 0x8(r30)
    mr r4, r31
    bl fn_801BEE68
@L_801A83A0:
    lwz r0, 0x24(r1)
    lfd f31, 0x18(r1)
    lwz r31, 0x14(r1)
    lwz r30, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
