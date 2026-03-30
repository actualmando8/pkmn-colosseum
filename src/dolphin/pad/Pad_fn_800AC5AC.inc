    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x2e0(r1)
    stw r31, 0x2dc(r1)
    lis r31, 0xcc00
    lwz r0, 0x6c00(r31)
    addi r3, r1, 0x10
    stw r30, 0x2d8(r1)
    ori r0, r0, 0x8
    stw r0, 0x6c00(r31)
    addi r30, r4, 0x0
    bl OSClearContext
    addi r3, r1, 0x10
    bl OSSetCurrentContext
    lwz r12, lbl_8047A8C8(r13)
    cmplwi r12, 0x0
    beq @L_800AC600
    addi r3, r31, 0x6c00
    mtlr r12
    lwz r3, 0x8(r3)
    blrl
    @L_800AC600:
    addi r3, r1, 0x10
    bl OSClearContext
    mr r3, r30
    bl OSSetCurrentContext
    lwz r0, 0x2e4(r1)
    lwz r31, 0x2dc(r1)
    lwz r30, 0x2d8(r1)
    addi r1, r1, 0x2e0
    mtlr r0
    blr
