    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x24(r1)
    stfd f31, 0x18(r1)
    fmr f31, f1
    stw r31, 0x14(r1)
    stw r30, 0x10(r1)
    mr r30, r4
    beq @L_801AD71C
    mr r31, r3
    b @L_801AD714
@L_801AD6F4:
    cmplwi r31, 0x0
    beq @L_801AD710
    rlwinm. r0, r30, 0, 28, 28
    beq @L_801AD710
    fmr f1, f31
    lwz r3, 0x18(r31)
    bl fn_801C29C4
@L_801AD710:
    lwz r31, 0x4(r31)
@L_801AD714:
    cmplwi r31, 0x0
    bne @L_801AD6F4
@L_801AD71C:
    lwz r0, 0x24(r1)
    lfd f31, 0x18(r1)
    lwz r31, 0x14(r1)
    lwz r30, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
