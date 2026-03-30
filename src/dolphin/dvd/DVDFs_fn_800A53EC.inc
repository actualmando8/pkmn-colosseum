    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x8(r1)
    lwz r12, 0x38(r4)
    cmplwi r12, 0x0
    beq @L_800A540C
    mtlr r12
    blrl
    @L_800A540C:
    lwz r0, 0xc(r1)
    addi r1, r1, 0x8
    mtlr r0
    blr
