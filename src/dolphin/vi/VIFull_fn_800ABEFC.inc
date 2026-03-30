    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x2e0(r1)
    stw r31, 0x2dc(r1)
    addi r31, r4, 0x0
    lwz r0, -24420(r13)
    cmplwi r0, 0x0
    beq @L_800ABF48
    addi r3, r1, 0x10
    bl OSClearContext
    addi r3, r1, 0x10
    bl OSSetCurrentContext
    lwz r12, -24420(r13)
    mtlr r12
    blrl
    addi r3, r1, 0x10
    bl OSClearContext
    mr r3, r31
    bl OSSetCurrentContext
@L_800ABF48:
    lwz r0, 0x2e4(r1)
    lwz r31, 0x2dc(r1)
    addi r1, r1, 0x2e0
    mtlr r0
    blr
