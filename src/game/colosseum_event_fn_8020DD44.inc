    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    bl fn_8020E0F8
    bl fn_8020E0B0
    mr r0, r3
    li r3, 0x0
    mr r4, r0
    li r5, 0x7
    li r6, 0x0
    bl fn_801F54A4
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
