    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    mr r4, r3
    li r3, 0xa9e
    bl fn_8019075C
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
