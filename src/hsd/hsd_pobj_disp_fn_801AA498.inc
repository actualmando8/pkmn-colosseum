    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    mr r6, r3
    stw r0, 0x14(r1)
    lwz r5, 0x8(r3)
    mr r3, r4
    subi r0, r5, 0x1
    stw r0, 0x8(r6)
    bl fn_801A6960
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
