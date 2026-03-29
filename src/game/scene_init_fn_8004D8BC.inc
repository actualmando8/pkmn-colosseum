    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    bl fn_80105624
    lwz r4, 0x60(r30)
    mr r31, r3
    lwz r3, 0x0(r4)
    lwz r3, 0x0(r3)
    bl fn_8004BE40
    bl fn_801D16F0
    cmplwi r3, 0x0
    bne @L_8004D908
    lhz r0, 0x0(r31)
    rlwinm r0, r0, 0, 27, 27
    cmpwi r0, 0x0
    bne @L_8004D910
    @L_8004D908:
    mr r3, r30
    bl fn_80102ED4
    @L_8004D910:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
