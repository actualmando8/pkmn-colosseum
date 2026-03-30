    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    li r0, 0x0
    stw r3, 0x8(r1)
    stw r31, 0x1c(r1)
    lwz r31, 0x8(r1)
    stw r0, 0x24(r31)
    lwz r0, 0x38(r31)
    cmplwi r0, 0x0
    beq @L_80180924
    lwz r12, 0x38(r31)
    lwz r3, 0x34(r31)
    lwz r4, 0x3c(r31)
    mtctr r12
    bctrl
    @L_80180924:
    li r0, 0x0
    stw r0, 0x20(r31)
    lwz r3, 0x28(r31)
    lwz r4, 0x30(r31)
    bl DCFlushRange
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
