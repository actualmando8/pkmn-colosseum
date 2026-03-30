    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x2e0(r1)
    stw r31, 0x2dc(r1)
    stw r30, 0x2d8(r1)
    mr r30, r4
    lwz r0, -24160(r13)
    lwz r3, -24224(r13)
    cmplwi r0, 0x0
    lhz r31, 0xe(r3)
    beq @L_800B9078
    addi r3, r1, 0x10
    bl OSClearContext
    addi r3, r1, 0x10
    bl OSSetCurrentContext
    lwz r12, -24160(r13)
    addi r3, r31, 0x0
    mtlr r12
    blrl
    addi r3, r1, 0x10
    bl OSClearContext
    mr r3, r30
    bl OSSetCurrentContext
@L_800B9078:
    lwz r3, -24224(r13)
    lhzu r0, 0xa(r3)
    rlwinm r0, r0, 0, 30, 28
    ori r0, r0, 0x4
    sth r0, 0x0(r3)
    lwz r0, 0x2e4(r1)
    lwz r31, 0x2dc(r1)
    lwz r30, 0x2d8(r1)
    addi r1, r1, 0x2e0
    mtlr r0
    blr
