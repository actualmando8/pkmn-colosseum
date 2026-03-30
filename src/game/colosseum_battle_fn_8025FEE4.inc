    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    bne @L_8025FF04
    li r3, 0x0
    li r4, 0xc
    bl fn_80129280
@L_8025FF04:
    lwz r0, 0x14(r1)
    lhz r3, 0x0(r3)
    mtlr r0
    addi r1, r1, 0x10
    blr
