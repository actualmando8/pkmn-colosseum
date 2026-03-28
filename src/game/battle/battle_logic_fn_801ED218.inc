    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    bne @L_801ED238
    li r3, 0x0
    li r4, 0xb
    bl fn_80129280
@L_801ED238:
    lwz r0, 0x14(r1)
    lbz r3, 0x0(r3)
    mtlr r0
    addi r1, r1, 0x10
    blr
