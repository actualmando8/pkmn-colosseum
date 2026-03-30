    nofralloc
    mflr r0
    li r3, 0x1
    stw r0, 0x4(r1)
    stwu r1, -0x8(r1)
    bl fn_800BD640
    lwz r0, 0xc(r1)
    addi r1, r1, 0x8
    mtlr r0
    blr
