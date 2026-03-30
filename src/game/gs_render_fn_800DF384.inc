    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r3
    lhz r0, 0x2(r3)
    lwz r30, 0x8(r3)
    andc. r31, r4, r0
    beq @L_800DF3D4
    rlwinm. r0, r31, 0, 29, 29
    stw r29, 0x20(r30)
    beq @L_800DF3C0
    bl fn_800DFABC
    @L_800DF3C0:
    lhz r0, 0x2(r29)
    mr r3, r30
    or r0, r0, r31
    sth r0, 0x2(r29)
    bl fn_801A6FF0
    @L_800DF3D4:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
