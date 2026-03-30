    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r3, 0xc(r3)
    bl fn_801A49C0
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
